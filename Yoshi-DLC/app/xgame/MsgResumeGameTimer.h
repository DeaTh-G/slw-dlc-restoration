#pragma once

namespace app
{
    namespace xgame
    {
        class MsgResumeGameTimer : public fnd::MessageNew
        {
        public:
            MsgResumeGameTimer() : MessageNew()
            {
                Type = fnd::PROC_MSG_RESUME_GAME_TIMER;
            }
        };
    }

}