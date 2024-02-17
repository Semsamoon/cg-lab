#include "Game.h"

#include <chrono>

#include "Stopwatch.h"

Core::Game Core::Game::New()
{
	auto& window = View::Window::New(L"Game").WndClass().HWindow(800, 800).Build();
	return Game(window);
}

Core::Game::~Game()
{
	delete &window_;
}

void Core::Game::Run()
{
	auto stopwatch = Stopwatch::New();

	while (true) {
		stopwatch.Update();
		Input();
		while (stopwatch.CanDecreaseLag(0.1)) FixedUpdate();
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