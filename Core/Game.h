#pragma once

#include "../View/Window.h"

namespace Core
{
	class Game final
	{
	public:
		static Game New();
		~Game();

		void Run();

	private:
		Game(View::Window& window) : window_(window) {}

		void Input();
		void FixedUpdate();
		void Update();
		void Render();

		View::Window& window_;
	};
}