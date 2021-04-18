#pragma once

namespace app
{
    namespace xgame
    {
        class MsgGetClimbObjectInfo : public fnd::Message
        {
        public:
            int field_18{};
            int field_1C{};

            MsgGetClimbObjectInfo() : Message()
            {
                Type = fnd::PROC_MSG_GET_CLIMB_OBJECT_INFO;
            }
        };
    }
}