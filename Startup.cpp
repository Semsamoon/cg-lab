#include <windows.h>

#include "Core/Game.h"

int main()
{
	{
		auto game = Core::Game::New();
		Sleep(2000);
	}
	Sleep(2000);
}