#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPLSendGameInfo : public fnd::Message
        {
        public:
            Game::EUser PlayerNumber{};
            float field_1C{};
            int field_20{};
            int NumHearts{};
            int MaxNumHearts{};

            MsgPLSendGameInfo() : Message()
            {
                Type = fnd::PROC_MSG_PL_SEND_GAME_INFO;
            }
        };
    }

}