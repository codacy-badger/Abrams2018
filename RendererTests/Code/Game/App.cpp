#include "Game/App.hpp"

#include "Engine/Core/EngineSubsystem.hpp"

#include "Engine/Core/TimeUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/DirectX/DX11Renderer.hpp"
#include "Engine/RHI/RHIOutput.hpp"

#include "Engine/Renderer/Window.hpp"

#include "Game/GameCommon.hpp"
#include "Game/GameConfig.hpp"

bool CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    EngineMessage msg;
    std::memset(&msg, 0, sizeof(msg));
    msg.hWnd = hwnd;
    msg.wmMessageCode = EngineSubsystem::GetWindowsSystemMessageFromUintMessage(uMsg);
    msg.wparam = wParam;
    msg.lparam = lParam;

    if(g_theInput && g_theInput->ProcessSystemMessage(msg)) {
        return true;
    } else {

        switch(uMsg) {
            case WM_CLOSE:
            case WM_QUIT:
            case WM_DESTROY:
            {
                g_theApp->SetIsQuitting(true);
                return true;
            }
            default:
            {
                return false;
            }
        }
    }
}

App::App() {
    g_theRenderer = new DX11Renderer(static_cast<unsigned int>(GRAPHICS_OPTION_WINDOW_WIDTH), static_cast<unsigned int>(GRAPHICS_OPTION_WINDOW_HEIGHT));
    g_theRenderer->SetVSync(GRAPHICS_OPTION_VSYNC);
    g_theRenderer->Initialize();

    if(g_theRenderer->GetOutput()) {
        Window* window = g_theRenderer->GetOutput()->GetWindow();
        if(window) {
            window->custom_message_handler = WindowProc;
            window->SetTitle(L"Test Title");
            bool is_fullscreen = false;
            if(is_fullscreen) {
                window->SetDisplayMode(RHIOutputMode::FULLSCREEN_WINDOW);
            }
        }
    }

    g_theInput = new InputSystem();
    g_theGame = new Game();

}

App::~App() {
    delete g_theGame;
    g_theGame = nullptr;

    delete g_theInput;
    g_theInput = nullptr;
    
    delete g_theRenderer;
    g_theRenderer = nullptr;
}

bool App::IsQuitting() const {
    return _isQuitting;
}

void App::SetIsQuitting(bool quit) {
    _isQuitting = quit;
}

void App::Initialize() {
    /* DO NOTHING */
}

void App::RunFrame() {
    using namespace std::chrono;

    BeginFrame();

    static float previousFrameTime = TimeUtils::GetCurrentTimeElapsed<duration<float>>();
    float currentFrameTime = TimeUtils::GetCurrentTimeElapsed<duration<float>>();
    float deltaSeconds = currentFrameTime - previousFrameTime;
    previousFrameTime = currentFrameTime;

    deltaSeconds = MathUtils::Clamp(deltaSeconds, 0.0f, 0.16f);

    Update(deltaSeconds);
    Render();
    EndFrame();
}

void App::BeginFrame() {
    g_theInput->BeginFrame();
    g_theGame->BeginFrame();
    g_theRenderer->BeginFrame();
}

void App::Update(float deltaSeconds) {
    g_theInput->Update(deltaSeconds);
    g_theGame->Update(deltaSeconds);
    g_theRenderer->Update(deltaSeconds);
}

void App::Render() const {
    g_theGame->Render();
    g_theInput->Render();
    g_theRenderer->Render();
}

void App::EndFrame() {
    g_theGame->EndFrame();
    g_theInput->EndFrame();
    g_theRenderer->EndFrame();
}
