#pragma once
#include "../Engine/Graphics/RenderComponent.h"
#include "../Engine/Physics/CollisionAble.h"

namespace objects
{
    class PlanetObject final : public engine::physics::CollisionAble
    {
    public:
        void Compose(
            const float3& position, const float3& scale, engine::graphics::RenderComponent* render);

        engine::graphics::RenderComponent* render() const;

        DXBox& bounding_box() override;

    private:
        engine::graphics::RenderComponent* render_ = nullptr;

        DXBox bounding_box_;
    };
}
