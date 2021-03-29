#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPLVisibleItemEffect : public fnd::MessageNew
        {
        public:
            int field_18{};

            MsgPLVisibleItemEffect() : MessageNew()
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