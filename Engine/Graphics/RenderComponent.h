#pragma once
#include <string>

#include "IndexBuffer.h"
#include "RenderPipeline.h"
#include "VertexBuffer.hpp"
#include "../Transform/TransformComponent.h"
#include "../../Lib/Types.h"

namespace engine
{
    namespace graphics
    {
        class RenderComponent : public RenderAble
        {
        protected:
            struct VertexInputBufferElement
            {
                float3 position;
                float3 normal;
                float2 uv;
            };

            struct TransformBufferElement
            {
                float4x4 world;
                float4x4 world_view_projection;
            };

        public:
            void Compose(transform::TransformComponent* transform);
            void Compose(const std::string& texture_file_path);
            void Compose(DXDevice* device) override;

            void Render(DXDeviceContext* context) override;

            const float4x4& world() override;
            DXShaderResourceView* texture() const override;
            uint32 index_count() const override;

        protected:
            transform::TransformComponent* transform_ = nullptr;

            ConstantBuffer<TransformBufferElement> transform_buffer_{};
            VertexBuffer<VertexInputBufferElement> vertex_buffer_{};
            IndexBuffer index_buffer_{};

            DXShaderResourceView* texture_ = nullptr;
            std::string texture_file_path_;

            uint32 index_count_ = 0;
        };
    }
}
