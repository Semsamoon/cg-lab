#include "Game.h"
#include "Basic/Shapes/Square.h"

int main()
{
    auto* game = Game::Instance();
    auto* square1 = new Basic::Shapes::Square(float2(-0.5f, 0), float2(0.25f, 0.25f));
    auto* square2 = new Basic::Shapes::Square(float2(0.5f, 0), float2(0.25f, 0.25f));
    game->Compose(L"Game");
    game->GetRenderPipeline()->Add(square2);
    game->GetRenderPipeline()->Add(square1);
    game->Run();
}
