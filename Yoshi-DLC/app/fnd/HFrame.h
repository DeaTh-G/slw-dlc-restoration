#pragma once

namespace app
{
	namespace fnd
	{
		class HFrame
		{
			char Data[0x130];

		public:
			inline static FUNCTION_PTR(HFrame*, __thiscall, __ct, ASLR(0x0048E210), char* This);
			inline static FUNCTION_PTR(void, __thiscall, ResetFlag, ASLR(0x0048DCD0), int* This, int arg2);
			inline static FUNCTION_PTR(Vector3*, __thiscall, SetLocalTranslation, ASLR(0x0048DD30), int* This, Vector3* position);
			inline static FUNCTION_PTR(csl::math::Quaternion*, __thiscall, SetLocalRotation, ASLR(0x0048DD80), int* This, csl::math::Quaternion* rotation);
			inline static FUNCTION_PTR(void, __thiscall, AddChild, ASLR(0x0048E650), int* This, int* location);
		};
	}
}