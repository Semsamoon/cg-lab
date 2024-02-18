#include <windows.h>
#include <iostream>

#include "Core/Game.h"

int main()
{
	auto* game = new Core::Game(L"Game");
	auto& input = game->InputDevice();
	bool isExitRequested = false;
	MSG msg = {};

	while (!isExitRequested) {
		// Handle the windows messages.
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			isExitRequested = true;
		}

		if (input.IsKeyDown(Core::Input::Keys::Escape)) {
			isExitRequested = true;
		}
	}
}