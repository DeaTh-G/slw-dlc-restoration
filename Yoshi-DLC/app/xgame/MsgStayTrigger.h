#pragma once

namespace app
{
    namespace xgame
    {
        class MsgStayTrigger : public fnd::MessageNew
        {
        public:
            int field_18{};
            int field_1C{};
            int field_20{};

            MsgStayTrigger() : MessageNew()
            {
                Type = fnd::PROC_MSG_STAY_TRIGGER;
            }
        };
    }

}