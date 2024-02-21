#pragma once

#include <windows.h>
#include <d3d11.h>
#include <directxmath.h>

#include "../../Lib/Types.h"
#include "../Renderable.h"

namespace View
{
	namespace Objects
	{
		class Figure : public Renderable
		{
		protected:
			void CreateVertexShader(LPCWSTR path, const D3D_SHADER_MACRO* defines, ID3DInclude* includes);
			void CreatePixelShader(LPCWSTR path, const D3D_SHADER_MACRO* defines, ID3DInclude* includes);
			void CreateLayout();
			void CreateVertexBuffer(uint32 pointsAmount);
			void CreateIndexBuffer(uint32 indicesAmount);
			void CreateRasterizerState();

			View::Render* render_;
			ID3DBlob* vertexShaderByteCode_;
			ID3DBlob* pixelShaderByteCode_;
			ID3D11VertexShader* vertexShader_;
			ID3D11PixelShader* pixelShader_;
			ID3D11InputLayout* inputLayout_;
			DirectX::XMFLOAT4* points_;
			int32* indices_;
			ID3D11Buffer* vertexBuffer_;
			ID3D11Buffer* indexBuffer_;
			ID3D11RasterizerState* rasterizerState_;
		};
	}
}