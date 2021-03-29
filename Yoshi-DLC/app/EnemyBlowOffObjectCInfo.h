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
		csl::math::Matrix34 field_10;
		csl::math::Vector3 field_50;
		float field_60;
		float field_64;
		float field_68;
		int field_6C;
		csl::math::Vector3 field_70;
		int field_80;
		int field_84;
		int field_88;
		int field_8C;

		inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x0072B840), EnemyBlowOffObjectCInfo* This);
		inline static FUNCTION_PTR(int*, __thiscall, SetParamByMessage, ASLR(0x0072B8D0), EnemyBlowOffObjectCInfo* This, fnd::MessageNew* message);

	};
}