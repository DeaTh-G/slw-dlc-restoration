#pragma once

namespace app
{
    namespace xgame
    {
        class MsgLeaveEventCollision : public fnd::Message
        {
        public:
            fnd::HandleBase field_18{};
            fnd::HandleBase field_20{};
            int ActorID{};

            MsgLeaveEventCollision() : Message()
            {
                Type = fnd::PROC_MSG_LEAVE_EVENT_COLLISION;
            }
        };
    }
}