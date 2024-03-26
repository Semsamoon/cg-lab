#include "Shader.h"

#include <d3dcompiler.h>
#include "../../Lib/Types.h"

using namespace engine::graphics;

void Shader::Compose(DXDevice* device, const char16* file_path)
{
    DXBlob* error_message;
    DXBlob* vertex_shader_buffer;
    DXBlob* pixel_shader_buffer;

    D3DCompileFromFile(file_path,
                       nullptr, nullptr,
                       "VSMain", "vs_5_0",
                       D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
                       &vertex_shader_buffer, &error_message);

    D3DCompileFromFile(file_path,
                       nullptr, nullptr,
                       "PSMain", "ps_5_0",
                       D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
                       &pixel_shader_buffer, &error_message);

    device->CreateVertexShader(
        vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(),
        nullptr, &vertex_shader_);

    device->CreatePixelShader(
        pixel_shader_buffer->GetBufferPointer(), pixel_shader_buffer->GetBufferSize(),
        nullptr, &pixel_shader_);

    constexpr uint32 input_layout_amount = 3;
    DXInputElementDescriptor input_layout_descriptor[input_layout_amount];
    input_layout_descriptor[0].SemanticName = "POSITION";
    input_layout_descriptor[0].SemanticIndex = 0;
    input_layout_descriptor[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    input_layout_descriptor[0].InputSlot = 0;
    input_layout_descriptor[0].AlignedByteOffset = 0;
    input_layout_descriptor[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    input_layout_descriptor[0].InstanceDataStepRate = 0;

    input_layout_descriptor[1].SemanticName = "NORMAL";
    input_layout_descriptor[1].SemanticIndex = 0;
    input_layout_descriptor[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    input_layout_descriptor[1].InputSlot = 0;
    input_layout_descriptor[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    input_layout_descriptor[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    input_layout_descriptor[1].InstanceDataStepRate = 0;

    input_layout_descriptor[2].SemanticName = "TEXCOORD";
    input_layout_descriptor[2].SemanticIndex = 0;
    input_layout_descriptor[2].Format = DXGI_FORMAT_R32G32_FLOAT;
    input_layout_descriptor[2].InputSlot = 0;
    input_layout_descriptor[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    input_layout_descriptor[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    input_layout_descriptor[2].InstanceDataStepRate = 0;

    device->CreateInputLayout(
        input_layout_descriptor, input_layout_amount,
        vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(),
        &input_layout_);

    vertex_shader_buffer->Release();
    pixel_shader_buffer->Release();

    DXSamplerDescriptor sampler_descriptor;
    sampler_descriptor.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampler_descriptor.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_descriptor.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_descriptor.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_descriptor.MipLODBias = 0.0f;
    sampler_descriptor.MaxAnisotropy = 1;
    sampler_descriptor.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sampler_descriptor.BorderColor[0] = 0;
    sampler_descriptor.BorderColor[1] = 0;
    sampler_descriptor.BorderColor[2] = 0;
    sampler_descriptor.BorderColor[3] = 0;
    sampler_descriptor.MinLOD = 0;
    sampler_descriptor.MaxLOD = D3D11_FLOAT32_MAX;

    device->CreateSamplerState(&sampler_descriptor, &sample_state_wrap_);

    sampler_descriptor.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_descriptor.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_descriptor.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    device->CreateSamplerState(&sampler_descriptor, &sample_state_clamp_);

    transform_buffer_.Compose(device);
}

void Shader::Destroy() const
{
    transform_buffer_.Release();
    sample_state_wrap_->Release();
    sample_state_clamp_->Release();
    input_layout_->Release();
    pixel_shader_->Release();
    vertex_shader_->Release();
}

void Shader::Render(
    DXDeviceContext* context, uint32 index_count,
    const float4x4& world, const float4x4& view_projection,
    DXShaderResourceView* texture)
{
    transform_buffer_.data.world = world;
    transform_buffer_.data.world_view_projection = world * view_projection;
    transform_buffer_.Apply(context);
    context->VSSetConstantBuffers(0, 1, transform_buffer_.buffer_pointer());

    context->PSSetShaderResources(0, 1, &texture);

    context->IASetInputLayout(input_layout_);

    context->VSSetShader(vertex_shader_, nullptr, 0);
    context->PSSetShader(pixel_shader_, nullptr, 0);

    context->PSSetSamplers(0, 1, &sample_state_wrap_);
    context->PSSetSamplers(1, 1, &sample_state_clamp_);

    context->DrawIndexed(index_count, 0, 0);
}