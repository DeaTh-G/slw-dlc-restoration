#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPLJumpToTargetPosition : public fnd::Message
        {
        public:
            float field_18{};
            float field_1C{};
            csl::math::Vector3 TargetPosition{};
            csl::math::Quaternion TargetRotation{};
            char field_40{};
            INSERT_PADDING(15);

            MsgPLJumpToTargetPosition() : Message()
            {
                Type = fnd::PROC_MSG_PL_HOLD_ON;
            }

            MsgPLJumpToTargetPosition(float a1, float a2, csl::math::Vector3 targetPos,
                csl::math::Quaternion targetRot, char a5) : MsgPLJumpToTargetPosition()
            {
                field_18 = a1;
                field_1C = a2;
                TargetPosition = targetPos;
                TargetRotation = targetRot;
                field_40 = a5;
            }
        };
    }

}