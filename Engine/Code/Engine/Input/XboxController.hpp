#pragma once

#include "Engine/Core/Win.hpp"

#include <XInput.h>
#pragma comment(lib, "Xinput.lib")

#include <bitset>

#include "Engine/Math/Vector2.hpp"

class XboxController {
public:
    enum class Button : unsigned char {
        FIRST_
        , A = FIRST_
        , B
        , X
        , Y
        , START
        , BACK
        , UP
        , DOWN
        , LEFT
        , RIGHT
        , LEFTTHUMB
        , RIGHTTHUMB
        , RIGHTBUMPER
        , LEFTBUMPER
        , LAST_
        , MAX = LAST_
    };

    enum class Motor {
        LEFT
        ,RIGHT
        ,BOTH
    };
    XboxController() = default;
    ~XboxController() = default;

    bool IsButtonUp(const Button& button) const;
    bool WasButtonJustPressed(const Button& button) const;
    bool IsButtonDown(const Button& button) const;
    bool WasButtonJustReleased(const Button& button) const;

    bool WasJustConnected() const;
    bool IsConnected() const;
    bool WasJustDisconnected() const;
    bool IsDisconnected() const;

    bool WasAnyButtonJustPressed() const;
    bool WasAnyButtonJustReleased() const;
    bool IsAnyButtonDown() const;

    void Update(int controller_number);

    void StopLeftMotor();
    void StopRightMotor();
    void StopMotors();

    void SetLeftMotorSpeed(unsigned short speed);
    void SetRightMotorSpeed(unsigned short speed);
    void SetBothMotorSpeed(unsigned short speed);

    void SetLeftMotorSpeedToMax();
    void SetRightMotorSpeedToMax();
    void SetBothMotorSpeedToMax();

    void SetLeftMotorSpeedAsPercent(float speed);
    void SetRightMotorSpeedAsPercent(float speed);
    void SetBothMotorSpeedAsPercent(float speed);

protected:
private:
    void UpdateState();
    void SetMotorSpeed(int controller_number, const Motor& motor, unsigned short value);

    bool DidMotorStateChange() const;

    enum class ActiveState {
        CONNECTED
        , MOTOR
        , MAX
    };

    Vector2 _leftThumbDistance = Vector2::ZERO;
    Vector2 _rightThumbDistance = Vector2::ZERO;
    Vector2 _triggerDistances = Vector2::ZERO;
    unsigned short _leftMotorState = 0;
    unsigned short _rightMotorState = 0;
    unsigned short _previousRawInput = 0;
    unsigned short _currentRawInput = 0;
    unsigned long _previousPacketNumber = 0;
    unsigned long _currentPacketNumber = 0;
    std::bitset<(std::size_t)ActiveState::MAX> _previousActiveState{};
    std::bitset<(std::size_t)ActiveState::MAX> _currentActiveState{};
    std::bitset<(std::size_t)Button::MAX> _previousButtonState{};
    std::bitset<(std::size_t)Button::MAX> _currentButtonState{};
};