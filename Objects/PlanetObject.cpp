#include "PlanetObject.h"

using namespace objects;

void PlanetObject::Compose(
    const float3& position, const float3& scale, engine::graphics::RenderComponent* render)
{
    transform_ = new engine::transform::TransformComponent();
    transform_->position() = position;
    transform_->scale() = scale;
    transform_->UpdateWorldMatrix();
    bounding_box_.Center = position;
    bounding_box_.Extents = scale / 3;
    render_ = render;
    render_->Compose(transform_);
}

engine::graphics::RenderComponent* PlanetObject::render() const
{
    return render_;
}

DXBox& PlanetObject::bounding_box()
{
    return bounding_box_;
}
