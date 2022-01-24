#pragma once

namespace app
{
    namespace xgame
    {
        class MsgCameraOff : public fnd::Message
        {
        public:
            float field_18{};
            char field_1C{};
            char field_1D{};
            INSERT_PADDING(2);
            int field_20{};
            int PlayerNumber{};

            MsgCameraOff() : Message()
            {
                Type = fnd::PROC_MSG_CAMERA_OFF;
            }

            MsgCameraOff(char a1, char a2, int a3, int playerNo, float a5) : MsgCameraOff()
            {
                field_18 = a5;
                field_1C = a1;
                field_1D = a2;
                field_20 = a3;
                PlayerNumber = playerNo;
            }
        };
    }

}