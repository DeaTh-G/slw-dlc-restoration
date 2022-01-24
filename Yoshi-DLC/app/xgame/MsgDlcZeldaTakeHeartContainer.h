#pragma once

namespace app
{
    namespace xgame
    {
        class MsgDlcZeldaTakeHeartContainer : public fnd::Message
        {
        public:
            MsgDlcZeldaTakeHeartContainer() : Message()
            {
                Type = fnd::PROC_MSG_DLC_ZELDA_TAKE_HEART_CONTAINER;
            }
        };
    }
}