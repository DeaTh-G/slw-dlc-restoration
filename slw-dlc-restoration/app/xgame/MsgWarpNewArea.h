#pragma once

namespace app
{
    namespace xgame
    {
        class MsgWarpNewArea : public fnd::Message
        {
        public:
            int PlayerNumber{};
            bool IsDestination2D{};
            INSERT_PADDING(3);
            csl::math::Vector3 TargetPosition{};
            csl::math::Quaternion TargetRotation{};
            int field_40{};
            char field_44{};
            char field_45{};
            short field_46{};
            int field_48{};
            int field_4C{};

            MsgWarpNewArea() : Message()
            {
                Type = fnd::PROC_MSG_WARP_NEW_AREA;
            }

            MsgWarpNewArea(int playerNo, bool isDest2D, csl::math::Vector3 targetPos, csl::math::Quaternion targetRot, int a4, char a5) : MsgWarpNewArea()
            {
                PlayerNumber = playerNo;
                IsDestination2D = isDest2D;
                TargetPosition = targetPos;
                TargetRotation = targetRot;
                field_40 = a4;
                field_45 = a5;
            }
        };
    }

}