#pragma once

#include "Engine/Render/Pipeline.h"
#include "Engine/Input/PC.h"
#include "Engine/Render/Window.h"

class Game final
{
public:
    Game()
        : window_(new Engine::Render::Window()), pipeline_(new Engine::Render::Pipeline()),
          inputDevice_(new Engine::Input::PC()), isFinished_(false)
    {
    }

    static Game* Instance();

    void Compose(const char16* pName) const;
    void Run();
    void Destroy() const;

    Engine::Render::Window* GetWindow() const { return window_; }
    Engine::Render::Pipeline* GetRenderPipeline() const { return pipeline_; }
    Engine::Input::Device* GetInputDevice() const { return inputDevice_; }

private:
    void Input();
    void FixedUpdate();
    void Update();

    Engine::Render::Window* window_;
    Engine::Render::Pipeline* pipeline_;
    Engine::Input::Device* inputDevice_;

    bool isFinished_;
};
