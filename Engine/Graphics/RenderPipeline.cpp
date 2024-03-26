#include "RenderPipeline.h"

#include "../../Game.h"

using namespace engine::graphics;
using namespace DirectX::SimpleMath;

void RenderPipeline::Compose(PHandlerWindow handler_window, const Point& size)
{
    size_ = size;
    viewport_.Width = static_cast<float>(size.x);
    viewport_.Height = static_cast<float>(size.y);
    viewport_.TopLeftX = 0;
    viewport_.TopLeftY = 0;
    viewport_.MinDepth = 0;
    viewport_.MaxDepth = 1;

    ComposeDeviceAndSwapChain(handler_window);
    ComposeRenderTargetView();
    ComposeDepthStencilBuffer(size);

    DXRasterizerDescriptor rasterizer_state_params;
    rasterizer_state_params.CullMode = D3D11_CULL_NONE;
    rasterizer_state_params.FillMode = D3D11_FILL_SOLID;
    device_->CreateRasterizerState(&rasterizer_state_params, &rasterizer_state_);

    depth_shader_.Compose(device_, device_context_, L"\\Shaders\\DepthShader.hlsl");
    shadow_shader_.Compose(device_, device_context_, L"\\Shaders\\ShadowShader.hlsl");

    light_transform_buffer_.data.light_view_projection = float4x4::CreateLookAt(
        float3(0, 10, 0), float3(0, 0, 0), float3::Forward) * float4x4::CreatePerspectiveFieldOfView(
        3.14f / 2, 1, 0.01f, 1000);
    light_transform_buffer_.Compose(device_, device_context_);

    ambient_light_buffer_.data.color = float4(1, 1, 1, 0);
    ambient_light_buffer_.data.intensity = 0.2f;
    ambient_light_buffer_.Compose(device_, device_context_);

    directional_light_buffer_.data.color = float4(1, 1, 1, 0);
    directional_light_buffer_.data.direction = float4(-0.7f, -0.7f, 0, 0);
    directional_light_buffer_.data.diffK_specA_specK = float4(0.8f, 2.0f, 0.2f, 0);
    directional_light_buffer_.Compose(device_, device_context_);

    point_light_buffer_.data.world_position = float4(0, 1, 0, 0);
    point_light_buffer_.data.color = float4(1, 0, 0, 0);
    point_light_buffer_.data.diffK_specA_specK = float4(1.0f, 1.0f, 1.0f, 0);
    point_light_buffer_.Compose(device_, device_context_);
}

void RenderPipeline::Render(const float4x4& view_projection, float delta)
{
    constexpr float color[] = {0.0f, 0.0f, 0.0f, 1.0f};

    device_context_->ClearState();
    device_context_->RSSetViewports(1, &viewport_);
    device_context_->RSSetState(rasterizer_state_);


    //Render for Depth Shader
    device_context_->ClearRenderTargetView(render_target_view_depth_, color);
    device_context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    device_context_->OMSetRenderTargets(1, &render_target_view_depth_, depth_stencil_view_);

    for (auto* render_able : render_ables_)
    {
        render_able->Render(device_context_);
        depth_shader_.Render(device_context_, render_able->index_count(), render_able->world(), view_projection);
    }

    //Set Constant Buffers for Shadow Shader
    const auto camera_position = Game::Instance()->camera()->position();
    light_transform_buffer_.data.camera_position = float4(camera_position.x, camera_position.y, camera_position.z, 0);
    light_transform_buffer_.Apply();
    device_context_->VSSetConstantBuffers(1, 1, light_transform_buffer_.buffer_pointer());

    ambient_light_buffer_.Apply();
    device_context_->PSSetConstantBuffers(2, 1, ambient_light_buffer_.buffer_pointer());

    directional_light_buffer_.data.direction = float4::Transform(directional_light_buffer_.data.direction,
                                                                 Quaternion::CreateFromYawPitchRoll(1 * delta, 0, 0));
    directional_light_buffer_.Apply();
    device_context_->PSSetConstantBuffers(3, 1, directional_light_buffer_.buffer_pointer());

    //t_ += delta;
    //point_light_buffer_.data.world_position = float4(0, 1, sin(t_) * 5, 0);
    //point_light_buffer_.Apply();
    //device_context_->PSSetConstantBuffers(4, 1, point_light_buffer_.buffer_pointer());

    //Render for Shadow Shader
    device_context_->ClearRenderTargetView(render_target_view_final_, color);
    device_context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    device_context_->OMSetRenderTargets(1, &render_target_view_final_, depth_stencil_view_);

    device_context_->PSSetShaderResources(1, 1, &depth_shader_resource_view_);

    for (auto* render_able : render_ables_)
    {
        render_able->Render(device_context_);
        shadow_shader_.Render(
            device_context_, render_able->index_count(), render_able->world(), view_projection, render_able->texture());
    }

    swap_chain_->Present(1, DXGI_PRESENT_DO_NOT_WAIT);
}

void RenderPipeline::Destroy() const
{
    render_target_view_final_->Release();
    swap_chain_->Release();
    device_context_->Release();
    device_->Release();
}

void RenderPipeline::Add(RenderAble* render_able)
{
    render_ables_.push_back(render_able);
    render_able->Compose(device_);
}

DXDevice* RenderPipeline::device() const
{
    return device_;
}

DXDeviceContext* RenderPipeline::device_context() const
{
    return device_context_;
}

void RenderPipeline::ComposeDeviceAndSwapChain(PHandlerWindow handler_window)
{
    constexpr D3D_FEATURE_LEVEL feature_level[] = {D3D_FEATURE_LEVEL_11_1};

    DXGI_SWAP_CHAIN_DESC swap_chain_descriptor;
    swap_chain_descriptor.BufferCount = 2;
    swap_chain_descriptor.BufferDesc.Width = size_.x;
    swap_chain_descriptor.BufferDesc.Height = size_.y;
    swap_chain_descriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_descriptor.BufferDesc.RefreshRate.Numerator = 60;
    swap_chain_descriptor.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_descriptor.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swap_chain_descriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swap_chain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_descriptor.OutputWindow = handler_window;
    swap_chain_descriptor.Windowed = true;
    swap_chain_descriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swap_chain_descriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swap_chain_descriptor.SampleDesc.Count = 1;
    swap_chain_descriptor.SampleDesc.Quality = 0;

    D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
        feature_level, 1, D3D11_SDK_VERSION, &swap_chain_descriptor,
        &swap_chain_, &device_, nullptr, &device_context_);
}

void RenderPipeline::ComposeRenderTargetView()
{
    DXTexture2DDescriptor texture_depth_descriptor;
    texture_depth_descriptor.Width = 1024;
    texture_depth_descriptor.Height = 1024;
    texture_depth_descriptor.MipLevels = 1;
    texture_depth_descriptor.ArraySize = 1;
    texture_depth_descriptor.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texture_depth_descriptor.SampleDesc.Count = 1;
    texture_depth_descriptor.SampleDesc.Quality = 0;
    texture_depth_descriptor.Usage = D3D11_USAGE_DEFAULT;
    texture_depth_descriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texture_depth_descriptor.CPUAccessFlags = 0;
    texture_depth_descriptor.MiscFlags = 0;

    device_->CreateTexture2D(&texture_depth_descriptor, nullptr, &background_texture_depth_);
    device_->CreateRenderTargetView(background_texture_depth_, nullptr, &render_target_view_depth_);
    device_->CreateShaderResourceView(background_texture_depth_, nullptr, &depth_shader_resource_view_);

    swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&background_texture_final_));
    device_->CreateRenderTargetView(background_texture_final_, nullptr, &render_target_view_final_);
}

void RenderPipeline::ComposeDepthStencilBuffer(const Point& size)
{
    DXTexture2DDescriptor depth_stencil_descriptor;
    depth_stencil_descriptor.Width = size.x;
    depth_stencil_descriptor.Height = size.y;
    depth_stencil_descriptor.MipLevels = 1;
    depth_stencil_descriptor.ArraySize = 1;
    depth_stencil_descriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_descriptor.SampleDesc.Count = 1;
    depth_stencil_descriptor.SampleDesc.Quality = 0;
    depth_stencil_descriptor.Usage = D3D11_USAGE_DEFAULT;
    depth_stencil_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_stencil_descriptor.CPUAccessFlags = 0;
    depth_stencil_descriptor.MiscFlags = 0;

    device_->CreateTexture2D(&depth_stencil_descriptor, nullptr, &depth_stencil_buffer_);
    device_->CreateDepthStencilView(depth_stencil_buffer_, nullptr, &depth_stencil_view_);
}
