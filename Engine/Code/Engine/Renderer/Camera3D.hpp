#pragma once

#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"

class Camera3D {
public:
    Camera3D() = default;
    ~Camera3D() = default;

    void SetupView(float fovVerticalDegrees, float aspectRatio = MathUtils::M_16_BY_9_RATIO, float nearDistance = 0.01f, float farDistance = 1.0f, const Vector3& worldUp = Vector3::Y_AXIS);
    void Update(float deltaSeconds);

    const Vector3& GetPosition() const;
    void SetPosition(const Vector3& newPosition);
    void SetPosition(float x, float y, float z);
    void SetPosition(const Vector2& newPosition);
    void SetPosition(float x, float y);
    void Translate(const Vector3& displacement);
    void Translate(float x, float y, float z);
    void Translate(const Vector2& displacement);
    void Translate(float x, float y);

    float GetFovYDegrees() const;
    float CalcFovXDegrees() const;
    float CalcNearViewWidth() const;
    float CalcNearViewHeight() const;
    float CalcFarViewWidth() const;
    float CalcFarViewHeight() const;

    float GetAspectRatio() const;
    float GetInverseAspectRatio() const;
    float GetNearDistance() const;
    float GetFarDistance() const;

    const Matrix4& GetViewMatrix() const;
    const Matrix4& GetProjectionMatrix() const;
    const Matrix4& GetViewProjectionMatrix() const;

    float trauma = 0.0f;
    float trauma_recovery_rate = 1.0f;
protected:
private:
    void CalcViewMatrix();
    void CalcViewProjectionMatrix();
    void CalcProjectionMatrix();

    float aspect_ratio = MathUtils::M_16_BY_9_RATIO;
    float fov_vertical_degrees = 60.0f;
    float near_view_height = 1600.0f;
    float far_view_height = 1600.0f;
    float near_distance = 0.01f;
    float far_distance = 1.0f;
    Vector3 position = Vector3::ZERO;
    Vector3 world_up = Vector3::Y_AXIS;
    Vector3 right = Vector3::X_AXIS;
    Vector3 up = Vector3::Y_AXIS;
    Vector3 look = Vector3::Z_AXIS;
    Matrix4 view_matrix = Matrix4::GetIdentity();
    Matrix4 projection_matrix = Matrix4::GetIdentity();
    Matrix4 view_projection_matrix = Matrix4::GetIdentity();

};