#pragma once

#include "Engine/Core/Event.hpp"
#include "Engine/UI/Types.hpp"

#include <functional>
#include <string>

class Camera2D;

namespace UI {
    class Canvas;
    class Panel;
    class Label;
}

class Game {
public:
    Game();
    ~Game();

    void Initialize();
    void InitializeData();
    void InitializeUI();

    void BeginFrame();
    void Update(float deltaSeconds);
    void Render() const;
    void SetPivotPositionText() const;
    void EndFrame();

protected:
private:
    void DoExport();
    void GenerateImageData(void* data);

    float _cameraSpeed = 10.0f;
    Camera2D* _camera2 = nullptr;
    UI::Canvas* _cnvFullscreen = nullptr;
    UI::Label* _lblPivotPosition = nullptr;
    UI::Label* _lblPivotName = nullptr;
    UI::PivotPosition _pivot_position = UI::PivotPosition::First_;
    bool _debug = true;

};