#include "PlanetObject.h"

using namespace objects;

void PlanetObject::Compose(
    const float3& position, const float3& scale, engine::graphics::RenderComponent* render)
{
    transform_.position() = position;
    transform_.scale() = scale;
    transform_.UpdateWorldMatrix();
    render_ = render;
    render_->Compose(&transform_);
}

engine::transform::TransformComponent* PlanetObject::transform()
{
    return &transform_;
}

engine::graphics::RenderComponent* PlanetObject::render() const
{
    return render_;
}
