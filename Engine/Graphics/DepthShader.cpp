#include "DepthShader.h"

#include "d3dcompiler.h"

using namespace engine::graphics;

void DepthShader::Compose(DXDevice* device, DXDeviceContext* context, const char16* file_path)
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

    constexpr uint32 input_layout_amount = 1;
    DXInputElementDescriptor input_layout_descriptor[input_layout_amount];
    input_layout_descriptor[0].SemanticName = "POSITION";
    input_layout_descriptor[0].SemanticIndex = 0;
    input_layout_descriptor[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    input_layout_descriptor[0].InputSlot = 0;
    input_layout_descriptor[0].AlignedByteOffset = 0;
    input_layout_descriptor[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    input_layout_descriptor[0].InstanceDataStepRate = 0;

    device->CreateInputLayout(
        input_layout_descriptor, input_layout_amount,
        vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(),
        &input_layout_);

    vertex_shader_buffer->Release();
    pixel_shader_buffer->Release();

    transform_buffer_.Compose(device, context);
}

void DepthShader::Destroy() const
{
    transform_buffer_.Release();
    input_layout_->Release();
    pixel_shader_->Release();
    vertex_shader_->Release();
}

void DepthShader::Render(
    DXDeviceContext* device_context, uint32 index_count, const float4x4& world, const float4x4& view_projection)
{
    transform_buffer_.data.world_view_projection = world * view_projection;
    transform_buffer_.Apply();
    device_context->VSSetConstantBuffers(0, 1, transform_buffer_.buffer_pointer());

    device_context->IASetInputLayout(input_layout_);

    device_context->VSSetShader(vertex_shader_, nullptr, 0);
    device_context->PSSetShader(pixel_shader_, nullptr, 0);

    device_context->DrawIndexed(index_count, 0, 0);
}
