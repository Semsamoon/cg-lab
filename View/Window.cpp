#include "Window.h"

View::Window::Window(
	const LPCWSTR const name,
	LRESULT(*windowProcedure)(HWND hWindow, UINT umessage, WPARAM wparam, LPARAM lparam),
	int32 width, int32 height)
	: name_(name), handlerInstance_(GetModuleHandle(nullptr)), width_(width), height_(height)
{
	if (handlerInstance_ == nullptr) exit(-1);
	InitializeWindowClass(windowProcedure);
	InitializeHandlerWindow();
	Show();
}

View::Window::~Window()
{
	DestroyWindow(handlerWindow_);
	UnregisterClass(name_, handlerInstance_);
}

void View::Window::InitializeWindowClass(
	LRESULT(*windowProcedure) (HWND hWindow, UINT umessage, WPARAM wparam, LPARAM lparam)) {
	windowClass_.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass_.lpfnWndProc = windowProcedure;
	windowClass_.cbClsExtra = 0;
	windowClass_.cbWndExtra = 0;
	windowClass_.hInstance = handlerInstance_;
	windowClass_.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	windowClass_.hIconSm = windowClass_.hIcon;
	windowClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass_.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	windowClass_.lpszMenuName = nullptr;
	windowClass_.lpszClassName = name_;
	windowClass_.cbSize = sizeof(WNDCLASSEX);
	if (RegisterClassEx(&windowClass_) == 0) exit(-2);
}

void View::Window::InitializeHandlerWindow()
{
	RECT windowRect = { 0, 0, width_, height_ };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;
	auto posX = (GetSystemMetrics(SM_CXSCREEN) - width_) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - height_) / 2;
	handlerWindow_ = CreateWindowEx(
		WS_EX_APPWINDOW, name_, name_, dwStyle,
		posX, posY, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		nullptr, nullptr, handlerInstance_, nullptr);
}

void View::Window::Show()
{
	ShowWindow(handlerWindow_, SW_SHOW);
	SetForegroundWindow(handlerWindow_);
	SetFocus(handlerWindow_);
	ShowCursor(true);
}