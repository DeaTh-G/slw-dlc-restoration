#pragma once

namespace app
{
    namespace xgame
    {
        class MsgCameraOn : public fnd::Message
        {
        public:
            float field_18{};
            char field_1C{};
            INSERT_PADDING(3);
            int field_20{};
            int field_24{};
            int PlayerNumber{};
            char field_2C{};
            INSERT_PADDING(3);

            MsgCameraOn() : Message()
            {
                Type = fnd::PROC_MSG_CAMERA_ON;
            }

            MsgCameraOn(char a1, int a2, int a3, int playerNo, char a5, float a6) : MsgCameraOn()
            {
                field_18 = a6;
                field_1C = a1;
                field_20 = a2;
                field_24 = a3;
                PlayerNumber = playerNo;
                field_2C = a5;
            }
        };
    }

}