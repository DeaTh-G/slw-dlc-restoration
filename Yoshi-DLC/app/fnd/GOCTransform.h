#pragma once

namespace app
{
	namespace fnd
	{
		class GOCTransform
		{
		public:
			int Data[0x78]{};

			inline static FUNCTION_PTR(void, __thiscall, SetLocalTranslation, ASLR(0x00494440), int* This, csl::math::Vector3* position);
			inline static FUNCTION_PTR(void, __thiscall, SetLocalRotation, ASLR(0x00494470), int* This, csl::math::Quaternion* position);
			inline static FUNCTION_PTR(bool, __thiscall, IsExistParent, ASLR(0x00494240), GOCTransform* This);
			inline static FUNCTION_PTR(char, __thiscall, SetInheriteFlags, ASLR(0x00494250), GOCTransform* This, char a2);
		};
	}
}