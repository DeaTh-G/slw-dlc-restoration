#pragma once

namespace app
{
	namespace math
	{
		class CalculatedTransform
		{
		public:
			inline static FUNCTION_PTR(int, __thiscall, GetTranslation, ASLR(0x005321C0), csl::math::Matrix34* transformMatrix, csl::math::Vector3* translation);
		};
	}
}