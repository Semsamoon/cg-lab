#include "Game.h"

#include <chrono>

#include "Structs/TimeAccumulator.h"
#include "Input/DevicePC.h"
#include "../View/Window.h"

Core::Game::Game(LPCWSTR name) : isFinished_(false)
{
	window_ = new View::Window(name, Input::DevicePC::InputWindowProcedure, 800, 800);
	inputDevice_ = new Input::DevicePC(window_->HandlerWindow());
	render_ = new View::Render(800, 800, window_->HandlerWindow());
	renderables_ = std::vector<View::Renderable*>();
}

Core::Game::~Game()
{
	delete window_;
	delete inputDevice_;
	renderables_.clear();
}

void Core::Game::Add(View::Renderable* renderable)
{
	renderables_.push_back(renderable);
}

void Core::Game::Run()
{
	Structs::TimeAccumulator time;
	Initialize();

	while (!isFinished_) {
		time.Update();
		Input();
		while (time.Accumulated() > 0.1) {
			FixedUpdate();
			time.Accumulated(time.Accumulated() - time.Delta());
		}
		Update();
		RenderUpdate();
	}
}

void Core::Game::Initialize()
{
	for (auto* renderable : renderables_) renderable->Initialize(render_);
}

void Core::Game::Input()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT) {
		isFinished_ = true;
	}

	if (inputDevice_->IsKeyDown(Core::Input::Keys::Escape)) {
		isFinished_ = true;
	}
}

void Core::Game::FixedUpdate()
{
}

void Core::Game::Update()
{
}

void Core::Game::RenderUpdate()
{
	render_->PreRenderUpdate();
	for (auto* renderable : renderables_) renderable->Render();
	render_->PostRenderUpdate();
}