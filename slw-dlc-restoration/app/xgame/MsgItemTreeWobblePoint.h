#pragma once

namespace app
{
    namespace xgame
    {
        class MsgItemTreeWobblePoint : public fnd::Message
        {
        public:
            int field_18{};
            int field_1C{};
            csl::math::Vector3 position{};

            MsgItemTreeWobblePoint() : Message()
            {
                Type = fnd::PROC_MSG_ITEM_TREE_WOBBLE_POINT;
            }

            MsgItemTreeWobblePoint(Vector3* a1) : MsgItemTreeWobblePoint()
            {
                position = *a1;
            }
        };
    }
}