#include "Render.h"

View::Render::Render(uint32 width, uint32 height, HWND handlerWindow)
{
	InitializeDeviceAndSwapChain(width, height, handlerWindow);
	InitializeRenderTargetView();
}

void View::Render::PreRenderUpdate()
{
	deviceDirect3D11Context_->ClearState();
	deviceDirect3D11Context_->OMSetRenderTargets(
		1, &renderTargetView_, nullptr);
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deviceDirect3D11Context_->ClearRenderTargetView(renderTargetView_, color);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(800);
	viewport.Height = static_cast<float>(800);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	deviceDirect3D11Context_->RSSetViewports(1, &viewport);
}

void View::Render::PostRenderUpdate()
{
	swapChain_->Present(1, DXGI_PRESENT_DO_NOT_WAIT);
}

void View::Render::SetRenderTargets(uint32 numViews, bool isNull)
{
	deviceDirect3D11Context_->OMSetRenderTargets(
		numViews, isNull ? nullptr : &renderTargetView_, nullptr);
}

void View::Render::InitializeDeviceAndSwapChain(uint32 width, uint32 height, HWND handlerWindow)
{
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = handlerWindow;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
		featureLevel, 1, D3D11_SDK_VERSION, &swapDesc,
		&swapChain_, &deviceDirect3D11_, nullptr, &deviceDirect3D11Context_);
}

void View::Render::InitializeRenderTargetView()
{
	ID3D11Texture2D* backTex;
	swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);
	deviceDirect3D11_->CreateRenderTargetView(backTex, nullptr, &renderTargetView_);
}