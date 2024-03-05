#pragma once

#include "../../Lib/Types.h"

namespace engine
{
    namespace physics
    {
        class Collision;

        class CollisionAble
        {
        public:
            virtual ~CollisionAble() = default;
            virtual void Compose(Collision* collision);
            virtual void Collided(CollisionAble* other);

            virtual DXBox& bounding_box() = 0;
        };
    }
}
