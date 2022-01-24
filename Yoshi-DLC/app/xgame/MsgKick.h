#pragma once

namespace app
{
    namespace xgame
    {
        class MsgKick : public MsgDamageBase
        {
        public:
            csl::math::Vector3 field_60;
            int field_70;
            int field_74;
            int field_78;
            int field_7C;

            MsgKick() : MsgDamageBase()
            {
                Type = fnd::PROC_MSG_KICK;
            }

            inline static FUNCTION_PTR(int, __thiscall, SetReplyForSucceed, ASLR(0x004D8800), xgame::MsgKick* message);
        };
    }
}