#pragma once
#include <vector>

#include "CollisionAble.h"
#include "../Update/FixedUpdateAble.h"

namespace engine
{
    namespace physics
    {
        class Collision final : public update::FixedUpdateAble
        {
        public:
            void FixedUpdate() override;

            void Add(CollisionAble* collision_able);

        private:
            static void DetectCollision(CollisionAble* collision_able_1, CollisionAble* collision_able_2);

            std::vector<CollisionAble*> collision_ables_{};
        };
    }
}
