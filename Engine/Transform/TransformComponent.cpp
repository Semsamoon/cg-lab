#include "TransformComponent.h"

using namespace engine::transform;

TransformComponent::TransformComponent()
{
    scale_ = float3(1);
    UpdateWorldMatrix();
}

void TransformComponent::Compose(TransformComponent* parent)
{
    parent_ = parent;
}

void TransformComponent::UpdateWorldMatrix()
{
    world_matrix_ =
        float4x4::CreateScale(scale_)
        * float4x4::CreateFromYawPitchRoll(rotation_)
        * float4x4::CreateTranslation(position_)
        * (parent_ == nullptr ? float4x4::Identity : parent_->world_matrix_);
}

TransformComponent* TransformComponent::parent() const
{
    return parent_;
}

float4x4& TransformComponent::world_matrix()
{
    return world_matrix_;
}

float3& TransformComponent::position()
{
    return position_;
}

float3& TransformComponent::rotation()
{
    return rotation_;
}

float3& TransformComponent::scale()
{
    return scale_;
}
