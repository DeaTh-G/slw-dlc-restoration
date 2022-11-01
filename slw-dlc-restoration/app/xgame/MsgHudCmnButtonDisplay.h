#pragma once

namespace app
{
    namespace xgame
    {
        class MsgHudCmnButtonDisplay : public fnd::Message
        {
        public:
            int field_18{};
            int field_1C { -1 };

            MsgHudCmnButtonDisplay() : Message()
            {
                Type = fnd::PROC_MSG_HUD_CMN_BUTTON_DISPLAY;
            }

            MsgHudCmnButtonDisplay(int a1) : Message()
            {
                Type = fnd::PROC_MSG_HUD_CMN_BUTTON_DISPLAY;
                field_18 = a1;
            }
        };
    }

}