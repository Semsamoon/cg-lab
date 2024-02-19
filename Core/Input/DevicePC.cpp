#include "DevicePC.h"

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
		auto size = SizeRawInput(lparam);
		if (size == 0) return DefWindowProc(handlerWindow, umessage, wparam, lparam);
		auto* rawInput = ReadRawInput(size, lparam);
		auto* device = (DevicePC*)GetWindowLongPtr(handlerWindow, GWLP_USERDATA);
		if (rawInput->header.dwType == RIM_TYPEKEYBOARD) ProcessKeyboard(rawInput, device);
		else if (rawInput->header.dwType == RIM_TYPEMOUSE) ProcessMouse(rawInput, device);
		delete rawInput;
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

uint32 Core::Input::DevicePC::SizeRawInput(LPARAM lparam)
{
	uint32 dwSize = 0;
	GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT,
		nullptr, &dwSize, sizeof(RAWINPUTHEADER));
	return dwSize;
}

RAWINPUT* Core::Input::DevicePC::ReadRawInput(uint32 size, LPARAM lparam)
{
	auto* inputs = new uint8[size];
	GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT,
		inputs, &size, sizeof(RAWINPUTHEADER));
	return reinterpret_cast<RAWINPUT*>(inputs);
}

void Core::Input::DevicePC::ProcessKeyboard(RAWINPUT* rawInput, DevicePC* device)
{
	RawKeyboardEvent args;
	args.MakeCode = rawInput->data.keyboard.MakeCode;
	args.Flags = rawInput->data.keyboard.Flags;
	args.VKey = rawInput->data.keyboard.VKey;
	args.Message = rawInput->data.keyboard.Message;
	device->OnKeyDown(args);
}

void Core::Input::DevicePC::ProcessMouse(RAWINPUT* rawInput, DevicePC* device)
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