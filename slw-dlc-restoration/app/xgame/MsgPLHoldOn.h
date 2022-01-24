#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPLHoldOn : public fnd::Message
        {
        public:
            int field_18{};

            MsgPLHoldOn() : Message()
            {
                Type = fnd::PROC_MSG_PL_HOLD_ON;
            }

            MsgPLHoldOn(int a1) : MsgPLHoldOn()
            {
                field_18 = a1;
            }
        };
    }

}