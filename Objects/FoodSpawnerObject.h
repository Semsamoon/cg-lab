#pragma once
#include <string>
#include <vector>

#include "FoodObject.h"
#include "Types.h"
#include "../Engine/Graphics/RenderAble.h"

namespace objects
{
    class FoodSpawnerObject final : public engine::update::VariableUpdateAble
    {
    public:
        void Compose(std::string file_paths[], float probabilities[], float2 area, int32 amount,
                     engine::transform::TransformComponent* camera);
        void Compose(engine::graphics::RenderPipeline* pipeline) const;
        void Compose(engine::physics::Collision* collision);

        void Update(float delta) override;

    private:
        std::vector<FoodObject> food_objects_{};
    };
}
