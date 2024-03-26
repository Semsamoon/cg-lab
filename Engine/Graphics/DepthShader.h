#pragma once
#include "ConstantBuffer.hpp"
#include "Types.h"

namespace engine
{
    namespace graphics
    {
        class DepthShader
        {
        private:
            struct TransformBufferElement
            {
                float4x4 world;
                float4x4 light_world_view_projection;
            };

        public:
            void Compose(DXDevice* device, const char16* file_path);
            void Destroy() const;
            void Render(
                DXDeviceContext* context, uint32 index_count,
                const float4x4& world, const float4x4& light_view_projection);

        private:
            DXVertexShader* vertex_shader_ = nullptr;
            DXPixelShader* pixel_shader_ = nullptr;

            DXInputLayout* input_layout_ = nullptr;

            ConstantBuffer<TransformBufferElement> transform_buffer_;
        };
    }
}
