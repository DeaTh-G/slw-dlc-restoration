#pragma once

namespace app
{
	class GOCEnemyTarget
	{
	public:
		inline static FUNCTION_PTR(void, __thiscall, Setup, ASLR(0x00726510), int* This, int* a2);
		inline static FUNCTION_PTR(void, __thiscall, SetColliderEyesight, ASLR(0x00726BC0), int* This, int a2);
		inline static FUNCTION_PTR(void, __thiscall, SetEyesightOffset, ASLR(0x00726580), int* This, csl::math::Vector3* offset);
	};
}