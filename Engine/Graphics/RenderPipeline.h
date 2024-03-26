#pragma once

#include <d3d11.h>
#include <vector>

#include "ConstantBuffer.hpp"
#include "RenderAble.h"
#include "../../Lib/Types.h"

namespace engine
{
    namespace graphics
    {
        class RenderAble;

        struct LightsParams
        {
            float4 direction;
            float4 color;
            float4 k;
        };

        class RenderPipeline final
        {
        public:
            void Compose(PHandlerWindow handler_window, const Point& size);
            void Render(const float4x4& camera, float delta);
            void Destroy() const;

            void Add(RenderAble* render_able);

            DXDevice* device() const;
            DXDeviceContext* device_context() const;

        private:
            void ComposeDeviceAndSwapChain(PHandlerWindow handler_window);
            void ComposeRenderTargetView();
            void ComposeDepthStencilBuffer(const Point& size);

            Point size_{};
            DXViewport viewport_{};
            DXDevice* device_ = nullptr;
            DXDeviceContext* device_context_ = nullptr;
            DXSwapChain* swap_chain_ = nullptr;
            DXRenderTargetView* render_target_view_ = nullptr;
            DXDepthStencilView* depth_stencil_view_ = nullptr;
            DXTexture2D* depth_stencil_buffer_ = nullptr;
            std::vector<RenderAble*> render_ables_{};

            ConstantBuffer<LightsParams> light_buffer_;
            ConstantBuffer<LightsParams> dyn_light_buffer_;
            float t_ = 0;
        };
    }
}
