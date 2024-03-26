#pragma once
#include <string>

#include "Buffer.h"
#include "RenderPipeline.h"
#include "../Transform/TransformComponent.h"
#include "../../Lib/Types.h"

namespace engine
{
    namespace graphics
    {
        class RenderComponent : public RenderAble
        {
        protected:
            struct VertexInputElement
            {
                float3 position;
                float3 normal;
                float2 uv;
            };

        public:
            void Compose(transform::TransformComponent* transform);
            void Compose(const std::string& texture_file_path);
            void Compose(DXDevice* device) override;
            void Render(DXDeviceContext* context) override;

            const float4x4& world() override;
            DXShaderResourceView* texture() const override;

        protected:
            transform::TransformComponent* transform_ = nullptr;

            Buffer vertex_buffer_;
            Buffer index_buffer_;

            std::vector<VertexInputElement> vertex_input_params_{};
            std::vector<uint32> indexes_{};

            DXShaderResourceView* texture_ = nullptr;
            std::string texture_file_path_;
        };
    }
}
