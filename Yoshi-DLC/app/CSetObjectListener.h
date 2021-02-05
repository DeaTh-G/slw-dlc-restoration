#pragma once

namespace app
{
	class CSetObjectListener
	{
	public:
		inline static FUNCTION_PTR(void, __thiscall, SetStatusRetire, ASLR(0x00844790), GameObject* This);
		inline static FUNCTION_PTR(void, __thiscall, SetStatusTemporaryRetire, ASLR(0x008447B0), GameObject* This, float a2);
		inline static FUNCTION_PTR(bool, __thiscall, ProcessMessage, ASLR(0x00844A30), int* This, fnd::Message* message);
		inline static FUNCTION_PTR(GameObject*, __thiscall, __ct, ASLR(0x008448A0), GameObject* This);
		inline static FUNCTION_PTR(GameObject*, __thiscall, GetParentObject, ASLR(0x00844AE0), GameObject* This);
		inline static FUNCTION_PTR(uint32_t, __thiscall, GetExtUserData, ASLR(0x00844830), GameObject* This, int a2);
		inline static FUNCTION_PTR(int, __thiscall, SetExtUserData, ASLR(0x00844850), GameObject* This, int a2, int a3);
	};
}