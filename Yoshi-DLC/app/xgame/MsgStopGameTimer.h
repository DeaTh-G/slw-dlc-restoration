#pragma once

namespace app
{
    namespace xgame
    {
        class MsgStopGameTimer : public fnd::MessageNew
        {
        public:
            MsgStopGameTimer() : MessageNew()
            {
                Type = fnd::PROC_MSG_STOP_GAME_TIMER;
            }
        };
    }
}