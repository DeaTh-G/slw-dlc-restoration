#pragma once

namespace app
{
    namespace xgame
    {
        class MsgResumeGameTimer : public fnd::Message
        {
        public:
            MsgResumeGameTimer() : Message()
            {
                Type = fnd::PROC_MSG_RESUME_GAME_TIMER;
            }
        };
    }

}