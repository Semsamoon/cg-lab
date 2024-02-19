#include "Device.h"

#include "../Math/Math.h"

Core::Input::Device::Device(HWND& handlerWindow) : handlerWindow_(handlerWindow)
{
	keys_ = new std::unordered_set<Keys>();
}

Core::Input::Device::~Device()
{
	delete keys_;
}

bool Core::Input::Device::IsKeyDown(Keys key)
{
	return keys_->count(key) != 0;
}

void Core::Input::Device::OnMouseMove(RawMouseEvent args)
{
	if (args.ButtonFlags & static_cast<int32>(MouseFlags::LeftButtonDown)) AddPressedKey(Keys::LeftButton);
	if (args.ButtonFlags & static_cast<int32>(MouseFlags::LeftButtonUp)) RemovePressedKey(Keys::LeftButton);
	if (args.ButtonFlags & static_cast<int32>(MouseFlags::RightButtonDown)) AddPressedKey(Keys::RightButton);
	if (args.ButtonFlags & static_cast<int32>(MouseFlags::RightButtonUp)) RemovePressedKey(Keys::RightButton);
	if (args.ButtonFlags & static_cast<int32>(MouseFlags::MiddleButtonDown)) AddPressedKey(Keys::MiddleButton);
	if (args.ButtonFlags & static_cast<int32>(MouseFlags::MiddleButtonUp)) RemovePressedKey(Keys::MiddleButton);

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(handlerWindow_, &p);

	MouseEvent mouseArgs;
	mouseArgs.Position = Core::Math::Vector2F(p.x, p.y);
	mouseArgs.Offset = Core::Math::Vector2F(args.X, args.Y);
	mouseArgs.WheelDelta = args.WheelDelta;

	MouseMove.Broadcast(mouseArgs);
}

void Core::Input::Device::OnKeyDown(RawKeyboardEvent args)
{
	auto key = static_cast<Keys>(args.VKey);

	if (args.MakeCode == 42) key = Keys::LeftShift;
	if (args.MakeCode == 54) key = Keys::RightShift;

	if (args.Flags & 0x01) {
		if (keys_->count(key)) RemovePressedKey(key);
	}
	else if (!keys_->count(key)) AddPressedKey(key);
}

void Core::Input::Device::AddPressedKey(Keys key)
{
	keys_->insert(key);
}

void Core::Input::Device::RemovePressedKey(Keys key)
{
	keys_->erase(key);
}