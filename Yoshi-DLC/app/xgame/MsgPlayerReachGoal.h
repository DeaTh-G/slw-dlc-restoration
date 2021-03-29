#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPlayerReachGoal : public fnd::Message
        {
        public:
            int field_18{};
            int field_1C{};
            csl::math::Vector3 field_20{};
            csl::math::Vector3 field_30{};

            MsgPlayerReachGoal() : Message()
            {
                Type = fnd::PROC_MSG_PLAYER_REACH_GOAL;
            }
        };
    }

}