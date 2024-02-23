#pragma once

#include "Pipeline.h"

namespace Engine
{
    namespace Render
    {
        class Pipeline;

        class Able
        {
        public:
            virtual ~Able() = default;

            virtual void Compose(Pipeline* pPipeline) = 0;
            virtual void Render() = 0;
        };
    }
}
