#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPLGetInputButton : public fnd::MessageNew
        {
        public:
            int field_18{};
            int field_1C{};
            bool field_20{};

            MsgPLGetInputButton() : MessageNew()
            {
                Type = fnd::PROC_MSG_PL_GET_INPUT_BUTTON;
            }
        };
    }

}