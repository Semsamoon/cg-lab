﻿#pragma once

#include "Stick.h"

namespace Pong
{
    class StickPlayer final : public Stick
    {
    public:
        void Compose(const float2& center, const float2& size, Side side, float speed,
            Engine::Input::Device* pInputDevice);

        void FixedUpdate() override;

    private:
        Engine::Input::Device* pInputDevice_{};
    };
}
