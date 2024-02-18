#pragma once

#include <windows.h>
#include <WinUser.h>

#include "../Lib/Types.h"

namespace View
{
	class Window final
	{
	public:
		Window(
			const LPCWSTR name,
			LRESULT(*windowProcedure) (HWND hWindow, UINT umessage, WPARAM wparam, LPARAM lparam),
			int32 width, int32 height);
		~Window();

		LPCWSTR& Name() { return name_; }
		HINSTANCE& HandlerInstance() { return handlerInstance_; }
		WNDCLASSEX& WindowClass() { return windowClass_; }
		int32 Width() const { return width_; }
		int32 Height() const { return height_; }
		HWND& HandlerWindow() { return handlerWindow_; }

	private:
		void InitializeWindowClass(
			LRESULT(*windowProcedure) (HWND hWindow, UINT umessage, WPARAM wparam, LPARAM lparam));
		void InitializeHandlerWindow();
		void Show();

		LPCWSTR name_;
		HINSTANCE handlerInstance_;
		WNDCLASSEX windowClass_;
		HWND handlerWindow_;
		int32 width_;
		int32 height_;
	};
}