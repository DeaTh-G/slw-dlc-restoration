#pragma once

namespace app
{
    namespace Player
    {
        class CStateBase
        {
        public:
            static void ProcMsgTakeObject();
            static void ProcMsgPlayerReachGoal();
            static void CheckHitDamage();
        };
    }
}