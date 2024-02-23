#pragma once

#include "../Components/Rendering.h"

namespace Basic
{
    namespace Shapes
    {
        class Square final : public Components::Rendering
        {
        public:
            Square(const float2& center, const float2& halfSize);

            void Compose(Engine::Render::Pipeline* pPipeline) override;

        private:
            float4 pointsSquare_[4 * 2]{};
            int32 indicesSquare_[3 * 2]{};
        };
    }
}
