#include "Figure.h"

#include <d3dcompiler.h>

void View::Objects::Figure::CreateVertexShader(
	LPCWSTR path, const D3D_SHADER_MACRO* defines, ID3DInclude* includes)
{
	ID3DBlob* error;
	D3DCompileFromFile(
		path, defines, includes, "VSMain", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
		&vertexShaderByteCode_, &error);
	render_->DeviceDirect3D11()->CreateVertexShader(
		vertexShaderByteCode_->GetBufferPointer(),
		vertexShaderByteCode_->GetBufferSize(),
		nullptr, &vertexShader_);
}

void View::Objects::Figure::CreatePixelShader(
	LPCWSTR path, const D3D_SHADER_MACRO* defines, ID3DInclude* includes)
{
	ID3DBlob* error;
	D3DCompileFromFile(
		path, defines, includes, "PSMain", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
		&pixelShaderByteCode_, &error);
	render_->DeviceDirect3D11()->CreatePixelShader(
		pixelShaderByteCode_->GetBufferPointer(),
		pixelShaderByteCode_->GetBufferSize(),
		nullptr, &pixelShader_);
}

void View::Objects::Figure::CreateLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
	},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
	}
	};

	render_->DeviceDirect3D11()->CreateInputLayout(
		inputElements, 2,
		vertexShaderByteCode_->GetBufferPointer(),
		vertexShaderByteCode_->GetBufferSize(), &inputLayout_);
}

void View::Objects::Figure::CreateVertexBuffer(uint32 pointsAmount)
{
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * pointsAmount;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points_;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	render_->DeviceDirect3D11()->CreateBuffer(
		&vertexBufDesc, &vertexData, &vertexBuffer_);
}

void View::Objects::Figure::CreateIndexBuffer(uint32 indicesAmount)
{
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int32) * indicesAmount;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices_;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	render_->DeviceDirect3D11()->CreateBuffer(
		&indexBufDesc, &indexData, &indexBuffer_);
}

void View::Objects::Figure::CreateRasterizerState()
{
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;

	render_->DeviceDirect3D11()->CreateRasterizerState(
		&rastDesc, &rasterizerState_);
}