#pragma once

namespace app
{
    namespace xgame
    {
        class MsgExtendPlayer : public fnd::Message
        {
        public:
            int field_18{};
            int field_1C{};

            MsgExtendPlayer() : Message()
            {
                Type = fnd::PROC_MSG_EXTEND_PLAYER;
            }

            MsgExtendPlayer(int a1, int a2) : MsgExtendPlayer()
            {
                field_18 = a1;
                field_1C = a2;
            }
        };
    }
}