#pragma once
#include "../Engine/Graphics/RenderComponent.h"

#include "../Lib/assimp/mesh.h"

namespace components
{
    class MeshComponent : public engine::graphics::RenderComponent
    {
    public:
        MeshComponent(const aiMesh* mesh);
    };
}
