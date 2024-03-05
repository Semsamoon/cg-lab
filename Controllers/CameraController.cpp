#include "CameraController.h"

#include "../Game.h"

using namespace controllers;

void CameraController::Compose(objects::CameraObject* camera_object, engine::input::Device* device)
{
    camera_object_ = camera_object;
    device_ = device;
    device_->mouse_event.AddRaw(this, &CameraController::OnMouseEvent);
}

void CameraController::Update(float delta)
{
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

void CameraController::OnMouseEvent(const engine::input::mouse::Event& args) const
{
    if (device_->IsKeyDown(engine::input::keyboard::Keys::LeftShift)) return;
    auto* transform = camera_object_->transform();
    transform->rotation().x += args.translation.y * -0.003f;
    transform->rotation().y += args.translation.x * -0.003f;
    transform->UpdateWorldMatrix();
    camera_object_->camera()->UpdateViewMatrix();
}
