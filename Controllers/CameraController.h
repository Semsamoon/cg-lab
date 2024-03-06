#pragma once
#include "../Engine/Input/Device.h"
#include "../Engine/Update/VariableUpdateAble.h"
#include "../Objects/CameraObject.h"

namespace controllers
{
    class CameraController final : public engine::update::VariableUpdateAble
    {
    public:
        void Compose(objects::CameraObject* camera_object, engine::input::Device* device);

        void Update(float delta) override;

    private:
        void OnMouseEvent(const engine::input::mouse::Event& args) const;
        void OnKeyboardEvent(const engine::input::keyboard::Event& args) const;

        objects::CameraObject* camera_object_ = nullptr;
        engine::input::Device* device_ = nullptr;
    };
}
