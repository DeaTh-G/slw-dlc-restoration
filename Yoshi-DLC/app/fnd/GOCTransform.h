#pragma once

namespace app
{
	namespace fnd
	{
		class GOCTransform
		{
		public:
			char Data[0x1E0];

			inline static FUNCTION_PTR(void, __thiscall, SetLocalTranslation, ASLR(0x00494440), int* This, csl::math::Vector3* position);
			inline static FUNCTION_PTR(void, __thiscall, SetLocalRotation, ASLR(0x00494470), int* This, csl::math::Quaternion* position);
		};
	}
}