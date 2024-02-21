#include <windows.h>
#include <iostream>

#include "Core/Game.h"
#include "View/Objects/Square.h"

int main()
{
	auto* game = new Core::Game(L"Game");

	ID3D11RasterizerState* rs1;
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	game->Render()->DeviceDirect3D11()->CreateRasterizerState(
		&rastDesc, &rs1);

	ID3D11RasterizerState* rs2;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	game->Render()->DeviceDirect3D11()->CreateRasterizerState(
		&rastDesc, &rs2);


	auto* square1 = new View::Objects::Square(
		Core::Math::Vector2F(-0.5f, 0), Core::Math::Vector2F(0.5f, 0.5f), rs1);
	auto* square2 = new View::Objects::Square(
		Core::Math::Vector2F(0.5f, 0), Core::Math::Vector2F(0.5f, 0.5f), rs2);
	game->Add(square2);
	game->Add(square1);

	game->Run();
}