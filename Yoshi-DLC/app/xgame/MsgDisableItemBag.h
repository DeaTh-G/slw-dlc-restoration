#pragma once

namespace app
{
    namespace xgame
    {
        class MsgDisableItemBag : public fnd::Message
        {
        public:
            MsgDisableItemBag() : Message()
            {
                Type = fnd::PROC_MSG_DISABLE_ITEM_BAG;
            }
        };
    }

}