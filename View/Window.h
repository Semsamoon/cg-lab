#pragma once

#include <windows.h>
#include <WinUser.h>

#include "../Data/Typedefs.h"

namespace View
{
	class Window final
	{
	public:
		class BuilderWindowClass;
		class BuilderHWindow;
		class Builder;

		static BuilderWindowClass New(LPCWSTR name);
		~Window();

	private:
		Window() {}

		LPCWSTR name_;
		HINSTANCE hInstance_;
		WNDCLASSEX wndClass_;
		HWND hWindow_;
		int32 width_;
		int32 height_;
	};

	class Window::BuilderWindowClass final
	{
	public:
		static BuilderWindowClass New(Window& window);
		BuilderHWindow WndClass();

	private:
		BuilderWindowClass(Window& window) : window_(window) {}
		Window& window_;
	};

	class Window::BuilderHWindow final
	{
	public:
		static BuilderHWindow New(Window& window);
		Builder HWindow(int32 width, int32 height);

	private:
		BuilderHWindow(Window& window) : window_(window) {}
		Window& window_;
	};

	class Window::Builder final
	{
	public:
		static Builder New(Window& window);
		Window& Build();

	private:
		Builder(Window& window) : window_(window) {}
		Window& window_;
	};
}