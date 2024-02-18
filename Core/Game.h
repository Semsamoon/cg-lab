#pragma once

#include "../View/Window.h"
#include "Input/Device.h"

namespace Core
{
	class Game final
	{
	public:
		Game(LPCWSTR name);
		~Game();

		void Run();

		View::Window& Window() { return *window_; }
		Core::Input::Device& InputDevice() { return *inputDevice_; }

	private:

		void Input();
		void FixedUpdate();
		void Update();
		void Render();

		View::Window* window_;
		Core::Input::Device* inputDevice_;
	};
}