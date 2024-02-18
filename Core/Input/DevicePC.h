#pragma once

#include "Device.h"

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
		};
	}
}