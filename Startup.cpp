#include <windows.h>
#include <iostream>

#include "Core/Game.h"

int main()
{
	auto* game = new Core::Game(L"Game");
	game->Run();
}