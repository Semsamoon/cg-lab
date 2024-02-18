#include "DevicePC.h"

#include "../../Lib/Types.h"

Core::Input::DevicePC::DevicePC(HWND& handlerWindow) : Device(handlerWindow)
{
	RAWINPUTDEVICE devices[2];
	InitializeMouse(&devices[0]);
	InitializeKeyboard(&devices[1]);
	if (RegisterRawInputDevices(devices, 2, sizeof(devices[0])) == 0) exit(-1);
	SetWindowLongPtr(handlerWindow_, GWLP_USERDATA, (int64)this);
}

LRESULT CALLBACK Core::Input::DevicePC::InputWindowProcedure(
	HWND handlerWindow, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_INPUT:
	{
		uint32 dwSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT,
			nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		if (dwSize == 0) return DefWindowProc(handlerWindow, umessage, wparam, lparam);

		auto* inputs = new uint8[dwSize];
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT,
			inputs, &dwSize, sizeof(RAWINPUTHEADER));
		RAWINPUT* rawInput = reinterpret_cast<RAWINPUT*>(inputs);
		auto* device = (DevicePC*)GetWindowLongPtr(handlerWindow, GWLP_USERDATA);

		if (rawInput->header.dwType == RIM_TYPEKEYBOARD)
		{
			RawKeyboardEvent args;
			args.MakeCode = rawInput->data.keyboard.MakeCode;
			args.Flags = rawInput->data.keyboard.Flags;
			args.VKey = rawInput->data.keyboard.VKey;
			args.Message = rawInput->data.keyboard.Message;
			device->OnKeyDown(args);
		}
		else if (rawInput->header.dwType == RIM_TYPEMOUSE)
		{
			RawMouseEvent args;
			args.Mode = rawInput->data.mouse.usFlags;
			args.ButtonFlags = rawInput->data.mouse.usButtonFlags;
			args.ExtraInformation = static_cast<int32>(rawInput->data.mouse.ulExtraInformation);
			args.Buttons = static_cast<int32>(rawInput->data.mouse.ulRawButtons);
			args.WheelDelta = static_cast<int32>(rawInput->data.mouse.usButtonData);
			args.X = rawInput->data.mouse.lLastX;
			args.Y = rawInput->data.mouse.lLastY;
			device->OnMouseMove(args);
		}

		delete[] inputs;
		return DefWindowProc(handlerWindow, umessage, wparam, lparam);
	}
	default:
		return DefWindowProc(handlerWindow, umessage, wparam, lparam);
	}
}

void Core::Input::DevicePC::InitializeMouse(RAWINPUTDEVICE* device)
{
	device->usUsagePage = 1;
	device->usUsage = 2;
	device->dwFlags = 0;
	device->hwndTarget = handlerWindow_;
}

void Core::Input::DevicePC::InitializeKeyboard(RAWINPUTDEVICE* device)
{
	device->usUsagePage = 1;
	device->usUsage = 6;
	device->dwFlags = 0;
	device->hwndTarget = handlerWindow_;
}