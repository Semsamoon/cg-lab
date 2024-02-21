#include "Square.h"

#include <windows.h>
#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

View::Objects::Square::Square(Core::Math::Vector2F center, Core::Math::Vector2F halfSize, ID3D11RasterizerState* rs)
{
	pointsSquare_[0] = DirectX::XMFLOAT4(center.X + halfSize.X, center.Y + halfSize.Y, 0.5f, 1.0f);
	pointsSquare_[1] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	pointsSquare_[2] = DirectX::XMFLOAT4(center.X - halfSize.X, center.Y - halfSize.Y, 0.5f, 1.0f);
	pointsSquare_[3] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	pointsSquare_[4] = DirectX::XMFLOAT4(center.X + halfSize.X, center.Y - halfSize.Y, 0.5f, 1.0f);
	pointsSquare_[5] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	pointsSquare_[6] = DirectX::XMFLOAT4(center.X - halfSize.X, center.Y + halfSize.Y, 0.5f, 1.0f);
	pointsSquare_[7] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	points_ = pointsSquare_;

	indicesSquare_[0] = 0;
	indicesSquare_[1] = 1;
	indicesSquare_[2] = 2;
	indicesSquare_[3] = 1;
	indicesSquare_[4] = 0;
	indicesSquare_[5] = 3;
	indices_ = indicesSquare_;
	rasterizerState_ = rs;
}

void View::Objects::Square::Initialize(View::Render* render)
{
	render_ = render;
	auto& device = render_->DeviceDirect3D11();
	D3D_SHADER_MACRO Shader_Macros[] = {
		"TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr
	};

	CreateVertexShader(L"./Shaders/Shader.hlsl", nullptr, nullptr);
	CreatePixelShader(L"./Shaders/Shader.hlsl", Shader_Macros, nullptr);
	CreateLayout();
	CreateVertexBuffer(4 * 2);
	CreateIndexBuffer(3 * 2);
	//CreateRasterizerState();

	render_->DeviceDirect3D11Context().RSSetState(rasterizerState_);
}

void View::Objects::Square::Render()
{
	auto& context = render_->DeviceDirect3D11Context();
	context.RSSetState(rasterizerState_);

	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };

	context.IASetInputLayout(inputLayout_);
	context.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context.IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	context.IASetVertexBuffers(0, 1, &vertexBuffer_, strides, offsets);
	context.VSSetShader(vertexShader_, nullptr, 0);
	context.PSSetShader(pixelShader_, nullptr, 0);

	context.DrawIndexed(6, 0, 0);
	//render_->SetRenderTargets(0, true);
}