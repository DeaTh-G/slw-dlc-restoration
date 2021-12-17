#pragma once

namespace app
{
    namespace xgame
    {
        class MsgBreakRollingStone : public fnd::Message
        {
        public:
            MsgBreakRollingStone() : Message()
            {
                Type = fnd::PROC_MSG_BREAK_ROLLING_STONE;
            }
        };
    }
}