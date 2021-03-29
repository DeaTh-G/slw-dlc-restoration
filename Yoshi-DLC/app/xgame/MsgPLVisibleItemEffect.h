#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPLVisibleItemEffect : public fnd::Message
        {
        public:
            int field_18{};

            MsgPLVisibleItemEffect() : Message()
            {
                Type = fnd::PROC_MSG_PL_VISIBLE_ITEM_EFFECT;
            }

            MsgPLVisibleItemEffect(int a1) : MsgPLVisibleItemEffect()
            {
                field_18 = a1;
            }
        };
    }

}