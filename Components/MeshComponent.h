#pragma once
#include "../Engine/Graphics/RenderComponent.h"

#include "../Lib/assimp/mesh.h"

namespace components
{
    struct VertexInputParams
    {
        float3 position;
        float3 normal;
        float2 uv;
    };

    class MeshComponent : public engine::graphics::RenderComponent
    {
    public:
        MeshComponent(const aiMesh* mesh);

    private:
        std::vector<VertexInputParams> vertex_input_params_{};
        std::vector<int32> indexes_{};
    };
}
