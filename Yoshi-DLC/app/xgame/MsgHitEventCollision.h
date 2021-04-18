#pragma once

namespace app
{
    namespace xgame
    {
        class MsgHitEventCollision : public fnd::Message
        {
        public:
            int* field_18;
            int* field_1C;
            int ActorID{};

            MsgHitEventCollision() : Message()
            {
                Type = fnd::PROC_MSG_HIT_EVENT_COLLISION;
            }
        };
    }
}