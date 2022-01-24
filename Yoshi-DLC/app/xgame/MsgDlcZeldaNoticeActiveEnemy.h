#pragma once

namespace app
{
    namespace xgame
    {
        class MsgDlcZeldaNoticeActiveEnemy : public fnd::Message
        {
        public:
            MsgDlcZeldaNoticeActiveEnemy() : Message()
            {
                Type = fnd::PROC_MSG_DLC_ZELDA_NOTICE_ACTIVE_ENEMY;
            }
        };
    }
}