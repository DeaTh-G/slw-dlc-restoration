#pragma once

namespace app
{
	class CSetAdapter
	{
	public:
		inline static FUNCTION_PTR(int*, __thiscall, GetData, ASLR(0x008408F0), int* This);
		inline static FUNCTION_PTR(int*, __thiscall, GetRotation, ASLR(0x008407F0), int* This, csl::math::Quaternion* rotation);
		inline static FUNCTION_PTR(int*, __thiscall, GetPosition, ASLR(0x008407C0), int* This, Vector3* position);
		inline static FUNCTION_PTR(int, __thiscall, GetUnitID, ASLR(0x00840860), int* This);
	};
}