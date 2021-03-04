#pragma once

namespace app
{
	namespace fnd
	{
		class GOCTransform
		{
		public:
			inline static FUNCTION_PTR(void, __thiscall, SetLocalTranslation, ASLR(0x00494440), int* This, Vector3* position);
			inline static FUNCTION_PTR(void, __thiscall, SetLocalRotation, ASLR(0x00494470), int* This, csl::math::Quaternion* position);
		};
	}
}