#pragma once

#define M_PI 3.1415927f

namespace app
{
	namespace egg
	{
		inline static int GetMaxLocusPositionNum() { return 0x79; }

		inline static float CalcSlipperyRatio(float a1, float a2) { return math::Clamp(sinf(((3.1415927f + 3.1415927f) / a2) * a1), -1, 1); }

		inline static csl::math::Vector3 CalcSlipperyScale(float time, float a3, float a4, float a5)
		{
			float slipperyRatio = fabs(CalcSlipperyRatio(time, a3));
			float y = -((a4 * slipperyRatio) - 1);
			float x = (a5 * slipperyRatio) + 1;

			return csl::math::Vector3(x, y, 1);
		}
	};
}