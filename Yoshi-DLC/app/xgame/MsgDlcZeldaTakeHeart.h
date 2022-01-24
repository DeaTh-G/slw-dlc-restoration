#pragma once

namespace app
{
    namespace xgame
    {
        class MsgDlcZeldaTakeHeart : public fnd::Message
        {
        public:
            MsgDlcZeldaTakeHeart() : Message()
            {
                Type = fnd::PROC_MSG_DLC_ZELDA_TAKE_HEART;
            }
        };
    }
}