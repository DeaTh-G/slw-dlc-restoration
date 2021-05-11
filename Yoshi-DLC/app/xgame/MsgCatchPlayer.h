#pragma once

namespace app
{
    namespace xgame
    {
        class MsgCatchPlayer : public fnd::Message
        {
        public:
            int field_18{};
            int field_1C{};
            csl::math::Matrix34 field_20{};
            int field_60{};
            int field_64{};
            int field_68{};
            int field_6C{};

            MsgCatchPlayer() : Message()
            {
                Type = fnd::PROC_MSG_CATCH_PLAYER;

                field_20.data[0][0] = 1;
                field_20.data[1][1] = 1;
                field_20.data[2][2] = 1;
                field_20.data[3][3] = 1;
            }

            MsgCatchPlayer(int a1) : MsgCatchPlayer()
            {
                field_60 = a1;
            }

            MsgCatchPlayer(csl::math::Matrix34 a1, int a2) : MsgCatchPlayer()
            {
                field_20 = a1;
                field_60 = a2;
            }
        };
    }

}