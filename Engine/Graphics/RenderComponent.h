#pragma once
#include <string>

#include "Buffers.h"
#include "RasterizerState.h"
#include "RenderPipeline.h"
#include "Shaders.h"
#include "../Transform/TransformComponent.h"
#include "../../Lib/Types.h"

namespace engine
{
    namespace graphics
    {
        struct RenderParams
        {
            DXPrimitiveTopology primitive_topology;
        };

        struct TransformBufferParams
        {
            float4x4 world;
            float4x4 world_view_projection;
            float4 view_position;
        };

        class RenderComponent : public RenderAble
        {
        public:
            RenderComponent();

            void Compose(transform::TransformComponent* transform, transform::TransformComponent* camera);
            void Compose(const std::string& texture_file_path);
            void Compose(RenderPipeline* pipeline) override;
            void Render(const float4x4& camera, float delta) override;

        protected:
            void UpdateTransformBuffer(DXDeviceContext* context, const float4x4& camera);

            transform::TransformComponent* transform_ = nullptr;
            ConstantBuffer<TransformBufferParams> transform_buffer_{};

            transform::TransformComponent* camera_ = nullptr;

            RenderParams render_params_{};
            Shaders shaders_;
            Buffers buffers_;
            RasterizerState rasterizer_state_;

            CD3D11_SAMPLER_DESC sample_descriptor{};
            DXSamplerState* sampler_state_ = nullptr;

            DXShaderResourceView* texture_ = nullptr;
            std::string texture_file_path_;

            uint32 index_count_ = 0;
        };
    }
}
