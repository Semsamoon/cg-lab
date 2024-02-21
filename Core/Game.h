#pragma once

#include <vector>

#include "../View/Renderable.h"
#include "../View/Window.h"
#include "Input/Device.h"
#include "../View/Render.h"

namespace Core
{
	class Game final
	{
	public:
		Game(LPCWSTR name);
		~Game();

		void Add(View::Renderable* renderable);
		void Run();

		View::Window* Window() { return window_; }
		Core::Input::Device* InputDevice() { return inputDevice_; }
		View::Render* Render() { return render_; }

	private:
		void Initialize();
		void Input();
		void FixedUpdate();
		void Update();
		void RenderUpdate();

		View::Window* window_;
		Core::Input::Device* inputDevice_;
		View::Render* render_;

		std::vector<View::Renderable*> renderables_;

		bool isFinished_;
	};
}