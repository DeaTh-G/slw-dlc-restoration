#pragma once

namespace app
{
    class ObjCocco;

    class alignas(16) MoveObjCocco: public game::MoveController
    {
    public:
        class CallbackHandle
        {
        public:
            ObjCocco* Object{};
            short field_04{};
            INSERT_PADDING(2);
            void (ObjCocco::*NotifyStopCallback)();
        };
        
        enum class State : char {};

        enum class MoveType : char 
        {
            MOVE_NONE,
            MOVE_TARGET_POINT,
            MOVE_TARGET_RELATIVE_POINT,
            MOVE_TARGET,
            MOVE_TARGET_JUMP,
            MOVE_TARGET_PLAYER
        };

    private:
        CallbackHandle CallbackHandle;
        INSERT_PADDING(4);
        MoveType MoveType;
        INSERT_PADDING(3);
        int field_24;
        int field_28;
        int field_2C;
        csl::math::Vector3 TargetPosition;
        csl::math::Vector3 field_40;
        float field_50;
        int field_54;
        int field_58;
        int field_5C;
        State State;
        INSERT_PADDING(3);
        int field_64;
        int field_68;
        int field_6C;

    protected:
        int OnEnter() override
        {
            csl::math::Vector3 upVector { 0, 0, 1 };
            
            game::GOCMovement* gocMovement = GetOwnerMovement();
            int* contextParam = game::GOCMovement::GetContextParam((int*)gocMovement);

            math::Vector3Rotate(&field_40, (csl::math::Quaternion*)contextParam + 1, &upVector);
            return 1;
        }

        int Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (MoveType == MoveType::MOVE_NONE)
                return 0;

            csl::math::Vector3 vector{};
            csl::math::Vector3 upVector { 0, 1, 0 };
            csl::math::Vector3 depthVector { 0, 0, 1 }; 

            game::GOCMovement* gocMovement = GetOwnerMovement();
            int* contextParam = game::GOCMovement::GetContextParam((int*)gocMovement);
            math::Vector3Rotate(&upVector, (csl::math::Quaternion*)contextParam + 1, &upVector);
            math::Vector3Rotate(&depthVector, (csl::math::Quaternion*)contextParam + 1, &depthVector);

            switch (MoveType)
            {
            case MoveType::MOVE_TARGET_POINT:
            {
                csl::math::Vector3 movePosition{};
                csl::math::Vector3 moveOffset{};
                csl::math::Vector3 scaledUpVector{};

                math::Vector3Subtract(&TargetPosition, (csl::math::Vector3*)contextParam, &movePosition);
                float dot = math::Vector3DotProduct(&movePosition, &upVector);
                math::Vector3Scale(&upVector, dot, &scaledUpVector);
                math::Vector3Subtract(&movePosition, &scaledUpVector, &movePosition);
                math::Vector3NormalizeZero(&movePosition, &movePosition);
                dot = math::Vector3DotProduct(&movePosition, &depthVector);
                dot = csl::math::Clamp(dot, -1, 1);
                if (abs(acosf(dot)) <= 0.000001f)
                {
                    dot = math::Vector3DotProduct((csl::math::Vector3*)contextParam + 2, &scaledUpVector);
                    math::Vector3Scale(&scaledUpVector, dot, &moveOffset);

                    if ((char)State == 1)
                    {
                        math::Vector3Scale(&-movePosition, 100, &vector);
                        -movePosition;
                    }
                }
                else
                {
                    float someFloat = 12.56637f * updateInfo.deltaTime;
                    if (someFloat < dot)
                    {
                        csl::math::Vector3 forwardVector { 1, 0, 0 };
                        math::Vector3Rotate(&forwardVector, (csl::math::Quaternion*)contextParam + 1, &forwardVector);
                        dot = math::Vector3DotProduct(&forwardVector, &movePosition);
                        dot = csl::math::Select(dot, abs(someFloat), -abs(someFloat));

                        csl::math::Quaternion rotation{};

                        Eigen::Vector3f v(scaledUpVector.X, scaledUpVector.Y, scaledUpVector.Z);
                        Eigen::Quaternionf q(Eigen::AngleAxisf(dot, v));
                        rotation = csl::math::Quaternion(q.x(), q.y(), q.z(), q.w());

                        math::Vector3Rotate(&depthVector, &rotation, &depthVector);

                        dot = math::Vector3DotProduct((csl::math::Vector3*)contextParam + 2, &scaledUpVector);
                        math::Vector3Scale(&scaledUpVector, dot, &moveOffset);
                    }

                    if ((char)State == 1)
                    {
                        math::Vector3Scale(&-movePosition, 100, &vector);
                        -movePosition;
                    }
                }

                field_40 = movePosition;

                math::Vector3Scale(&movePosition, field_50, &movePosition);
                math::Vector3Add(&movePosition, &moveOffset, &movePosition);
                math::Vector3Scale(&vector, updateInfo.deltaTime, &vector);
                math::Vector3Add(&movePosition, &vector, &movePosition);
                math::Vector3Scale(&movePosition, updateInfo.deltaTime, &movePosition);
                math::Vector3Add((csl::math::Vector3*)contextParam, &movePosition, (csl::math::Vector3*)contextParam);

                /*csl::math::Vector3 doubleScaledUpVector{};
                csl::math::Vector3 upCross{};

                dot = math::Vector3DotProduct(&field_40, &scaledUpVector);
                math::Vector3Scale(&scaledUpVector, dot, &doubleScaledUpVector);
                math::Vector3Subtract(&field_40, &doubleScaledUpVector, &doubleScaledUpVector);
                math::Vector3NormalizeZero(&doubleScaledUpVector, &doubleScaledUpVector);
                math::Vector3CrossProduct(&scaledUpVector, &doubleScaledUpVector, &doubleScaledUpVector);
                math::Vector3CrossProduct(&doubleScaledUpVector, &scaledUpVector, &upCross);

                math::Vector3NormalizeZero(&doubleScaledUpVector, &doubleScaledUpVector);
                csl::math::Matrix34 m{};
                *(csl::math::Vector3*)(m.data[0]) = doubleScaledUpVector;
                *(csl::math::Vector3*)(m.data[1]) = scaledUpVector;
                *(csl::math::Vector3*)(m.data[2]) = upCross;

                csl::math::Quaternion rotation = GetRotationFromMatrix(&m);
                *(((csl::math::Quaternion*)contextParam) + 1) = rotation;

                if ((char)State)
                {
                    if ((char)State != 1)
                    {
                        if (MoveType == MoveType::MOVE_NONE)
                            return 0;

                        if ((char)MoveType <= 2)
                        {
                            csl::math::Vector3 targetDistance{};

                            math::Vector3Subtract(&TargetPosition, (csl::math::Vector3*)contextParam, &targetDistance);
                        }
                    }
                }*/

                break;
            }
            case MoveType::MOVE_TARGET_RELATIVE_POINT:
                break;
            case MoveType::MOVE_TARGET:
                break;
            case MoveType::MOVE_TARGET_JUMP:
                break;
            case MoveType::MOVE_TARGET_PLAYER:
                break;
            }
            return 0;
        }

    public:
        MoveObjCocco() : MoveController(114)
        {
            MoveType = MoveType::MOVE_TARGET_POINT;
        }

        void SetRelativeTargetPoint(csl::math::Vector3* target, float a2)
        {
            MoveType = MoveType::MOVE_TARGET_RELATIVE_POINT;
            TargetPosition = *target;
            field_50 = a2;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetTargetDirection(csl::math::Vector3* target, float a2)
        {
            MoveType = MoveType::MOVE_TARGET;
            TargetPosition = *target;
            field_50 = a2;
            *(char*)&State = 1;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetTargetDirectionJump(csl::math::Vector3* target, float a2)
        {
            MoveType = MoveType::MOVE_TARGET_JUMP;
            TargetPosition = *target;
            field_50 = a2;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            int* contextParam = game::GOCMovement::GetContextParam((int*)gocMovement);
            *((csl::math::Vector3*)contextParam + 2) = Vector3(0, a2, 0);
        }

        void SetTargetDirectionJump(float a1, float a2)
        {
            csl::math::Vector3 upVector { 0, 1, 0 };

            game::GOCMovement* gocMovement = GetOwnerMovement();
            int* contextParam = game::GOCMovement::GetContextParam((int*)gocMovement);
            math::Vector3Rotate(&upVector, (csl::math::Quaternion*)contextParam + 1, &upVector);

            // Fix this so it detects P2 as well
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)(((GameObject*)(((int*)gocMovement)[5]))->field_24[1]), 0);
            if (!playerInfo)
                return;

            csl::math::Vector3 playerPosition{ *((csl::math::Vector3*)playerInfo + 4) };
            math::Vector3Subtract(&playerPosition, (csl::math::Vector3*)contextParam, &playerPosition);
            float dot = math::Vector3DotProduct(&playerPosition, &upVector);
            math::Vector3Scale(&upVector, dot, &upVector);
            math::Vector3Subtract(&playerPosition, &upVector, &playerPosition);
            math::Vector3NormalizeZero(&playerPosition, &playerPosition);

            MoveType = MoveType::MOVE_TARGET_PLAYER;
            TargetPosition = playerPosition;
            field_50 = a1;
            *(char*)&State = 1;
            *((csl::math::Vector3*)contextParam + 2) = Vector3(dot, 0, a2);

            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetTargetPoint(csl::math::Vector3* target, float a2)
        {
            MoveType = MoveType::MOVE_TARGET_POINT;
            TargetPosition = *target;
            field_50 = a2;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetNotifyStopCallback(short a1, void (ObjCocco::*notifyStopCallback)(), ObjCocco* obj)
        {
            CallbackHandle.Object = obj;
            CallbackHandle.field_04 = a1;
            CallbackHandle.NotifyStopCallback = notifyStopCallback;
        }
    };
}