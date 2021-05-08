#pragma once

namespace app
{
    namespace xgame
    {
        class MsgDlcZeldaNoticeStopEnemy : public fnd::Message
        {
        public:
            MsgDlcZeldaNoticeStopEnemy() : Message()
            {
                Type = fnd::PROC_MSG_DLC_ZELDA_NOTICE_STOP_ENEMY;
            }
        };
    }
}