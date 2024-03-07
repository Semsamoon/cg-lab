#include "CameraController3D.h"

#include "CameraControllerRadial.h"

using namespace controllers;

void CameraController3D::Compose(CameraControllerRadial* radial)
{
    radial_ = radial;
}

void CameraController3D::Update(float delta)
{
    if (!is_active_) return;

    auto* transform = camera_object_->transform();
    if (device_->IsKeyDown(engine::input::keyboard::Keys::W))
        transform->position() += transform->world_matrix().Forward() * 1.0f * delta;
    if (device_->IsKeyDown(engine::input::keyboard::Keys::S))
        transform->position() += transform->world_matrix().Backward() * 1.0f * delta;
    if (device_->IsKeyDown(engine::input::keyboard::Keys::A))
        transform->position() += transform->world_matrix().Left() * 1.0f * delta;
    if (device_->IsKeyDown(engine::input::keyboard::Keys::D))
        transform->position() += transform->world_matrix().Right() * 1.0f * delta;
    if (device_->IsKeyDown(engine::input::keyboard::Keys::Space))
        transform->position().y += 1.0f * delta;
    if (device_->IsKeyDown(engine::input::keyboard::Keys::Z))
        transform->position().y += -1.0f * delta;
    transform->UpdateWorldMatrix();
    camera_object_->camera()->UpdateViewMatrix();
}

void CameraController3D::OnMouseEvent(const engine::input::mouse::Event& args)
{
    if (!is_active_) return;

    if (device_->IsKeyDown(engine::input::keyboard::Keys::LeftShift)) return;
    auto* transform = camera_object_->transform();
    transform->rotation().x += static_cast<float>(args.translation.y) * -0.003f;
    transform->rotation().y += static_cast<float>(args.translation.x) * -0.003f;
    transform->UpdateWorldMatrix();
    camera_object_->camera()->UpdateViewMatrix();
}

void CameraController3D::OnKeyboardEvent(const engine::input::keyboard::Event& args)
{
    if (!is_active_) return;

    CameraController::OnKeyboardEvent(args);
    if (args.code != engine::input::keyboard::Keys::MouseLeftButton
        || args.button != engine::input::keyboard::Buttons::Down)
        return;
    radial_->SetFollowing();
    is_active_ = false;
    radial_->is_active() = true;
}
