#pragma once

#include "../../Lib/Types.h"

namespace engine
{
    namespace transform
    {
        class TransformComponent
        {
        public:
            TransformComponent();

            void Compose(TransformComponent* parent);
            void UpdateWorldMatrix();

            TransformComponent* parent() const;
            
            float3& position();
            float3& rotation();
            float3& scale();
            float4x4& world_matrix();

        private:
            TransformComponent* parent_ = nullptr;

            float3 position_;
            float3 rotation_;
            float3 scale_;
            float4x4 world_matrix_;
        };
    }
}
