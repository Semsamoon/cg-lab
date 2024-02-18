#pragma once

#include "../../Lib/Types.h"

namespace Core
{
	namespace Math
	{
		struct Vector2F final
		{
		public:
			Vector2F() : X(0), Y(0) {}
			Vector2F(float x, float y) : X(x), Y(y) {}

			float X;
			float Y;
		};

		struct Vector2L final
		{
		public:
			Vector2L() : X(0), Y(0) {}
			Vector2L(int64 x, int64 y) : X(x), Y(y) {}

			int64 X;
			int64 Y;
		};
	}
}