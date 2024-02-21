#pragma once

#include <wrl.h>
#include <d3d11.h>

#include "../Lib/Types.h"

#pragma comment(lib, "d3d11.lib")

namespace View
{
	class Render final
	{
	public:
		Render(uint32 width, uint32 height, HWND handlerWindow);

		void PreRenderUpdate();
		void PostRenderUpdate();

		void SetRenderTargets(uint32 numViews, bool isNull);

		Microsoft::WRL::ComPtr<ID3D11Device>& DeviceDirect3D11() { return deviceDirect3D11_; }
		ID3D11DeviceContext& DeviceDirect3D11Context() { return *deviceDirect3D11Context_; }
		IDXGISwapChain& SwapChain() { return *swapChain_; }
		ID3D11RenderTargetView& RenderTargetView() { return *renderTargetView_; }

	private:
		void InitializeDeviceAndSwapChain(uint32 width, uint32 height, HWND handlerWindow);
		void InitializeRenderTargetView();

		Microsoft::WRL::ComPtr<ID3D11Device> deviceDirect3D11_;
		ID3D11DeviceContext* deviceDirect3D11Context_;
		IDXGISwapChain* swapChain_;
		ID3D11RenderTargetView* renderTargetView_;
	};
}