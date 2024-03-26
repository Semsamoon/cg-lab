#pragma once

#include <d3d11.h>
#include <vector>

#include "ConstantBuffer.hpp"
#include "DepthShader.h"
#include "RenderAble.h"
#include "ShadowShader.h"
#include "../../Lib/Types.h"

namespace engine
{
    namespace graphics
    {
        class RenderAble;

        struct LightTransformBufferElement
        {
            float4x4 light_view_projection;
            float4 camera_position;
        };

        struct AmbientLightBufferElement
        {
            float4 color;
            float intensity = 0;
        };

        struct DirectionalLightBufferElement
        {
            float4 direction;
            float4 color;
            float4 diffK_specA_specK;
        };

        struct PointLightParams
        {
            float4 world_position;
            float4 color;
            float4 diffK_specA_specK;
        };

        class RenderPipeline final
        {
        public:
            void Compose(PHandlerWindow handler_window, const Point& size);
            void Render(const float4x4& view_projection, float delta);
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
            DXRasterizerState* rasterizer_state_ = nullptr;

            DXRenderTargetView* render_target_view_final_ = nullptr;
            DXRenderTargetView* render_target_view_depth_ = nullptr;
            DXShaderResourceView* depth_shader_resource_view_ = nullptr;

            DXDepthStencilView* depth_stencil_view_ = nullptr;
            DXTexture2D* depth_stencil_buffer_ = nullptr;

            //Renderable Game Objects
            std::vector<RenderAble*> render_ables_{};

            //Shaders
            DepthShader depth_shader_{};
            ShadowShader shadow_shader_{};

            //Constant Buffers
            ConstantBuffer<LightTransformBufferElement> light_transform_buffer_;
            ConstantBuffer<AmbientLightBufferElement> ambient_light_buffer_;
            ConstantBuffer<DirectionalLightBufferElement> directional_light_buffer_;
            ConstantBuffer<PointLightParams> point_light_buffer_;

            float t_ = 0;

            DXTexture2D* background_texture_depth_ = nullptr;
            DXTexture2D* background_texture_final_ = nullptr;
        };
    }
}
