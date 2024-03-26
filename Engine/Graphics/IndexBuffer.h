#pragma once
#include <vector>

#include "Types.h"

namespace engine
{
    namespace graphics
    {
        class IndexBuffer final
        {
        public:
            std::vector<uint32> data{};

            void Compose(DXDevice* device)
            {
                DXBufferDescriptor buffer_descriptor{};
                buffer_descriptor.Usage = D3D11_USAGE_DEFAULT;
                buffer_descriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
                buffer_descriptor.ByteWidth = static_cast<uint32>(sizeof(uint32) * data.size());

                DXSubresourceData subresource_data{};
                subresource_data.pSysMem = data.data();

                device->CreateBuffer(&buffer_descriptor, &subresource_data, &buffer_);
            }

            void Destroy() const
            {
                buffer_->Release();
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
        };
    }
}
