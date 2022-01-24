#pragma once

namespace app
{
    namespace xgame
    {
        class MsgGetPosition : public fnd::Message
        {
        public:
            csl::math::Vector3* position;

            MsgGetPosition() : Message()
            {
                Type = fnd::PROC_MSG_GET_POSITION;
            }

            MsgGetPosition(Vector3* a1) : MsgGetPosition()
            {
                position = a1;
            }
        };
    }
}