#pragma once
#include "framework.h"
#include "GameObject.h"
#include "..\app\fnd\Message.h"

namespace app
{
	class CSetObjectListener
	{
	public:
		inline static FUNCTION_PTR(void, __thiscall, SetStatusRetire, ASLR(0x00844790), app::GameObject* This);
		inline static FUNCTION_PTR(void, __thiscall, SetStatusTemporaryRetire, ASLR(0x008447B0), app::GameObject* This, float a2);
		inline static FUNCTION_PTR(bool, __thiscall, ProcessMessage, ASLR(0x00844A30), int* This, fnd::Message* message);
		inline static FUNCTION_PTR(app::GameObject*, __thiscall, __ct, ASLR(0x008448A0), app::GameObject* This);
		inline static FUNCTION_PTR(app::GameObject*, __thiscall, GetParentObject, ASLR(0x00844AE0), app::GameObject* This);
		inline static FUNCTION_PTR(uint32_t, __thiscall, GetExtUserData, ASLR(0x00844830), app::GameObject* This, int a2);
		inline static FUNCTION_PTR(int, __thiscall, SetExtUserData, ASLR(0x00844850), app::GameObject* This, int a2, int a3);
	};
}