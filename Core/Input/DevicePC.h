#pragma once

#include "Device.h"
#include "../../Lib/Types.h"

namespace Core
{
	namespace Input
	{
		class DevicePC final : public Device
		{
		public:
			DevicePC(HWND& handlerWindow);

			static LRESULT CALLBACK InputWindowProcedure(HWND hWindow, UINT umessage, WPARAM wparam, LPARAM lparam);

		private:
			void InitializeMouse(RAWINPUTDEVICE* device);
			void InitializeKeyboard(RAWINPUTDEVICE* device);

			static uint32 SizeRawInput(LPARAM lparam);
			static RAWINPUT* ReadRawInput(uint32 size, LPARAM lparam);
			static void ProcessKeyboard(RAWINPUT* rawInput, DevicePC* device);
			static void ProcessMouse(RAWINPUT* rawInput, DevicePC* device);
		};
	}
}