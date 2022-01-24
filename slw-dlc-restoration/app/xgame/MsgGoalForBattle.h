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
            csl::math::Matrix34 MatrixP1{};
            csl::math::Matrix34 MatrixP2{};

            MsgGoalForBattle() : Message()
            {
                Type = fnd::PROC_MSG_GOAL_FOR_BATTLE;
            }

            MsgGoalForBattle(int winnerNo) : MsgGoalForBattle()
            {
                WinnerNumber = winnerNo;
            }

            MsgGoalForBattle(int winnerNo, csl::math::Matrix34* matrixP1, csl::math::Matrix34* matrixP2) : MsgGoalForBattle()
            {
                WinnerNumber = winnerNo;
                MatrixP1 = *matrixP1;
                MatrixP2 = *matrixP2;
            }
        };
    }

}