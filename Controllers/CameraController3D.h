#pragma once
#include "CameraController.h"

namespace controllers
{
    class CameraControllerRadial;

    class CameraController3D final : public CameraController
    {
    public:
        using CameraController::Compose;

        void Compose(CameraControllerRadial* radial);

        void Update(float delta) override;

    protected:
        void OnMouseEvent(const engine::input::mouse::Event& args) override;
        void OnKeyboardEvent(const engine::input::keyboard::Event& args) override;

    private:
        CameraControllerRadial* radial_ = nullptr;
    };
}
