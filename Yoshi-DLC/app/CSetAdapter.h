#pragma once

namespace app
{
	class CSetAdapter
	{
	public:
		inline static FUNCTION_PTR(int*, __thiscall, GetData, ASLR(0x008408F0), int* This);
		inline static FUNCTION_PTR(int*, __thiscall, GetRotation, ASLR(0x008407F0), int* This, Quaternion* rotation);
		inline static FUNCTION_PTR(int*, __thiscall, GetPosition, ASLR(0x008407C0), int* This, Vector3* position);
	};
}