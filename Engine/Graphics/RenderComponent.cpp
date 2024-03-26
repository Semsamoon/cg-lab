#include "RenderComponent.h"

#include <d3dcompiler.h>
#include <iostream>
#include <WICTextureLoader.h>

using namespace engine::graphics;

void RenderComponent::Compose(transform::TransformComponent* transform)
{
    transform_ = transform;
}

void RenderComponent::Compose(const std::string& texture_file_path)
{
    texture_file_path_ = texture_file_path;
}

void RenderComponent::Compose(DXDevice* device)
{
    vertex_buffer_.Compose(device);
    index_buffer_.Compose(device);
    index_count_ = static_cast<uint32>(index_buffer_.data.size());
    const std::wstring wide_string(texture_file_path_.begin(), texture_file_path_.end());
    DirectX::CreateWICTextureFromFile(device, wide_string.data(), nullptr, &texture_);
}

void RenderComponent::Render(DXDeviceContext* context)
{
    constexpr uint32 stride = sizeof(VertexInputBufferElement);
    constexpr uint32 offset = 0;

    context->IASetVertexBuffers(0, 1, vertex_buffer_.buffer_pointer(), &stride, &offset);
    context->IASetIndexBuffer(index_buffer_.buffer(), DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

const float4x4& RenderComponent::world()
{
    return transform_->world_matrix();
}

DXShaderResourceView* RenderComponent::texture() const
{
    return texture_;
}

uint32 RenderComponent::index_count() const
{
    return index_count_;
}
