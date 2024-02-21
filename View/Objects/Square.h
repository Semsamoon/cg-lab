#pragma once

#include "Figure.h"
#include "../../Core/Math/Math.h"

namespace View
{
	namespace Objects
	{
		class Square final : public Figure
		{
		public:
			Square(Core::Math::Vector2F center, Core::Math::Vector2F size, ID3D11RasterizerState* rs);

			virtual void Initialize(View::Render* render) override;
			virtual void Render() override;

		private:
			DirectX::XMFLOAT4 pointsSquare_[4 * 2];
			int32 indicesSquare_[3 * 2];
		};
	}
}