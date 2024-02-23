#include "Square.h"

#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Basic::Shapes;

Square::Square(const float2& center, const float2& halfSize) : Rendering()
{
    pointsSquare_[0] = float4(center.x + halfSize.x, center.y + halfSize.y, 0.5f, 1.0f);
    pointsSquare_[1] = float4(1.0f, 0.0f, 0.0f, 1.0f);
    pointsSquare_[2] = float4(center.x - halfSize.x, center.y - halfSize.y, 0.5f, 1.0f);
    pointsSquare_[3] = float4(0.0f, 0.0f, 1.0f, 1.0f);
    pointsSquare_[4] = float4(center.x + halfSize.x, center.y - halfSize.y, 0.5f, 1.0f);
    pointsSquare_[5] = float4(0.0f, 1.0f, 0.0f, 1.0f);
    pointsSquare_[6] = float4(center.x - halfSize.x, center.y + halfSize.y, 0.5f, 1.0f);
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

    auto* device = pPipeline_->GetDevice();
    constexpr DXShaderMacros pixelShaderMacros[] = {
        "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr
    };

    CreateVertexShader(L"./Shaders/Shader.hlsl", nullptr, nullptr);
    CreatePixelShader(L"./Shaders/Shader.hlsl", pixelShaderMacros, nullptr);
    CreateLayout();
    CreateVertexBuffer(4 * 2);
    CreateIndexBuffer(3 * 2);
    CreateRasterizerState();

    pPipeline_->GetDeviceContext()->RSSetState(pRasterizerState_);
}
