#pragma once

namespace app
{
    namespace xgame
    {
        class MsgStayTrigger : public fnd::Message
        {
        public:
            int field_18{};
            int field_1C{};
            int field_20{};

            MsgStayTrigger() : Message()
            {
                Type = fnd::PROC_MSG_STAY_TRIGGER;
            }
        };
    }

}