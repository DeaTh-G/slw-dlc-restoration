#pragma once

namespace app
{
	namespace game
	{
		class MoveBound
		{
		public:
			class Listener
			{
			public:
				virtual void OnBound(csl::math::Plane& const a1) {}
			};
		};
	}
}