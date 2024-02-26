#include "Square.h"

#include <d3dcompiler.h>

#include "../../Engine/Render/Pipeline.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace Basic::Shapes;

void Square::Compose(const float2& center, const float2& size)
{
    pointsSquare_[0] = float4(center.x + size.x, center.y + size.y, 0.5f, 1.0f);
    pointsSquare_[1] = float4(1.0f, 0.0f, 0.0f, 1.0f);
    pointsSquare_[2] = float4(center.x - size.x, center.y - size.y, 0.5f, 1.0f);
    pointsSquare_[3] = float4(0.0f, 0.0f, 1.0f, 1.0f);
    pointsSquare_[4] = float4(center.x + size.x, center.y - size.y, 0.5f, 1.0f);
    pointsSquare_[5] = float4(0.0f, 1.0f, 0.0f, 1.0f);
    pointsSquare_[6] = float4(center.x - size.x, center.y + size.y, 0.5f, 1.0f);
    pointsSquare_[7] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    pPoints_ = pointsSquare_;

    indicesSquare_[0] = 0;
    indicesSquare_[1] = 1;
    indicesSquare_[2] = 2;
    indicesSquare_[3] = 1;
    indicesSquare_[4] = 0;
    indicesSquare_[5] = 3;
    pIndices_ = indicesSquare_;

    indicesAmount_ = 6;
}

void Square::Compose(Engine::Render::Pipeline* pPipeline)
{
    Rendering::Compose(pPipeline);

    CreateVertexShader(L"./Shaders/Shader.hlsl", nullptr, nullptr);
    CreatePixelShader(L"./Shaders/Shader.hlsl", nullptr, nullptr);
    CreateLayout();
    CreateVertexBuffer(4 * 2);
    CreateIndexBuffer(3 * 2);
    CreateRasterizerState();

    pPipeline_->GetDeviceContext()->RSSetState(pRasterizerState_);
}
