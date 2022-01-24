#pragma once

namespace app
{
    namespace xgame
    {
        class MsgStopBgm : public fnd::Message
        {
        public:
            float field_18{};
            size_t field_1C{};

            MsgStopBgm() : Message()
            {
                Type = fnd::PROC_MSG_STOP_BGM;
            }

            MsgStopBgm(float a1, size_t a2) : MsgStopBgm()
            {
                field_18 = a1;
                field_1C = a2;
            }
        };
    }

}