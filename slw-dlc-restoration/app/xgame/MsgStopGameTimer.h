#pragma once

namespace app
{
    namespace xgame
    {
        class MsgStopGameTimer : public fnd::Message
        {
        public:
            MsgStopGameTimer() : Message()
            {
                Type = fnd::PROC_MSG_STOP_GAME_TIMER;
            }
        };
    }
}