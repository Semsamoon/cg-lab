#include "Buffer.h"

using namespace engine::graphics;

void Buffer::Compose(DXDevice* device, uint32 bind_flags, const void* data, uint32 size)
{
    DXBufferDescriptor buffer_descriptor;
    buffer_descriptor.Usage = D3D11_USAGE_DEFAULT;
    buffer_descriptor.BindFlags = bind_flags;
    buffer_descriptor.CPUAccessFlags = 0;
    buffer_descriptor.MiscFlags = 0;
    buffer_descriptor.StructureByteStride = 0;
    buffer_descriptor.ByteWidth = size;

    DXSubresourceData subresource_data;
    subresource_data.pSysMem = data;
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;

    device->CreateBuffer(&buffer_descriptor, &subresource_data, &buffer_);
}

DXBuffer* Buffer::buffer() const
{
    return buffer_;
}

DXBuffer* const* Buffer::buffer_pointer() const
{
    return &buffer_;
}
