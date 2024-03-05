#include "Camera.h"

using namespace engine::graphics;

Camera::Camera()
{
    perspective_params_.field_of_view = 3.14f / 2;
    perspective_params_.aspect_ratio = 1;
    perspective_params_.near_z = 0.01f;
    perspective_params_.far_z = 1000;
}

void Camera::Compose()
{
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void Camera::Compose(transform::TransformComponent* transform)
{
    transform_ = transform;
}

void Camera::UpdateViewMatrix()
{
    const float4x4 rotation = float4x4::CreateFromYawPitchRoll(transform_->rotation());
    const float3 target = float3::Transform(float3::Forward, rotation) + transform_->position();
    const float3 up_direction = float3::Transform(float3::Up, rotation);
    view_matrix_ = float4x4::CreateLookAt(transform_->position(), target, up_direction);
}

void Camera::UpdateProjectionMatrix()
{
    projection_matrix_ = float4x4::CreatePerspectiveFieldOfView(
        perspective_params_.field_of_view, perspective_params_.aspect_ratio,
        perspective_params_.near_z, perspective_params_.far_z);
}

CameraPerspectiveParams& Camera::perspective_params()
{
    return perspective_params_;
}

float4x4& Camera::view_matrix()
{
    return view_matrix_;
}

float4x4& Camera::projection_matrix()
{
    return projection_matrix_;
}
