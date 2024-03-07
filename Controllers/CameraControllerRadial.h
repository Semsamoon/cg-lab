#pragma once
#include "CameraController.h"
#include "../Engine/Input/Device.h"
#include "../Engine/Physics/Collision.h"
#include "../Engine/Transform/TransformComponent.h"
#include "../Lib/Types.h"

namespace controllers
{
    class CameraController3D;

    class CameraControllerRadial final : public CameraController
    {
    public:
        using CameraController::Compose;

        void Compose(CameraController3D* camera_3d);
        void Compose(engine::physics::Collision* collision);
        void SetFollowing();

        void Update(float delta) override;

    protected:
        void OnMouseEvent(const engine::input::mouse::Event& args) override;
        void OnKeyboardEvent(const engine::input::keyboard::Event& args) override;

    private:
        CameraController3D* camera_3d_ = nullptr;
        engine::physics::Collision* collision_ = nullptr;

        engine::transform::TransformComponent* following_ = nullptr;
        float3 center_;
        float distance_ = 0;
        float3 rotation_;
    };
}
