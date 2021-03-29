#pragma once

namespace app
{
    namespace xgame
    {
        class MsgGetExternalMovePosition : public fnd::MessageNew
        {
        public:
            csl::math::Matrix34* Transform;

            MsgGetExternalMovePosition() : MessageNew()
            {
                Type = fnd::PROC_MSG_GET_EXTERNAL_MOVE_POSITION;
            }
        };
    }

}