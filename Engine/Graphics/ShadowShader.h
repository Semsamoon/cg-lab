#pragma once
#include "ConstantBuffer.hpp"
#include "Types.h"

namespace engine
{
    namespace graphics
    {
        class ShadowShader
        {
        private:
            struct TransformBufferElement
            {
                float4x4 world;
                float4x4 world_view_proj;
            };

        public:
            void Compose(DXDevice* device, DXDeviceContext* context, const char16* file_path);
            void Destroy() const;
            void Render(
                DXDeviceContext* context, uint32 index_count,
                const float4x4& world, const float4x4& view_projection,
                DXShaderResourceView* texture);

        private:
            DXVertexShader* vertex_shader_ = nullptr;
            DXPixelShader* pixel_shader_ = nullptr;
            DXInputLayout* input_layout_ = nullptr;
            DXSamplerState* sample_state_wrap_ = nullptr;
            DXSamplerState* sample_state_clamp_ = nullptr;
            ConstantBuffer<TransformBufferElement> transform_buffer_;
        };
    }
}
