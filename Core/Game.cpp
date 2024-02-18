#include "Game.h"

#include <chrono>

#include "Structs/TimeAccumulator.h"
#include "Input/DevicePC.h"

Core::Game::Game(LPCWSTR name)
{
	window_ = new View::Window(name, Input::DevicePC::InputWindowProcedure, 800, 800);
	inputDevice_ = new Input::DevicePC(window_->HandlerWindow());
}

Core::Game::~Game()
{
	delete window_;
	delete inputDevice_;
}

void Core::Game::Run()
{
	Structs::TimeAccumulator time;

	while (true) {
		time.Update();
		Input();
		while (time.Accumulated() > 0.1) {
			FixedUpdate();
			time.Accumulated(time.Accumulated() - time.Delta());
		}
		Update();
		Render();
	}
}

void Core::Game::Input()
{
}

void Core::Game::FixedUpdate()
{
}

void Core::Game::Update()
{
}

void Core::Game::Render()
{
}