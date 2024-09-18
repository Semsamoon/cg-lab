#pragma once
#include "Types.h"

namespace engine
{
    namespace graphics
    {
        class SquadShader
        {
        public:
            void Compose(DXDevice* device, const char16* file_path);
            void Render(
                DXDeviceContext* context, uint32 index_count,
                DXShaderResourceView* texture);

        private:
            DXVertexShader* vertex_shader_ = nullptr;
            DXPixelShader* pixel_shader_ = nullptr;

            DXInputLayout* input_layout_ = nullptr;

            DXSamplerState* sample_state_wrap_ = nullptr;
        };
    }
}
