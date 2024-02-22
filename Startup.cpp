#include <windows.h>
#include <iostream>

#include "Core/Game.h"
#include "View/Objects/Square.h"

int main()
{
	auto* game = new Core::Game(L"Game");
	auto* square1 = new View::Objects::Square(
		Core::Math::Vector2F(-0.5f, 0), Core::Math::Vector2F(0.5f, 0.5f));
	auto* square2 = new View::Objects::Square(
		Core::Math::Vector2F(0.5f, 0), Core::Math::Vector2F(0.5f, 0.5f));
	game->Add(square2);
	game->Add(square1);

	game->Run();
}