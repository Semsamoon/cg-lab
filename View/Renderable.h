#pragma once

#include "Render.h"

namespace View
{
	class Renderable
	{
	public:
		virtual void Initialize(View::Render* render) = 0;
		virtual void Render() = 0;
	};
}