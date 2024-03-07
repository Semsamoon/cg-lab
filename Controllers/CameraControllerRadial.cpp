#include "CameraControllerRadial.h"

#include "CameraController3D.h"

using namespace controllers;

void CameraControllerRadial::Compose(CameraController3D* camera_3d)
{
    camera_3d_ = camera_3d;
}

void CameraControllerRadial::Compose(engine::physics::Collision* collision)
{
    collision_ = collision;
}

void CameraControllerRadial::SetFollowing()
{
    auto* camera_transform = camera_object_->transform();

    rotation_ = camera_transform->world_matrix().ToEuler();

    if (collision_->CastRay(
        camera_transform->position(), camera_transform->world_matrix().Forward(), &following_))
    {
        distance_ = float3::Distance(following_->position(), camera_transform->position());
        return;
    }

    center_ = camera_transform->position() + camera_transform->world_matrix().Forward() * 3;
    distance_ = 3;
}

void CameraControllerRadial::Update(float delta)
{
    if (!is_active_) return;

    auto* camera_transform = camera_object_->transform();
    const auto rotation = float4x4::CreateFromYawPitchRoll(rotation_);
    const auto offset = float3::Transform(float3::Backward * distance_, rotation);

    if (following_ != nullptr)
        camera_transform->position() = following_->position() + offset;
    else
        camera_transform->position() = center_ + offset;

    camera_transform->rotation() = -rotation_;
    camera_transform->rotation() *= -1;
    camera_transform->rotation().z = 0;
    camera_transform->UpdateWorldMatrix();
    camera_object_->camera()->UpdateViewMatrix();
}

void CameraControllerRadial::OnMouseEvent(const engine::input::mouse::Event& args)
{
    if (!is_active_) return;

    if (device_->IsKeyDown(engine::input::keyboard::Keys::LeftShift)) return;
    rotation_.x = max(min(rotation_.x + static_cast<float>(args.translation.y) * -0.003f, 3.14f/3.0f), -3.14f/3.0f);
    rotation_.y += static_cast<float>(args.translation.x) * -0.003f;
}

void CameraControllerRadial::OnKeyboardEvent(const engine::input::keyboard::Event& args)
{
    if (!is_active_) return;

    CameraController::OnKeyboardEvent(args);
    if (args.code != engine::input::keyboard::Keys::MouseLeftButton
        || args.button != engine::input::keyboard::Buttons::Up)
        return;
    following_ = nullptr;
    rotation_ = float3();
    is_active_ = false;
    camera_3d_->is_active() = true;
}
