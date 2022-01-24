#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPopCameraController : public fnd::Message
        {
        public:
            Camera::CCameraController* CameraController = NULL;
            float field_1C{};
            bool field_20{};
            INSERT_PADDING(3);
            int field_24;
            bool field_28;
            INSERT_PADDING(3);

            MsgPopCameraController(Camera::CCameraController* controller, int a2, int a3, int a4, float a5) : Message()
            {
                Type = fnd::PROC_MSG_POP_CAMERA_CONTROLLER;

                CameraController = controller;
                field_20 = a2;
                field_24 = a3;
                field_28 = a4;
                field_1C = a5;
            }
        };
    }

}