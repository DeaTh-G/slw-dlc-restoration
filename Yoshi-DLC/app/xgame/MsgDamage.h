#pragma once

namespace app
{
    namespace AttackType
    {
        inline static FUNCTION_PTR(bool, __cdecl, IsDamaged, ASLR(0x00844C60), int AttackType, int a2);
    }

    namespace xgame
    {
        class MsgDamage : public fnd::MessageNew
        {
        private:
            inline static FUNCTION_PTR(bool, __thiscall, __ct, ASLR(0x004D8580), MsgDamage* This, int a1, int a2, int a3, xgame::MsgHitEventCollision* hitMessage, csl::math::Vector3* position);

        public:
            fnd::HandleBase field_18{};
            int field_20{};
            int field_24{};
            int field_28{};
            int field_2C{};
            csl::math::Vector3 field_30{};
            csl::math::Vector3 field_40{};
            int field_50{};
            int field_54{};
            int field_58{};
            int field_5C{};
            int AttackType{};
            int field_64{};
            int field_68{};
            int field_6C{};
            int field_70{};
            int field_74{};
            int field_78{};
            int field_7C{};
            int field_80{};
            int field_84{};
            int field_88{};
            int field_8C{};
            int field_90{};
            int field_94{};
            int field_98{};
            int field_9C{};

            MsgDamage(int a1, int a2, int a3, xgame::MsgHitEventCollision* hitMessage, csl::math::Vector3* position)
            {
                sizeof(MsgDamage);
                __ct(this, a1, a2, a3, hitMessage, position);
            }

            inline static FUNCTION_PTR(int, __thiscall, SetReply, ASLR(0x004D86C0), MsgDamage* message, csl::math::Vector3* translation, int a3);
        };
    }
}
