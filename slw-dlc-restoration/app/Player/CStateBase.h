#pragma once

namespace app
{
    namespace Player
    {
        class CStateBase
        {
        public:
            static void ProcessMessage();
            static void ProcMsgTakeObject();
            static void ProcMsgPlayerReachGoal();
            static void CheckHitDamage();
        };
    }
}