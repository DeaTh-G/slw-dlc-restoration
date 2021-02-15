#pragma once

namespace app
{
	class EnemyBase
	{
	public:
		inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x00671FA0), GameObject* This);
		inline static FUNCTION_PTR(bool, __thiscall, ProcessMessage, ASLR(0x006724B0), int* This, fnd::Message* message);
		inline static FUNCTION_PTR(int*, __thiscall, CreateCenterPositionFrame, ASLR(0x00672040), GameObject* This, Vector3* position);
		inline static FUNCTION_PTR(int*, __thiscall, GetCenterPositionFrame, ASLR(0x00672080), GameObject* This);
		inline static FUNCTION_PTR(int*, __thiscall, SendTouchDamage, ASLR(0x00672180), int* This, fnd::Message* message);
		inline static FUNCTION_PTR(int*, __thiscall, CreateEnemyBlowOffObject, ASLR(0x006720D0), GameObject* This, EnemyBlowOffObjectCInfo* blowOffObject);
		inline static FUNCTION_PTR(int*, __thiscall, ProcMission, ASLR(0x00671E20), GameObject* This, fnd::Message* message);
	};
}