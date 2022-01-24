#pragma once

namespace app
{
    namespace xgame
    {
        class MsgKill : public fnd::Message
        {
        public:
            MsgKill()
            {
                Type = fnd::PROC_MSG_KILL;
            }
        };
    }
}