#pragma once

namespace app
{
    namespace xgame
    {
        class MsgHitEventCollision : public MsgHitTriggerBase
        {
        public:
            MsgHitEventCollision() : MsgHitTriggerBase()
            {
                Type = fnd::PROC_MSG_HIT_EVENT_COLLISION;
            }
        };
    }
}