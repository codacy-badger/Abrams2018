#pragma once

#include "Engine/Core/Riff.hpp"
#include "Engine/Core/StringUtils.hpp"

#include <string>

class Wav {
public:
    static struct ID {
        static constexpr const uint32_t WAVE = StringUtils::FourCC("WAVE");
        static constexpr const uint32_t FMT = StringUtils::FourCC("fmt ");
        static constexpr const uint32_t FACT = StringUtils::FourCC("fact");
        static constexpr const uint32_t DATA = StringUtils::FourCC("data");
    }ID;

    Wav() = default;
    ~Wav();

    struct WavHeader {
        char fourcc[4]{};
        uint32_t length{};
    };

    struct WavFormatChunk {
        uint16_t formatId{};
        uint16_t channelCount{};
        uint32_t samplesPerSecond{};
        uint32_t bytesPerSecond{}; //samplesPerSecond * channelCount * bitsPerSample / 8
        uint16_t dataBlockSize{};  //channelCount * bitsPerSample / 8
        uint16_t bitsPerSample{};
        uint16_t extensionSize{};
        uint16_t validBitsPerSample{};
        uint32_t speakerPositionMask{};
        uint32_t subFormat[4]{};
    };

    struct WavFactChunk {
        uint32_t samplesPerChannel{};
    };

    struct WavDataChunk {
        uint32_t length{};
        uint8_t* data;
        ~WavDataChunk() {
            delete[] data;
            data = nullptr;
        }
    };

    static constexpr const unsigned int WAV_SUCCESS = 0;
    static constexpr const unsigned int WAV_ERROR_NOT_A_WAV = 1;
    static constexpr const unsigned int WAV_ERROR_BAD_FILE = 2;

    unsigned int Load(const std::string& filepath);
    unsigned char* GetFormatAsBuffer();
    unsigned char* GetDataBuffer();
    const WavFormatChunk& GetFormatChunk() const;
    const WavDataChunk& GetDataChunk() const;
    uint32_t GetDataBufferSize() const;
protected:
private:
    bool IsRecognizableChunk(const char* id) const;

    WavFormatChunk _fmt{};
    WavFactChunk _fact{};
    WavDataChunk _data{};
    Riff::RiffChunk _list{};
    Riff* _riff_data = nullptr;

};