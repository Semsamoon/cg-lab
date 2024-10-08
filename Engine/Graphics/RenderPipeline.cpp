#include "RenderPipeline.h"

#include "../../Game.h"
#include "../../Components/SquadComponent.h"

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

    viewport_depth_directional_light_ = viewport_;
    viewport_depth_directional_light_.Width *= static_cast<float>(K_Directional_Light_Depth);
    viewport_depth_directional_light_.Height *= static_cast<float>(K_Directional_Light_Depth);

    viewport_depth_point_light_ = viewport_depth_directional_light_;
    viewport_depth_point_light_.Width /= static_cast<float>(L_Point_Light_Depth);
    viewport_depth_point_light_.Height /= static_cast<float>(L_Point_Light_Depth);

    ComposeDeviceAndSwapChain(handler_window);
    ComposeRenderTargetView();
    ComposeDepthStencilBuffer();

    DXRasterizerDescriptor rasterizer_descriptor{};
    rasterizer_descriptor.CullMode = D3D11_CULL_NONE;
    rasterizer_descriptor.FillMode = D3D11_FILL_SOLID;
    device_->CreateRasterizerState(&rasterizer_descriptor, &rasterizer_state_);

    //Shaders
    shader_.Compose(device_, L"./Shaders/Shader.hlsl");
    depth_shader_.Compose(device_, L"./Shaders/DepthShader.hlsl");
    squad_shader_.Compose(device_, L"./Shaders/SquadShader.hlsl");

    //Ambient Light
    ambient_light_buffer_.data.color = float4(0.2f, 0.2f, 0.2f, 0);
    ambient_light_buffer_.Compose(device_);

    //Directional Light
    directional_light_position_ =
        float3::Transform(float3(-10, 10, 0), Quaternion::CreateFromAxisAngle(float3::Left, t_));
    const auto directional_light_direction = float4(
        -directional_light_position_.x, -directional_light_position_.y, -directional_light_position_.z, 0);
    const float4x4 directional_light_view = float4x4::CreateLookAt(
        directional_light_position_, directional_light_position_ + float3::Right, float3::Up);

    directional_light_buffer_.data.color = float4(1, 1, 1, 0);
    directional_light_buffer_.data.direction = directional_light_direction;
    directional_light_buffer_.data.diffK_specA_specK = float4(1.0f, 100.0f, 1.0f, 0);
    directional_light_buffer_.Compose(device_);

    directional_light_view_ = directional_light_view;
    directional_light_projection_ = float4x4::CreatePerspectiveFieldOfView(3.14f / 2, 1, 0.01f, 1000);

    //Point Light
    point_light_position_ = float3(0, 1, sin(t_) * 5);

    point_light_buffer_.data.position = float4(
        point_light_position_.x, point_light_position_.y, point_light_position_.z, 0);
    point_light_buffer_.data.color = float4(1, 0, 0, 0);
    point_light_buffer_.data.diffK_specA_specK = float4(1.0f, 0.1f, 0.1f, 0);
    point_light_buffer_.Compose(device_);

    point_light_view_ = float4x4::CreateLookAt(
        point_light_position_, point_light_position_ + float3::Down, float3::Right);
    point_light_projection_ = float4x4::CreatePerspectiveFieldOfView(3.14f / 1.25f, 1, 0.01f, 1000);

    //Camera and Lights
    light_transform_buffer_.data.directional_light_view_projection = directional_light_view_ *
        directional_light_projection_;
    light_transform_buffer_.data.point_light_view_projection = point_light_view_ * point_light_projection_;
    light_transform_buffer_.Compose(device_);
}

void RenderPipeline::Render(const float4x4& view_projection, float delta)
{
    constexpr float color[] = {0.0f, 0.0f, 0.0f, 1.0f};

    device_context_->ClearState();
    device_context_->RSSetState(rasterizer_state_);

    //Light
    ambient_light_buffer_.Apply(device_context_);
    device_context_->PSSetConstantBuffers(2, 1, ambient_light_buffer_.buffer_pointer());

    t_ += delta;
    directional_light_position_ =
        float3::Transform(float3(-10, 5, 0), Quaternion::CreateFromAxisAngle(float3::Left, t_ / 5));
    auto directional_light_direction = float4(
        -directional_light_position_.x, -directional_light_position_.y, -directional_light_position_.z, 0);
    directional_light_direction.Normalize();
    directional_light_view_ = float4x4::CreateLookAt(
        directional_light_position_, directional_light_position_ + float3::Right + float3::Down, float3::Up);

    directional_light_buffer_.data.direction = directional_light_direction;
    directional_light_buffer_.Apply(device_context_);
    device_context_->PSSetConstantBuffers(3, 1, directional_light_buffer_.buffer_pointer());

    point_light_position_ = float3(0, 1, sin(t_) * 5);
    point_light_view_ = float4x4::CreateLookAt(
        point_light_position_, point_light_position_ + float3::Down, float3::Right);

    point_light_buffer_.data.position = float4(
        point_light_position_.x, point_light_position_.y, point_light_position_.z, 0);
    point_light_buffer_.Apply(device_context_);
    device_context_->PSSetConstantBuffers(4, 1, point_light_buffer_.buffer_pointer());

    const auto camera_position = Game::Instance()->camera()->transform()->world_matrix().Translation();
    light_transform_buffer_.data.directional_light_view_projection = directional_light_view_ *
        directional_light_projection_;
    light_transform_buffer_.data.point_light_view_projection = point_light_view_ * point_light_projection_;
    light_transform_buffer_.data.camera_position = float4(camera_position.x, camera_position.y, camera_position.z, 0);
    light_transform_buffer_.Apply(device_context_);
    device_context_->VSSetConstantBuffers(1, 1, light_transform_buffer_.buffer_pointer());

    //Render Directional
    device_context_->RSSetViewports(1, &viewport_depth_directional_light_);

    device_context_->ClearRenderTargetView(render_target_view_depth_directional_light_, color);
    device_context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    device_context_->OMSetRenderTargets(1, &render_target_view_depth_directional_light_, depth_stencil_view_);

    for (auto* render_able : render_ables_)
    {
        render_able->Render(device_context_);
        depth_shader_.Render(
            device_context_, render_able->index_count(), render_able->world(),
            directional_light_view_ * directional_light_projection_);
    }

    //Render Point
    device_context_->RSSetViewports(1, &viewport_depth_point_light_);

    device_context_->ClearRenderTargetView(render_target_view_depth_point_light_, color);
    device_context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    device_context_->OMSetRenderTargets(1, &render_target_view_depth_point_light_, depth_stencil_view_);

    for (auto* render_able : render_ables_)
    {
        render_able->Render(device_context_);
        depth_shader_.Render(
            device_context_, render_able->index_count(), render_able->world(),
            point_light_view_ * point_light_projection_);
    }

    //Render Final
    device_context_->RSSetViewports(1, &viewport_);

    device_context_->ClearRenderTargetView(render_target_view_, color);
    device_context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    device_context_->OMSetRenderTargets(1, &render_target_view_, depth_stencil_view_);

    device_context_->PSSetShaderResources(1, 1, &resource_view_depth_directional_light_);
    device_context_->PSSetShaderResources(2, 1, &resource_view_depth_point_light_);

    for (auto* render_able : render_ables_)
    {
        render_able->Render(device_context_);
        shader_.Render(
            device_context_, render_able->index_count(), render_able->world(), view_projection, render_able->texture());
        // depth_shader_.Render(
        //     device_context_, render_able->index_count(), render_able->world(),
        //     directional_light_view_ * directional_light_projection_);
    }

    components::SquadComponent squad;
    squad.Compose(device_);
    squad.Render(device_context_);
    squad_shader_.Render(device_context_, squad.index_count(), resource_view_depth_point_light_);

    swap_chain_->Present(1, DXGI_PRESENT_DO_NOT_WAIT);
}

void RenderPipeline::Destroy() const
{
    render_target_view_->Release();
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
    DXTexture2DDescriptor depth_texture_descriptor{};
    depth_texture_descriptor.Width = size_.x * K_Directional_Light_Depth;
    depth_texture_descriptor.Height = size_.y * K_Directional_Light_Depth;
    depth_texture_descriptor.MipLevels = 1;
    depth_texture_descriptor.ArraySize = 1;
    depth_texture_descriptor.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    depth_texture_descriptor.SampleDesc.Count = 1;
    depth_texture_descriptor.Usage = D3D11_USAGE_DEFAULT;
    depth_texture_descriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    ID3D11Texture2D* background_texture_depth;
    device_->CreateTexture2D(&depth_texture_descriptor, nullptr, &background_texture_depth);
    device_->CreateRenderTargetView(background_texture_depth, nullptr, &render_target_view_depth_directional_light_);
    device_->CreateShaderResourceView(background_texture_depth, nullptr, &resource_view_depth_directional_light_);

    depth_texture_descriptor.Width = size_.x * K_Directional_Light_Depth / L_Point_Light_Depth;
    depth_texture_descriptor.Height = size_.y * K_Directional_Light_Depth / L_Point_Light_Depth;

    ID3D11Texture2D* background_texture_depth_point_light;
    device_->CreateTexture2D(&depth_texture_descriptor, nullptr, &background_texture_depth_point_light);
    device_->CreateRenderTargetView(background_texture_depth_point_light, nullptr,
                                    &render_target_view_depth_point_light_);
    device_->CreateShaderResourceView(background_texture_depth_point_light, nullptr, &resource_view_depth_point_light_);

    ID3D11Texture2D* background_texture;
    swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&background_texture));
    device_->CreateRenderTargetView(background_texture, nullptr, &render_target_view_);
}

void RenderPipeline::ComposeDepthStencilBuffer()
{
    DXTexture2DDescriptor depth_stencil_descriptor;
    depth_stencil_descriptor.Width = size_.x * K_Directional_Light_Depth;
    depth_stencil_descriptor.Height = size_.y * K_Directional_Light_Depth;
    depth_stencil_descriptor.MipLevels = 1;
    depth_stencil_descriptor.ArraySize = 1;
    depth_stencil_descriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_descriptor.SampleDesc.Count = 1;
    depth_stencil_descriptor.SampleDesc.Quality = 0;
    depth_stencil_descriptor.Usage = D3D11_USAGE_DEFAULT;
    depth_stencil_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_stencil_descriptor.CPUAccessFlags = 0;
    depth_stencil_descriptor.MiscFlags = 0;

    DXTexture2D* depth_stencil_buffer;
    device_->CreateTexture2D(&depth_stencil_descriptor, nullptr, &depth_stencil_buffer);
    device_->CreateDepthStencilView(depth_stencil_buffer, nullptr, &depth_stencil_view_);
}
