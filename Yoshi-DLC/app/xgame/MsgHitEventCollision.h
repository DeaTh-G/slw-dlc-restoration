#pragma once

namespace app
{
    namespace xgame
    {
        class MsgHitEventCollision : public fnd::MessageNew
        {
        public:
            int field_18;
            int field_1C;
            int ActorID;

            MsgHitEventCollision() : MessageNew()
            {
                Type = fnd::PROC_MSG_HIT_EVENT_COLLISION;
                field_18 = 0;
                field_1C = 0;
                ActorID = 0;
            }
        };
    }
}