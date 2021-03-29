#pragma once

namespace app
{
    namespace xgame
    {
        class MsgCatchEndPlayer : public fnd::MessageNew
        {
        public:
            bool field_18{};
            char field_19{};
            char field_1A{};
            char field_1B{};

            MsgCatchEndPlayer() : MessageNew()
            {
                Type = fnd::PROC_MSG_CATCH_END_PLAYER;
            }

            MsgCatchEndPlayer(bool a1) : MsgCatchEndPlayer()
            {
                field_18 = a1;
            }
        };
    }

}