#pragma once

namespace app
{
    namespace xgame
    {
        class MsgChangeBGMVolume : public fnd::Message
        {
        public:
            float field_18;
            float field_1C;

            MsgChangeBGMVolume() : Message()
            {
                Type = fnd::PROC_MSG_CHANGE_BGM_VOLUME;
            }

            MsgChangeBGMVolume(float a1, float a2) : MsgChangeBGMVolume()
            {
                field_18 = a1;
                field_1C = a2;
            }
        };
    }

}