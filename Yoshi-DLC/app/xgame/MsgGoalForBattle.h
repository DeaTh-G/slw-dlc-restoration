#pragma once

namespace app
{
    namespace xgame
    {
        class MsgGoalForBattle : public fnd::Message
        {
        public:
            int WinnerNumber{};
            int field_1C{};
            csl::math::Matrix34 field_20{};
            csl::math::Matrix34 field_60{};

            MsgGoalForBattle() : Message()
            {
                Type = fnd::PROC_MSG_GOAL_FOR_BATTLE;
            }
            
            MsgGoalForBattle(int winnerNo) : MsgGoalForBattle()
            {
                WinnerNumber = winnerNo;
            }
        };
    }

}