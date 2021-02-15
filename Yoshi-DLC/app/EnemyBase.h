#pragma once

namespace app
{
	class EnemyBase
	{
	public:
		inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x00671FA0), GameObject* This);
		inline static FUNCTION_PTR(bool, __thiscall, ProcessMessage, ASLR(0x006724B0), int* This, fnd::Message* Message);
		inline static FUNCTION_PTR(int*, __thiscall, CreateCenterPositionFrame, ASLR(0x00672040), GameObject* This, Vector3* Position);
		inline static FUNCTION_PTR(int*, __thiscall, GetCenterPositionFrame, ASLR(0x00672080), GameObject* This);
		inline static FUNCTION_PTR(int*, __thiscall, SendTouchDamage, ASLR(0x00672180), int* This, fnd::Message* Message);
	};
}