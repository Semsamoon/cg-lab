#include "PlanetController.h"

using namespace controllers;

void PlanetController::Compose(
    objects::PlanetObject* planet, engine::transform::TransformComponent* transform_rotation_center)
{
    planet_ = planet;
    transform_ = planet->transform();
    transform_rotation_center_ = transform_rotation_center;
}

void PlanetController::SetSpeed(
    const float3& offset, const float3& axis_around_center, float speed_around_center,
    const float3& axis_self, float speed_self_axis)
{
    offset_ = offset;
    axis_around_center_ = axis_around_center;
    speed_around_center_ = speed_around_center;
    axis_self_ = axis_self;
    speed_self_axis_ = speed_self_axis;
}

void PlanetController::Update(float delta)
{
    if (transform_rotation_center_ != nullptr) UpdateAroundCenter(delta);
    UpdateSelf(delta);
    planet_->bounding_box().Center = transform_->position();
    transform_->UpdateWorldMatrix();
}

void PlanetController::UpdateAroundCenter(float delta)
{
    angle_around_center_ += speed_around_center_ * delta;
    const auto rotation_around_center = float4x4::CreateFromAxisAngle(axis_around_center_, angle_around_center_);
    transform_->position() = transform_rotation_center_->position()
        + float3::Transform(offset_, rotation_around_center);
}

void PlanetController::UpdateSelf(float delta) const
{
    const auto rotation_around_self = float4x4::CreateFromAxisAngle(axis_self_, speed_self_axis_ * delta);
    auto rotated = transform_->world_matrix() * rotation_around_self;
    float3 scale, translation;
    DirectX::SimpleMath::Quaternion rotation;
    rotated.Decompose(scale, rotation, translation);
    transform_->rotation() = rotation.ToEuler();
}
