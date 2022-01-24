#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPushCameraController : public fnd::Message
        {
        public:
            Camera::CCameraController* CameraController = NULL;
            float field_1C{};
            bool field_20{};
            INSERT_PADDING(3);
            int field_24;
            int field_28;
            bool field_2C;
            INSERT_PADDING(3);

            MsgPushCameraController(Camera::CCameraController* controller, int a2, int a3, int a4, int a5, float a6) : Message()
            {
                Type = fnd::PROC_MSG_PUSH_CAMERA_CONTROLLER;

                CameraController = controller;
                field_20 = a2;
                field_24 = a3;
                field_28 = a4;
                field_2C = a5;
                field_1C = a6;
            }
        };
    }

}