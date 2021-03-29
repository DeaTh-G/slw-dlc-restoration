#pragma once

namespace app
{
    namespace xgame
    {
        class MsgSpringImpulse : public fnd::MessageNew
        {
        public:
            int field_18{};
            int field_1C{};
            csl::math::Vector3 ObjectPosition{};
            csl::math::Vector3 LaunchVector{};
            float OutOfControl{};
            float TravelTime{};
            float field_48{};
            float OutOfParkour{};
            int field_50{};
            int field_54{};
            int field_58{};
            int field_5C{};
            csl::math::Vector3 field_60{};

            MsgSpringImpulse(game::GOCLauncher::ShotInfo* shotInfo) : MessageNew()
            {
                Type = fnd::PROC_MSG_SPRING_IMPULSE;
                ObjectPosition = shotInfo->StartingPosition;
                LaunchVector = shotInfo->LaunchVector;
                OutOfControl = shotInfo->OutOfControl;
                TravelTime = shotInfo->TravelTime;
                field_50 |= 44;
            }

            MsgSpringImpulse(game::GOCLauncher::ShotInfo* shotInfo, float outOfParkour) : MsgSpringImpulse(shotInfo)
            {
                OutOfParkour = outOfParkour;
            }
        };
    }
}