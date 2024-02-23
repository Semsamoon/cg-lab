#include "Game.h"

#include <chrono>

#include "Lib/TimeAccumulator.h"
#include "Engine/Input/Keyboard.h"

using namespace Engine;

Game* Game::Instance()
{
    static auto* instance = new Game();
    return instance;
}

void Game::Compose(const char16* pName) const
{
    constexpr Point size = {800, 800};
    window_->Compose(pName, size);
    inputDevice_->Compose(window_->GetHandlerWindow());
    pipeline_->Compose(window_->GetHandlerWindow(), size);
}

void Game::Run()
{
    Lib::TimeAccumulator time;

    while (!isFinished_)
    {
        time.Update();
        Input();
        while (time.Accumulated() > 0.1)
        {
            FixedUpdate();
            time.Accumulated(time.Accumulated() - time.Delta());
        }
        Update();
        pipeline_->Render();
    }
}

void Game::Destroy() const
{
    delete pipeline_;
    delete inputDevice_;
    delete window_;
}

void Game::Input()
{
    MSG msg;

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT) isFinished_ = true;
    if (inputDevice_->IsKeyDown(Input::Keyboard::Keys::Escape)) isFinished_ = true;
}

void Game::FixedUpdate()
{
}

void Game::Update()
{
}
