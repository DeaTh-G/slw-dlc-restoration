#pragma once

namespace app
{
    namespace xgame
    {
        class MsgKick : public fnd::MessageNew
        {
        public:
            INSERT_PADDING(104);

            MsgKick() : MessageNew() {}

            inline static FUNCTION_PTR(int, __thiscall, SetReplyForSucceed, ASLR(0x004D8800), xgame::MsgKick* message);
        };
    }
}