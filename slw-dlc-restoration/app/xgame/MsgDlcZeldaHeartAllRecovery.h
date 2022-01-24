#pragma once

namespace app
{
    namespace xgame
    {
        class MsgDlcZeldaHeartAllRecovery : public fnd::Message
        {
        public:
            MsgDlcZeldaHeartAllRecovery() : Message()
            {
                Type = fnd::PROC_MSG_DLC_ZELDA_HEART_ALL_RECOVERY;
            }
        };
    }
}