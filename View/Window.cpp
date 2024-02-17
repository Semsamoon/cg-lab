#include "Window.h"

View::Window::BuilderWindowClass View::Window::New(LPCWSTR name)
{
	auto window = new Window();
	window->name_ = name;
	window->hInstance_ = GetModuleHandle(nullptr);
	if (window->hInstance_ == nullptr) exit(-1);
	return View::Window::BuilderWindowClass::New(*window);
}

View::Window::~Window()
{
	UnregisterClass(name_, hInstance_);
	DestroyWindow(hWindow_);
}

View::Window::BuilderWindowClass View::Window::BuilderWindowClass::New(Window& window)
{
	return View::Window::BuilderWindowClass(window);
}

View::Window::BuilderHWindow View::Window::BuilderWindowClass::WndClass() {
	window_.wndClass_.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_.wndClass_.lpfnWndProc = DefWindowProc;
	window_.wndClass_.cbClsExtra = 0;
	window_.wndClass_.cbWndExtra = 0;
	window_.wndClass_.hInstance = window_.hInstance_;
	window_.wndClass_.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	window_.wndClass_.hIconSm = window_.wndClass_.hIcon;
	window_.wndClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_.wndClass_.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	window_.wndClass_.lpszMenuName = nullptr;
	window_.wndClass_.lpszClassName = window_.name_;
	window_.wndClass_.cbSize = sizeof(WNDCLASSEX);
	if (RegisterClassEx(&window_.wndClass_) == 0) exit(-1);

	return View::Window::BuilderHWindow::New(window_);
}

View::Window::BuilderHWindow View::Window::BuilderHWindow::New(Window& window)
{
	return Window::BuilderHWindow(window);
}

View::Window::Builder View::Window::BuilderHWindow::HWindow(int32 width, int32 height)
{
	window_.width_ = width;
	window_.height_ = height;
	RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;
	auto posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	window_.hWindow_ = CreateWindowEx(WS_EX_APPWINDOW, window_.name_, window_.name_, dwStyle,
		posX, posY, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		nullptr, nullptr, window_.hInstance_, nullptr);

	return View::Window::Builder::New(window_);
}

View::Window::Builder View::Window::Builder::New(Window& window)
{
	return View::Window::Builder(window);
}

View::Window& View::Window::Builder::Build()
{
	ShowWindow(window_.hWindow_, SW_SHOW);
	SetForegroundWindow(window_.hWindow_);
	SetFocus(window_.hWindow_);
	ShowCursor(true);
	return window_;
}