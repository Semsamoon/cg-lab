#pragma once
#include "../../Lib/Types.h"

namespace engine
{
    namespace graphics
    {
        class RenderAble
        {
        public:
            virtual ~RenderAble() = default;
            virtual void Compose(DXDevice* device) = 0;
            virtual void Render(DXDeviceContext* context) = 0;

            virtual uint32 index_count() const = 0;
            virtual const float4x4& world() = 0;
            virtual DXShaderResourceView* texture() const = 0;
        };
    }
}
