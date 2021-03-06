#include "Game/Game.hpp"

#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/Core/KerningFont.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Core/StringUtils.hpp"

#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/Disc2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/LineSegment3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Noise.hpp"
#include "Engine/Math/Sphere3.hpp"

#include "Engine/Renderer/AnimatedSprite.hpp"
#include "Engine/Renderer/ArrayBuffer.hpp"
#include "Engine/Renderer/Buffer.hpp"
#include "Engine/Renderer/Camera2D.hpp"
#include "Engine/Renderer/Camera3D.hpp"
#include "Engine/Renderer/ConstantBuffer.hpp"
#include "Engine/Renderer/Material.hpp"
#include "Engine/Renderer/RasterState.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/Texture1D.hpp"
#include "Engine/Renderer/Texture2D.hpp"
#include "Engine/Renderer/Texture3D.hpp"
#include "Engine/Renderer/Window.hpp"

#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/RHI/RHIOutput.hpp"

#include "Engine/Profiling/ProfileLogScope.hpp"

#include "Engine/UI/UI.hpp"

#include "Thirdparty/Imgui/imgui.h"

#include "Game/GameCommon.hpp"
#include "Game/GameConfig.hpp"

#include <algorithm>
#include <functional>
#include <numeric>
#include <sstream>

Game::~Game() {
    /* DO NOTHING */
}

void Game::Initialize() {
    InitializeData();
    InitializeUI();
}

void Game::InitializeData() {
    g_theRenderer->RegisterTexturesFromFolder(std::string{ "Data/Images" }, true);
    g_theRenderer->RegisterShadersFromFolder(std::string{ "Data/Shaders" });
    g_theRenderer->RegisterMaterialsFromFolder(std::string{ "Data/Materials" });
    g_theRenderer->RegisterFontsFromFolder(std::string{ "Data/Fonts" });

    _camera3.SetPosition(Vector3(230.0f, 270.0f, -180.0f));
    _camera3.SetEulerAngles(Vector3(-45.0f, 0.0f, 0.0f));

    _obb1.half_extents = Vector2::ONE * 25.0f;
    _obb2.half_extents = Vector2::ONE * 25.0f;
    _aabb2.AddPaddingToSides(25.0f, 25.0f);
}

void Game::InitializeUI() {
    /* DO NOTHING */
}

void Game::BeginFrame() {
    /* DO NOTHING */
}

void Game::Update(TimeUtils::FPSeconds deltaSeconds) {
    if(g_theInput->WasKeyJustPressed(KeyCode::Esc)) {
        g_theApp->SetIsQuitting(true);
        return;
    }

    _slowmo = false;
    if(g_theInput->IsKeyDown(KeyCode::RCtrl)) {
        _slowmo = true;
    }

    g_theRenderer->UpdateGameTime(_slowmo ? deltaSeconds * 0.10f : deltaSeconds);
    UpdateCameraFromKeyboard(g_theRenderer->GetGameFrameTime());
    UpdateCameraFromMouse(g_theRenderer->GetGameFrameTime());

    if(g_theInput->WasKeyJustPressed(KeyCode::H)) {
        g_theInput->ToggleMouseCursorVisibility();
    }

    if(g_theInput->WasKeyJustPressed(KeyCode::F1)) {
        _debug = !_debug;
    }

    if(g_theInput->WasKeyJustPressed(KeyCode::F4)) {
        MathUtils::SetRandomEngineSeed(1729);
    }

    _camera2.Update(g_theRenderer->GetGameFrameTime());
    _camera3.Update(g_theRenderer->GetGameFrameTime());

    ImGui::ShowDemoWindow();

    {
        static float f = 0.0f;
        static float a_d = 0.0f;
        static float a_r = 0.0f;
        static int counter = 0;
        static int i = 0;

        ImGui::Begin("Hello, world!");

        ImGui::Checkbox("Lock Mouse to center", &_lock_mouse_to_center);
        ImGui::Checkbox("Debug Mode", &_debug);
        ImGui::ColorEdit3("clear color", clear_color);
        ImGui::Text("Image: %s", _img_path.c_str());
        ImGui::Image(g_theRenderer->GetTexture(_img_path), Vector2::ONE * 250.0f, Vector2::ZERO, Vector2::ONE, img_tint_color, img_border_color);
        if(ImGui::Button("New Image")) {
            ShowFileDialog();
        }
        ImGui::ColorEdit4("tint", img_tint_color);
        ImGui::ColorEdit4("border", img_border_color);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();
    }

}


void Game::ShowFileDialog() {
    ImGui::OpenPopup("SI");
    if(ImGui::BeginPopupModal("SI", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Select an image to show in the box...");
        const auto& paths = FileUtils::GetAllPathsInFolders("Data/Images/", Image::GetSupportedExtensionsList());
        for(const auto& t : paths) {
            if(ImGui::Button(t.string().c_str())) {
                _img_path = t.string();
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }
}

void Game::UpdateCameraFromKeyboard(TimeUtils::FPSeconds deltaSeconds) {
    float camera_move_speed = 0.0f;
    {
        bool is_fast = false;
        if(g_theInput->IsKeyDown(KeyCode::LShift)) {
            is_fast = true;
        }
        camera_move_speed = _cameraSpeed * deltaSeconds.count() * (is_fast ? _camera_move_speed_multiplier : 1.0f);
    }

    auto forward = _camera3.GetForward();
    if(g_theInput->IsKeyDown(KeyCode::W)) {
        _camera3.Translate(forward * camera_move_speed);
    } else if(g_theInput->IsKeyDown(KeyCode::S)) {
        _camera3.Translate(-forward * camera_move_speed);
    }

    auto right = _camera3.GetRight();
    if(g_theInput->IsKeyDown(KeyCode::A)) {
        _camera3.Translate(-right * camera_move_speed);
    } else if(g_theInput->IsKeyDown(KeyCode::D)) {
        _camera3.Translate(right * camera_move_speed);
    }

    if(g_theInput->IsKeyDown(KeyCode::Q)) {
        Vector3 angles = Vector3{ _camera3.GetPitchDegrees(), _camera3.GetYawDegrees(), _camera3.GetRollDegrees() - 1.0f };
        _camera3.SetEulerAnglesDegrees(angles);
    } else if(g_theInput->IsKeyDown(KeyCode::E)) {
        Vector3 angles = Vector3{ _camera3.GetPitchDegrees(), _camera3.GetYawDegrees(), _camera3.GetRollDegrees() + 1.0f };
        _camera3.SetEulerAnglesDegrees(angles);
    }

    auto up = _camera3.GetUp();
    if(g_theInput->IsKeyDown(KeyCode::Space)) {
        _camera3.Translate(up * camera_move_speed);
    } else if(g_theInput->IsKeyDown(KeyCode::LCtrl)) {
        _camera3.Translate(-up * camera_move_speed);
    }

    if(g_theInput->WasKeyJustPressed(KeyCode::R)) {
        _camera3.SetPosition(Vector3::ZERO);
        _camera3.SetEulerAngles(Vector3::ZERO);
        _camera2.SetPosition(Vector2::ZERO);
        _camera2.SetOrientationDegrees(0.0f);
        _fovV = _default_fovV;
    }

    if(g_theInput->WasKeyJustPressed(KeyCode::L)) {
        _lock_mouse_to_center = !_lock_mouse_to_center;
    }
}

void Game::UpdateCameraFromMouse(TimeUtils::FPSeconds deltaSeconds) {
    if(!g_theApp->HasFocus() || g_theConsole->IsOpen()) {
        return;
    }
    float camera_move_speed = 0.0f;
    {
        bool is_fast = false;
        if(g_theInput->IsKeyDown(KeyCode::Shift)) {
            is_fast = true;
        }
        camera_move_speed = _cameraSpeed * deltaSeconds.count() * (is_fast ? _camera_move_speed_multiplier : 1.0f);
    }
    const auto& window = *(g_theRenderer->GetOutput()->GetWindow());
    auto mouse_pos = g_theInput->GetCursorWindowPosition(window);
    if(_lock_mouse_to_center) {
        g_theInput->SetCursorToWindowCenter(window);
    }
    auto mouse_delta_pos = (mouse_pos - g_theInput->GetCursorWindowPosition(window));
    auto moved_x = mouse_delta_pos.x;
    auto moved_y = mouse_delta_pos.y;
    Vector3 angles = Vector3{ _camera3.GetPitchDegrees() - moved_y, _camera3.GetYawDegrees() - moved_x, _camera3.GetRollDegrees() };
    _camera3.SetEulerAnglesDegrees(angles);
    if(g_theInput->WasKeyJustPressed(KeyCode::MButton)) {
        _fovV = _default_fovV;
    }
    if(g_theInput->WasMouseWheelJustScrolledUp()) {
        _fovV *= 0.90f;
        if(_fovV < 0.0001f) {
            _fovV = 0.0001f;
        }
    }
    if(g_theInput->WasMouseWheelJustScrolledDown()) {
        _fovV *= 1.10f;
        if(_fovV > 179.90f) {
            _fovV = 179.90f;
        }
    }
    if(g_theInput->WasMouseWheelJustScrolledLeft()) {
        _camera3.Translate(-_camera3.GetRight() * camera_move_speed);
    }
    if(g_theInput->WasMouseWheelJustScrolledRight()) {
        _camera3.Translate(_camera3.GetRight() * camera_move_speed);
    }
}

void Game::Render() const {

    g_theRenderer->SetRenderTarget();
    g_theRenderer->ClearColor(clear_color);
    g_theRenderer->ClearDepthStencilBuffer();

    ViewportDesc view_desc{};
    view_desc.width = GRAPHICS_OPTION_WINDOW_WIDTH;
    view_desc.height = GRAPHICS_OPTION_WINDOW_HEIGHT;
    g_theRenderer->SetViewport(view_desc);
    
    g_theRenderer->SetModelMatrix(Matrix4::GetIdentity());
    g_theRenderer->SetViewMatrix(Matrix4::GetIdentity());
    g_theRenderer->SetProjectionMatrix(Matrix4::GetIdentity());

    _camera3.SetupView(_fovV, MathUtils::M_16_BY_9_RATIO, 0.01f, 1000.0f);
    g_theRenderer->SetCamera(_camera3);

    g_theRenderer->SetAmbientLight(Rgba::Grey, 1.0f);
    g_theRenderer->SetLightingEyePosition(_camera3.GetPosition());

    DirectionalLightDesc dl_desc{};
    dl_desc.color = Rgba::White;
    dl_desc.direction = (Vector3(-1.0f, -1.0f, 0.0f)).GetNormalize();
    dl_desc.intensity = 0.0f;
    g_theRenderer->SetDirectionalLight(0, dl_desc);

    SpotLightDesc sp_desc{};
    sp_desc.position = _camera3.GetPosition();
    auto dir = (_camera3.GetForward() + -_camera3.GetUp()).GetNormalize();
    sp_desc.direction = dir;
    sp_desc.intensity = 1.0f;
    sp_desc.inner_outer_anglesDegrees = Vector2(80.0f, 160.0f);
    g_theRenderer->SetSpotlight(1, sp_desc);

    RenderStuff();

    const auto& window_dimensions = g_theRenderer->GetOutput()->GetDimensions();
    auto window_width = static_cast<float>(window_dimensions.x);
    auto window_height = static_cast<float>(window_dimensions.y);
    float view_half_width = window_width * 0.50f;
    float view_half_height = window_height * 0.50f;

    Vector2 view_leftBottom = Vector2(-view_half_width, view_half_height);
    Vector2 view_rightTop = Vector2(view_half_width, -view_half_height);
    Vector2 view_topLeft = Vector2(-view_half_width, -view_half_height);
    Vector2 view_bottomRight = Vector2(view_half_width, view_half_height);
    Vector2 view_bottom = Vector2(0.0f, view_half_height);
    Vector2 view_nearFar = Vector2(0.0f, 1.0f);

    _camera2.SetupView(view_leftBottom, view_rightTop, view_nearFar, MathUtils::M_16_BY_9_RATIO);
    g_theRenderer->SetCamera(_camera2);

    //g_theRenderer->SetMaterial(g_theRenderer->GetMaterial("__2D"));
    //g_theRenderer->SetModelMatrix(Matrix4::GetIdentity());
    //g_theRenderer->DrawOBB2(_obb1, Rgba::Blue, Rgba::NoAlpha, Vector2::ONE * 0.001f);
    //g_theRenderer->DrawOBB2(_obb2, Rgba::Green, Rgba::NoAlpha, Vector2::ONE * 0.001f);
    //g_theRenderer->DrawAABB2(_aabb2, Rgba::Green, Rgba::NoAlpha);

    //_canvas->Render(g_theRenderer);
    //if(_debug) {
    //    _canvas->DebugRender(g_theRenderer);
    //}
}

void Game::RenderStuff() const {
    //g_theRenderer->SetModelMatrix(Matrix4::CreateTranslationMatrix(_pos.back().position));
    //g_theRenderer->SetMaterial(g_theRenderer->GetMaterial("__2D"));
    //DrawPointCloud();

    //g_theRenderer->SetModelMatrix(Matrix4::CreateTranslationMatrix(Vector3::X_AXIS));
    //g_theRenderer->SetMaterial(g_theRenderer->GetMaterial("Stone"));
    //DrawCube();

    //g_theRenderer->SetMaterial(g_theRenderer->GetMaterial("Test"));
    //DrawCube();
    //g_theRenderer->SetModelMatrix(Matrix4::Create3DXRotationDegreesMatrix(180.0f));
    //TODO: Fix UVs upside-down
    //DrawCube();
    g_theRenderer->SetModelMatrix();
    g_theRenderer->DrawDebugSphere(Rgba::Red);
    DrawWorldGrid();
    DrawAxes();

}

void Game::EndFrame() {
    /* DO NOTHING */
}

void Game::DrawWorldGrid() const {
    if(!_debug) {
        return;
    }
    g_theRenderer->DrawWorldGridXZ();
}

void Game::DrawAxes() const {
    if(!_debug) {
        return;
    }
    g_theRenderer->DrawAxes();
}

void Game::DrawCube() const {
    static const auto aabb2 = AABB2::ZERO_TO_ONE;
    static const auto mins = aabb2.mins;
    static const auto maxs = aabb2.maxs;
    static const auto tex_left_bottom = Vector2{ mins.x, maxs.y };
    static const auto tex_left_top = Vector2{ mins.x, mins.y };
    static const auto tex_right_top = Vector2{ maxs.x, mins.y };
    static const auto tex_right_bottom = Vector2{ maxs.x, maxs.y };
    static std::vector<Vertex3D> vbo{
        //Bottom
        Vertex3D(Vector3(-0.5f, -0.5f,  0.5f), Rgba::White, tex_left_bottom, -Vector3::Y_AXIS),
        Vertex3D(Vector3(-0.5f, -0.5f, -0.5f), Rgba::White, tex_left_top, -Vector3::Y_AXIS),
        Vertex3D(Vector3(0.5f,  -0.5f, -0.5f), Rgba::White, tex_right_top, -Vector3::Y_AXIS),
        Vertex3D(Vector3(0.5f,  -0.5f,  0.5f), Rgba::White, tex_right_bottom, -Vector3::Y_AXIS),
        //Top
        Vertex3D(Vector3(-0.5f, 0.5f, -0.5f), Rgba::White, tex_left_bottom, Vector3::Y_AXIS),
        Vertex3D(Vector3(-0.5f, 0.5f, 0.5f),  Rgba::White,  tex_left_top, Vector3::Y_AXIS),
        Vertex3D(Vector3(0.5f,  0.5f, 0.5f),  Rgba::White,  tex_right_top, Vector3::Y_AXIS),
        Vertex3D(Vector3(0.5f,  0.5f, -0.5f), Rgba::White, tex_right_bottom, Vector3::Y_AXIS),
        //Left
        Vertex3D(Vector3(-0.5f, 0.5f, -0.5f),  Rgba::White,  tex_right_top, -Vector3::X_AXIS),
        Vertex3D(Vector3(-0.5f, -0.5f, -0.5f), Rgba::White, tex_right_bottom, -Vector3::X_AXIS),
        Vertex3D(Vector3(-0.5f, -0.5f, 0.5f),  Rgba::White,  tex_left_bottom, -Vector3::X_AXIS),
        Vertex3D(Vector3(-0.5f, 0.5f, 0.5f),   Rgba::White,   tex_left_top, -Vector3::X_AXIS),
        //Right
        Vertex3D(Vector3(0.5f, -0.5f, -0.5f), Rgba::White, tex_left_bottom, Vector3::X_AXIS),
        Vertex3D(Vector3(0.5f,  0.5f, -0.5f), Rgba::White, tex_left_top, Vector3::X_AXIS),
        Vertex3D(Vector3(0.5f,  0.5f,  0.5f), Rgba::White, tex_right_top, Vector3::X_AXIS),
        Vertex3D(Vector3(0.5f, -0.5f,  0.5f), Rgba::White, tex_right_bottom, Vector3::X_AXIS),
        //Front
        Vertex3D(Vector3(-0.5f, -0.5f, -0.5f), Rgba::White, tex_left_bottom, -Vector3::Z_AXIS),
        Vertex3D(Vector3(-0.5f,  0.5f, -0.5f), Rgba::White, tex_left_top, -Vector3::Z_AXIS),
        Vertex3D(Vector3(0.5f,   0.5f, -0.5f), Rgba::White, tex_right_top, -Vector3::Z_AXIS),
        Vertex3D(Vector3(0.5f,  -0.5f, -0.5f), Rgba::White, tex_right_bottom, -Vector3::Z_AXIS),
        //Back
        Vertex3D(Vector3(0.5f,  -0.5f, 0.5f), Rgba::White, tex_left_bottom, Vector3::Z_AXIS),
        Vertex3D(Vector3(0.5f,   0.5f, 0.5f), Rgba::White, tex_left_top, Vector3::Z_AXIS),
        Vertex3D(Vector3(-0.5f,  0.5f, 0.5f), Rgba::White, tex_right_top, Vector3::Z_AXIS),
        Vertex3D(Vector3(-0.5f, -0.5f, 0.5f), Rgba::White, tex_right_bottom, Vector3::Z_AXIS),
    };

    static std::vector<unsigned int> ibo{
         0,  1,  2,  0,  2,  3,
         4,  5,  6,  4,  6,  7,
         8,  9, 10,  8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23,
    };
    g_theRenderer->DrawIndexed(PrimitiveType::Triangles, vbo, ibo);
}

void Game::DrawPointCloud() const {
    static bool is_init = false;
    static std::vector<Vertex3D> vbo{};
    static std::vector<unsigned int> ibo{};
    if(!is_init) {
        is_init = true;
        static std::vector<Vector3> verts{};
        static std::size_t size = 1000000;
        verts.reserve(size);
        //static const AABB3 sphere{ -Vector3::ONE * 50.0f, Vector3::ONE * 50.0f };
        static const Sphere3 sphere{ Vector3::ZERO, 50.0f};
        //static const LineSegment3 sphere{ -Vector3::ONE * 50.0f, Vector3::ONE * 50.0f};
        //static const Disc2 sphere{ Vector2::ZERO, 50.0f };
        //static const AABB2 sphere{ -Vector2::ONE * 50.0f, Vector2::ONE * 50.0f };
        for(int i = 0; i < size; ++i) {
            verts.push_back(MathUtils::GetRandomPointInside(sphere));
        }
        vbo.resize(verts.size());
        for(std::size_t i = 0; i < vbo.size(); ++i) {
            vbo[i] = Vertex3D{verts[i], Rgba::White};
        }
        ibo.resize(vbo.size());
        std::iota(std::begin(ibo), std::end(ibo), 0);
    }
    g_theRenderer->DrawIndexed(PrimitiveType::Points, vbo, ibo);
}
