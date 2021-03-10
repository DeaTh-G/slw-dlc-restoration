#pragma once

namespace app
{
	namespace AttackType
	{
		inline static FUNCTION_PTR(bool, __cdecl, IsDamaged, ASLR(0x00844C60), int AttackType, int a2);
	}

	namespace xgame
	{
		class MsgDamage : public fnd::Message
		{
		public:
			fnd::HandleBase field_18;
			int field_20;
			int field_24;
			int field_28;
			int field_2C;
			int field_30;
			int field_34;
			int field_38;
			int field_3C;
			int field_40;
			int field_44;
			int field_48;
			int field_4C;
			int field_50;
			int field_54;
			int field_58;
			int field_5C;
			int AttackType;
			int field_64;
			int field_68;
			int field_6C;
			int field_70;
			int field_74;
			int field_78;
			int field_7C;
			int field_80;
			int field_84;
			int field_88;
			int field_8C;
			int field_90;
			int field_94;
			int field_98;
			int field_9C;

			inline static FUNCTION_PTR(int, __thiscall, SetReply, ASLR(0x004D86C0), MsgDamage* message, csl::math::Vector3* translation, int a3);
		};
	}
}