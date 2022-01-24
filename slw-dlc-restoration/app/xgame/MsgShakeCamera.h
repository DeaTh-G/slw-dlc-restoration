#pragma once

namespace app
{
    namespace xgame
    {
        class MsgShakeCamera : public fnd::Message
        {
        public:
            float field_18{};
            float field_1C{};
            int field_20{};
            float field_24{};
            float field_28{};
            int field_2C{};
            csl::math::Vector3 field_30{};

            MsgShakeCamera() : Message()
            {
                Type = fnd::PROC_MSG_SHAKE_CAMERA;
                field_18 = 1;
                field_1C = 0.5f;
                field_20 = 10;
                field_28 = 0.25f;
            }

            MsgShakeCamera(float a1, float a2) : MsgShakeCamera()
            {
                field_18 = a1;
                field_1C = a2;
            }
        };
    }

}