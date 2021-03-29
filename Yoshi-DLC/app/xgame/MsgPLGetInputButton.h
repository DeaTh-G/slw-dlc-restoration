#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPLGetInputButton : public fnd::Message
        {
        public:
            int field_18{};
            int field_1C{};
            bool IsPressed{};

            MsgPLGetInputButton() : Message()
            {
                Type = fnd::PROC_MSG_PL_GET_INPUT_BUTTON;
            }

            MsgPLGetInputButton(int a1, int a2) : MsgPLGetInputButton()
            {
                field_18 = a1;
                field_1C = a2;
            }
        };
    }

}