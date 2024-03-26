#pragma once
#include "Types.h"

namespace engine
{
    namespace graphics
    {
        class Buffer final
        {
        public:
            void Compose(DXDevice* device, uint32 bind_flags, const void* data, uint32 size);

            DXBuffer* buffer() const;
            DXBuffer* const * buffer_pointer() const;

        private:
            DXBuffer* buffer_ = nullptr;
        };
    }
}
