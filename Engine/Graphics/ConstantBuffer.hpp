#pragma once
#include "Types.h"

namespace engine
{
    namespace graphics
    {
        template <typename T>
        class ConstantBuffer final
        {
        public:
            T data;

            void Compose(DXDevice* device, DXDeviceContext* context)
            {
                context_ = context;

                DXBufferDescriptor descriptor;
                descriptor.Usage = D3D11_USAGE_DYNAMIC;
                descriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                descriptor.MiscFlags = 0;
                descriptor.ByteWidth = static_cast<uint32>(sizeof(T) + (16 - (sizeof(T) % 16)));
                descriptor.StructureByteStride = 0;

                device->CreateBuffer(&descriptor, nullptr, &buffer_);
            }

            void Apply()
            {
                DXMappedSubresource subresource;
                context_->Map(buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
                CopyMemory(subresource.pData, &data, sizeof(T));
                context_->Unmap(buffer_, 0);
            }

            DXBuffer* buffer() const
            {
                return buffer_;
            }

            DXBuffer* const * buffer_pointer() const
            {
                return &buffer_;
            }

        private:
            DXBuffer* buffer_ = nullptr;
            DXDeviceContext* context_ = nullptr;
        };
    }
}
