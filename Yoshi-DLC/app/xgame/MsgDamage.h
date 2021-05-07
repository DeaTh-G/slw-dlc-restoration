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
        private:
            inline static FUNCTION_PTR(bool, __thiscall, __ct2, ASLR(0x004D8500), MsgDamage* This, int a1, int a2, int a3, csl::math::Vector3* a4, csl::math::Vector3* a5);
            inline static FUNCTION_PTR(bool, __thiscall, __ct, ASLR(0x004D8580), MsgDamage* This, int a1, int a2, int a3, xgame::MsgHitEventCollision* hitMessage, csl::math::Vector3* position);
            inline static FUNCTION_PTR(int, __thiscall, f_SetReply, ASLR(0x004D86C0), MsgDamage* message, csl::math::Vector3* translation, int a3);

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

            MsgDamage() : Message()
            {
                Type = fnd::PROC_MSG_DAMAGE;
            }

            MsgDamage(int a1, int a2, int a3, csl::math::Vector3* a4, csl::math::Vector3* a5) : MsgDamage()
            {
                __ct2(this, a1, a2, a3, a4, a5);
            }

            MsgDamage(int a1, int a2, int a3, xgame::MsgHitEventCollision* hitMessage, csl::math::Vector3* position) : MsgDamage()
            {
                __ct(this, a1, a2, a3, hitMessage, position);
            }

            int SetReply(csl::math::Vector3* translation, int a2)
            {
                return f_SetReply(this, translation, a2);
            }
        };
    }
}
