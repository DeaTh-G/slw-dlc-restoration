#pragma once

namespace app
{
	class EnemyBlowOffObjectCInfo
	{
	public:
		int Model;
		int Skeleton;
		int Animation;
		int field_0C;
		Matrix34 field_10;
		Vector3 field_50;
		Vector3 field_60;
		Vector3 field_70;
		int field_80;
		int field_84;
		int field_88;
		int field_8C;

		inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x0072B840), EnemyBlowOffObjectCInfo* This);
		inline static FUNCTION_PTR(int*, __thiscall, SetParamByMessage, ASLR(0x0072B8D0), EnemyBlowOffObjectCInfo* This, fnd::Message* message);

	};
}