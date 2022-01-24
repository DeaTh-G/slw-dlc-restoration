#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPLKickTargetting : public fnd::Message
        {
        public:
            int field_18;
            int field_1C;
            csl::math::Vector3 field_20;
            int field_30;
            int field_34;
            int field_38;
            int field_3C;

            MsgPLKickTargetting() : Message()
            {
                Type = fnd::PROC_MSG_PL_KICK_TARGETTING;
            }
        };
    }

}