#pragma once

#include <d3d11.h>
#include <vector>

#include "ConstantBuffer.hpp"
#include "DepthShader.h"
#include "RenderAble.h"
#include "Shader.h"
#include "../../Lib/Types.h"

namespace engine
{
    namespace graphics
    {
        class RenderAble;

        class RenderPipeline final
        {
        private:
            struct LightTransformBufferElement
            {
                float4x4 directional_light_view_projection;
                float4x4 point_light_view_projection;
                float4 camera_position;
            };

            struct AmbientLightBufferElement
            {
                float4 color;
            };

            struct DirectionalLightBufferElement
            {
                float4 direction;
                float4 color;
                float4 diffK_specA_specK;
            };

            struct PointLightBufferElement
            {
                float4 position;
                float4 color;
                float4 diffK_specA_specK;
            };

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
            void ComposeDepthStencilBuffer();

            //Shadow
            Point size_{};
            DXViewport viewport_{};
            DXDevice* device_ = nullptr;
            DXDeviceContext* device_context_ = nullptr;
            DXSwapChain* swap_chain_ = nullptr;
            DXRasterizerState* rasterizer_state_ = nullptr;

            DXRenderTargetView* render_target_view_ = nullptr;
            DXDepthStencilView* depth_stencil_view_ = nullptr;

            DXViewport viewport_depth_directional_light_{};
            DXRenderTargetView* render_target_view_depth_directional_light_ = nullptr;
            DXShaderResourceView* resource_view_depth_directional_light_ = nullptr;

            DXViewport viewport_depth_point_light_{};
            DXRenderTargetView* render_target_view_depth_point_light_ = nullptr;
            DXShaderResourceView* resource_view_depth_point_light_ = nullptr;

            //Rendering
            std::vector<RenderAble*> render_ables_{};

            Shader shader_{};
            DepthShader depth_shader_{};

            ConstantBuffer<LightTransformBufferElement> light_transform_buffer_;
            ConstantBuffer<AmbientLightBufferElement> ambient_light_buffer_;
            ConstantBuffer<DirectionalLightBufferElement> directional_light_buffer_;
            ConstantBuffer<PointLightBufferElement> point_light_buffer_;

            float3 directional_light_position_;
            float4x4 directional_light_view_;
            float4x4 directional_light_projection_;

            float3 point_light_position_;
            float4x4 point_light_view_;
            float4x4 point_light_projection_;

            float t_ = 0;

            const uint32 K_Directional_Light_Depth = 3;
            const uint32 L_Point_Light_Depth = 6;
        };
    }
}
