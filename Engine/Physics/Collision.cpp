#include "Collision.h"

#include "../Update/FixedUpdate.h"

using namespace engine::physics;

void Collision::FixedUpdate()
{
    for (auto i = 0; i < collision_ables_.size(); i++)
        for (auto j = i + 1; j < collision_ables_.size(); j++)
            DetectCollision(collision_ables_[i], collision_ables_[j]);
}

void Collision::Add(CollisionAble* collision_able)
{
    collision_ables_.push_back(collision_able);
    collision_able->Compose(this);
}

void Collision::DetectCollision(CollisionAble* collision_able_1, CollisionAble* collision_able_2)
{
    if (!collision_able_1->bounding_box().Intersects(collision_able_2->bounding_box())) return;
    collision_able_1->Collided(collision_able_2);
    collision_able_2->Collided(collision_able_1);
}
