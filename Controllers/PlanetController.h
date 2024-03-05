#pragma once
#include "../Engine/Transform/TransformComponent.h"
#include "../Engine/Update/VariableUpdateAble.h"

namespace controllers
{
    class PlanetController final : public engine::update::VariableUpdateAble
    {
    public:
        void Compose(
            engine::transform::TransformComponent* transform,
            engine::transform::TransformComponent* transform_rotation_center);
        void SetSpeed(
            const float3& offset, const float3& axis_around_center, float speed_around_center,
            const float3& axis_self, float speed_self_axis);
        void Update(float delta) override;

    private:
        void UpdateAroundCenter(float delta);
        void UpdateSelf(float delta) const;

        engine::transform::TransformComponent* transform_ = nullptr;
        engine::transform::TransformComponent* transform_rotation_center_ = nullptr;

        float3 offset_;
        float3 axis_around_center_;
        float speed_around_center_ = 0;
        float angle_around_center_ = 0;
        float3 axis_self_;
        float speed_self_axis_ = 0;
    };
}
