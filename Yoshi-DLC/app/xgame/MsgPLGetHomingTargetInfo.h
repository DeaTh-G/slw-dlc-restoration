#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPLGetHomingTargetInfo : public fnd::Message
        {
        public:
            int field_18;
            int field_1C;
            csl::math::Vector3 field_20;
            int field_30;
            int field_34;
            int field_38;
            int field_3C;
            int field_40;
            int field_44;
            int field_48;
            int field_4C;

            MsgPLGetHomingTargetInfo() : Message()
            {
                Type = fnd::PROC_MSG_PL_GET_HOMING_TARGET_INFO;
            }
        };
    }

}