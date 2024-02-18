#pragma once

#include <unordered_set>
#include <windows.h>

#include "Keys.h"
#include "../Structs/Delegates.h"
#include "../../Lib/Types.h"
#include "../Math/Math.h"

namespace Core
{
	namespace Input
	{
		class Device
		{
		public:
			struct MouseEvent;

			Device(HWND& handlerWindow);
			~Device();

			Core::Structs::MulticastDelegate<const MouseEvent&> MouseMove;
			bool IsKeyDown(Keys key);

		protected:
			struct RawMouseEvent;
			struct RawKeyboardEvent;
			enum struct MouseFlags : int32;

			void OnMouseMove(RawMouseEvent args);
			void OnKeyDown(RawKeyboardEvent args);

			void AddPressedKey(Keys key);
			void RemovePressedKey(Keys key);

			std::unordered_set<Keys>* keys_;
			HWND& handlerWindow_;
		};

		struct Device::MouseEvent final
		{
		public:
			Math::Vector2F Position;
			Math::Vector2F Offset;
			float WheelDelta;
		};

		struct Device::RawKeyboardEvent final
		{
			uint16 MakeCode;
			uint16 Flags;
			uint16 VKey;
			uint32 Message;
		};

		struct Device::RawMouseEvent
		{
			int32 Mode;
			int32 ButtonFlags;
			int32 ExtraInformation;
			int32 Buttons;
			int32 WheelDelta;
			int32 X;
			int32 Y;
		};

		enum struct Device::MouseFlags : int32
		{
			LeftButtonDown = 1,
			LeftButtonUp = 1 << 1,
			RightButtonDown = 1 << 2,
			RightButtonUp = 1 << 3,
			MiddleButtonDown = 1 << 4,
			MiddleButtonUp = 1 << 5
		};
	}
}