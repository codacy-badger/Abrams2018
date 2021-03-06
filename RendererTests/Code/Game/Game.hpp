#pragma once

#include "Engine/Core/Event.hpp"
#include "Engine/Core/Obj.hpp"
#include "Engine/Core/TimeUtils.hpp"

#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB2.hpp"

#include "Engine/Renderer/Camera2D.hpp"
#include "Engine/Renderer/Camera3D.hpp"

#include "Engine/UI/Types.hpp"

#include <functional>
#include <string>

class AnimatedSprite;
class ConstantBuffer;
class Texture;

namespace UI {
class Canvas;
class Panel;
class Label;
class Sprite;
}

struct mandelbrot_t {
    Vector4 escapeColor{};
    Vector4 convergeColor{};
    Vector2 mouseDelta{};
    Vector2 padding{};
    unsigned int maxIterations{};
    float power{};
    float zoom{};
    unsigned int use_escape_color{};
};

class Game {
public:
    Game() = default;
    ~Game();

    void Initialize();
    void InitializeData();
    void InitializeUI();

    void BeginFrame();
    void Update(TimeUtils::FPSeconds deltaSeconds);

    void ShowFileDialog();

    void Render() const;
    void RenderStuff() const;
    void EndFrame();

protected:
private:
    void UpdateCameraFromKeyboard(TimeUtils::FPSeconds deltaSeconds);
    void UpdateCameraFromMouse(TimeUtils::FPSeconds deltaSeconds);
    void DrawWorldGrid() const;
    void DrawAxes() const;
    void DrawCube() const;
    void DrawPointCloud() const;

    AABB2 _aabb2{};
    OBB2 _obb1{};
    OBB2 _obb2{};
    const float _default_fovV = 45.0f;
    float _fovV = _default_fovV;
    const float _cameraSpeed = 10.0f;
    const float _camera_move_speed_multiplier = 10.0f;
    mutable Camera2D _camera2{};
    mutable Camera3D _camera3{};
    bool _lock_mouse_to_center{ false };
    Rgba clear_color = Rgba::Grey;
    Rgba img_tint_color = Rgba::White;
    Rgba img_border_color = Rgba::NoAlpha;
    bool _debug = true;
    bool _slowmo = false;    
    std::string _img_path{ "Data/Images/Test_StbiAndDirectX.png" };
};