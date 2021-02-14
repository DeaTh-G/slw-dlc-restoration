#pragma once

namespace app
{
	class EnemyBase
	{
	public:
		inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x00671FA0), GameObject* This);
		inline static FUNCTION_PTR(bool, __thiscall, ProcessMessage, ASLR(0x006724B0), int* This, fnd::Message* message);
		inline static FUNCTION_PTR(int*, __thiscall, GetCenterPositionFrame, ASLR(0x00672080), GameObject* This);
	};
}