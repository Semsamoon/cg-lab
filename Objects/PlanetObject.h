#pragma once
#include "../Engine/Graphics/RenderComponent.h"
#include "../Engine/Transform/TransformComponent.h"

namespace objects
{
    class PlanetObject
    {
    public:
        void Compose(
            const float3& position, const float3& scale, engine::graphics::RenderComponent* render);

        engine::transform::TransformComponent* transform();
        engine::graphics::RenderComponent* render() const;

    private:
        engine::transform::TransformComponent transform_;
        engine::graphics::RenderComponent* render_ = nullptr;
    };
}
