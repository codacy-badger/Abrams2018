#include "Engine/Audio/AudioSystem.hpp"

#include "Engine/Core/BuildConfig.cpp"
#include "Engine/Core/FileUtils.hpp"

#include "Engine/Audio/Wav.hpp"

#include "Engine/Input/InputSystem.hpp"

#include <algorithm>

AudioSystem::AudioSystem(std::size_t max_channels /*= 1024*/)
    : EngineSubsystem()
    , _max_channels(max_channels)
{
    ::CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    GUARANTEE_OR_DIE(SUCCEEDED(::XAudio2Create(&_xaudio2)), "Failed to create Audio System.");
}

AudioSystem::~AudioSystem() {

    for(auto& channel : _active_channels) {
        channel.reset();
    }
    _active_channels.clear();
    _active_channels.shrink_to_fit();

    for(auto& channel : _idle_channels) {
        channel.reset();
    }
    _idle_channels.clear();
    _idle_channels.shrink_to_fit();

    for(auto& sound : _sounds) {
        delete sound.second;
        sound.second = nullptr;
    }
    _sounds.clear();

    for(auto& wav : _wave_files) {
        delete wav.second;
        wav.second = nullptr;
    }
    _wave_files.clear();

    _master_voice->DestroyVoice();
    _master_voice = nullptr;

    _xaudio2->UnregisterForCallbacks(&_engine_callback);

    _xaudio2->Release();
    _xaudio2 = nullptr;

    ::CoUninitialize();
}

void AudioSystem::Initialize() {
#ifdef AUDIO_DEBUG
    XAUDIO2_DEBUG_CONFIGURATION config{};
    config.LogFileline = true;
    config.LogFunctionName = true;
    config.LogThreadID = true;
    config.LogTiming = true;
    config.BreakMask = XAUDIO2_LOG_WARNINGS;
    config.TraceMask = XAUDIO2_LOG_DETAIL | XAUDIO2_LOG_WARNINGS | XAUDIO2_LOG_FUNC_CALLS;
    _xaudio2->SetDebugConfiguration(&config);
#endif
    _xaudio2->CreateMasteringVoice(&_master_voice);
    _idle_channels.reserve(_max_channels);
    _active_channels.reserve(_max_channels);

    FileUtils::Wav::WavFormatChunk fmt{};
    fmt.formatId = 1;
    fmt.channelCount = 1;
    fmt.samplesPerSecond = 44100;
    fmt.bytesPerSecond = 88200;
    fmt.dataBlockSize = 2;
    fmt.bitsPerSample = 16;
    SetFormat(fmt);

    SetEngineCallback(&_engine_callback);
}

void AudioSystem::SetEngineCallback(EngineCallback* callback) {
    if(&_engine_callback == callback) {
        return;
    }
    _xaudio2->UnregisterForCallbacks(&_engine_callback);
    _engine_callback = *callback;
    _xaudio2->RegisterForCallbacks(&_engine_callback);
}

const WAVEFORMATEXTENSIBLE& AudioSystem::GetFormat() const {
    return _audio_format_ex;
}

FileUtils::Wav::WavFormatChunk AudioSystem::GetLoadedWavFileFormat() const {
    FileUtils::Wav::WavFormatChunk fmt{};
    if(_wave_files.empty()) {
        return fmt;
    }
    return _wave_files.begin()->second->GetFormatChunk();
}

void AudioSystem::BeginFrame() {
    /* DO NOTHING */
}

void AudioSystem::Update(float /*deltaSeconds*/) {
    /* DO NOTHING */
}

void AudioSystem::Render() const {
    /* DO NOTHING */
}

void AudioSystem::EndFrame() {
    _idle_channels.erase(std::remove_if(std::begin(_idle_channels), std::end(_idle_channels), [](const std::unique_ptr<Channel>& c) { return c == nullptr; }), std::end(_idle_channels));
}

bool AudioSystem::ProcessSystemMessage(const EngineMessage& msg) {
    WPARAM wp = msg.wparam;
    LPARAM lp = msg.lparam;
    switch(msg.wmMessageCode) {
        case WindowsSystemMessage::KEYBOARD_KEYDOWN:
        {
            unsigned char key = static_cast<unsigned char>(wp);
            uint32_t lpBits = lp;
            constexpr uint32_t extended_key_mask = 0b0000'0001'0000'0000'0000'0000'0000'0000; //0x01000000;
            bool is_extended_key = (lpBits & extended_key_mask) != 0;
            auto my_key = InputSystem::ConvertWinVKToKeyCode(key);
            if(is_extended_key) {
                switch(my_key) {
                    case KeyCode::VOLUME_MUTE:
                    {
                        DebuggerPrintf("Audio System detected MUTE.\n");
                        return false;
                    }
                    case KeyCode::VOLUME_UP:
                    {
                        DebuggerPrintf("Audio System detected VOLUME UP.\n");
                        return false;
                    }
                    case KeyCode::VOLUME_DOWN:
                    {
                        DebuggerPrintf("Audio System detected VOLUME DOWN.\n");
                        return false;
                    }
                    default:
                    {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }
        default:
        {
            return false;
        }
    }
}

void AudioSystem::SetFormat(const WAVEFORMATEXTENSIBLE& format) {
    _audio_format_ex = format;
}

void AudioSystem::SetFormat(const FileUtils::Wav::WavFormatChunk& format) {
    auto fmt_buffer = reinterpret_cast<const unsigned char*>(&format);
    std::memcpy(&_audio_format_ex, fmt_buffer, sizeof(_audio_format_ex));
}

void AudioSystem::RegisterWavFilesFromFolder(const std::string& folderpath, bool recursive /*= false*/) {
    namespace FS = std::filesystem;
    RegisterWavFilesFromFolder(FS::path{ folderpath.begin(), folderpath.end() }, recursive);
}

void AudioSystem::RegisterWavFilesFromFolder(const std::filesystem::path& folderpath, bool recursive /*= false*/) {
    namespace FS = std::filesystem;
    bool is_folder = FS::is_directory(folderpath);
    if(!is_folder) {
        return;
    }
    auto cb =
    [this](const std::filesystem::path& p) {
        this->RegisterWavFile(p);
    };
    FileUtils::IterateFileInFolders(folderpath, ".wav", cb, recursive);
}

void AudioSystem::DeactivateChannel(Channel& channel) {
    auto found_iter = std::find_if(std::begin(_active_channels), std::end(_active_channels),
                                   [&channel](const std::unique_ptr<Channel>& c) { return c.get() == &channel; });
    _idle_channels.push_back(std::move(*found_iter));
    _active_channels.erase(found_iter);
}

void AudioSystem::Play(Sound& snd) {
    if(_idle_channels.size() == _max_channels) {
        return;
    }
    if(_active_channels.size() == _max_channels) {
        return;
    }
    if(_idle_channels.size() < _max_channels) {
        _idle_channels.push_back(std::make_unique<Channel>(*this));
    }
    _active_channels.push_back(std::move(_idle_channels.back()));
    _active_channels.back()->Play(snd);
}

void AudioSystem::Play(const std::string& filepath) {
    namespace FS = std::filesystem;
    FS::path p(filepath);
    auto found_iter = _sounds.find(filepath);
    Sound* snd = nullptr;
    if(found_iter == _sounds.end()) {
        snd = new Sound(p.string());
        _sounds.insert_or_assign(p.string(), snd);
    } else {
        snd = (*found_iter).second;
    }
    Play(*snd);
}

void AudioSystem::RegisterWavFile(const std::string& filepath) {
    namespace FS = std::filesystem;
    return RegisterWavFile(FS::path{ filepath });
}

void AudioSystem::RegisterWavFile(const std::filesystem::path& filepath) {
    auto found_iter = _wave_files.find(filepath.string());
    if(found_iter != _wave_files.end()) {
        return;
    }

    auto wav = new FileUtils::Wav;
    auto wav_result = wav->Load(filepath.string());
    if(wav_result == FileUtils::Wav::WAV_SUCCESS) {
        _wave_files.insert_or_assign(filepath.string(), wav);
        return;
    }

    delete wav;
    wav = nullptr;
    switch(wav_result) {
        case FileUtils::Wav::WAV_ERROR_NOT_A_WAV:
        {
            std::string e = filepath.string() + " is not a .wav file.\n";
            DebuggerPrintf(e.c_str());
            break;
        }
        case FileUtils::Wav::WAV_ERROR_BAD_FILE:
        {
            std::string e = filepath.string() + " is improperly formatted.\n";
            DebuggerPrintf(e.c_str());
            break;
        }
        default:
        {
            std::string e = "Unknown error attempting to load " + filepath.string() + "\n";
            DebuggerPrintf(e.c_str());
            break;
        }
    }
}

void STDMETHODCALLTYPE AudioSystem::Channel::VoiceCallback::OnBufferEnd(void* pBufferContext) {
    Channel& channel = *reinterpret_cast<Channel*>(pBufferContext);
    channel._sound->RemoveChannel(&channel);
    channel._sound = nullptr;
    channel._audio_system->DeactivateChannel(channel);
}

AudioSystem::Channel::Channel(AudioSystem& audioSystem) : _audio_system(&audioSystem) {
    static VoiceCallback vcb;
    _buffer.pContext = this;
    auto fmt = reinterpret_cast<const WAVEFORMATEX*>(&(_audio_system->GetFormat()));
    _audio_system->_xaudio2->CreateSourceVoice(&_voice, fmt, 0, 2.0f, &vcb);
}

AudioSystem::Channel::~Channel() {
    if(_voice) {
        Stop();
        _voice->DestroyVoice();
        _voice = nullptr;
    }
}

void AudioSystem::Channel::Play(Sound& snd) {
    snd.AddChannel(this);
    _sound = &snd;
    _buffer.pAudioData = snd.GetWav()->GetDataBuffer();
    _buffer.AudioBytes = snd.GetWav()->GetDataBufferSize();
    _voice->SubmitSourceBuffer(&_buffer, nullptr);
    _voice->Start();
}

void AudioSystem::Channel::Stop() {
    if(_voice && _sound) {
        _voice->Stop();
        _voice->FlushSourceBuffers();
    }
}

void AudioSystem::Channel::SetVolume(float newVolume) {
    _voice->SetVolume(newVolume);
}

std::size_t AudioSystem::Sound::_id = 0;

AudioSystem::Sound::Sound(const std::string& filepath) {
    auto wav = new FileUtils::Wav;
    auto result = wav->Load(filepath);
    if(result != FileUtils::Wav::WAV_SUCCESS) {
        delete wav;
        wav = nullptr;
        std::string error = "Error loading " + filepath;
        ERROR_AND_DIE(error.c_str());
    }
    _my_id = _id++;
    _wave_file = wav;
}

AudioSystem::Sound::~Sound() {
    _channels.clear();
    _channels.shrink_to_fit();
    delete _wave_file;
    _wave_file = nullptr;
}

void AudioSystem::Sound::AddChannel(Channel* channel) {
    _channels.push_back(channel);
}

void AudioSystem::Sound::RemoveChannel(Channel* channel) {
    _channels.erase(std::remove_if(std::begin(_channels), std::end(_channels),
                                   [channel](Channel* c)->bool { return c == channel; })
                    , std::end(_channels));
}

const std::size_t AudioSystem::Sound::GetId() const {
    return _my_id;
}

const std::size_t AudioSystem::Sound::GetCount() const {
    return _id;
}

const FileUtils::Wav* const AudioSystem::Sound::GetWav() const {
    return _wave_file;
}
