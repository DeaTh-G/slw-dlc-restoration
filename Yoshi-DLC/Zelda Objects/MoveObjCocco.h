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
            short field_06{};
            void (ObjCocco::*NotifyStopCallback)();
        };
        
        enum class State : char 
        {
            STATE_0,
            STATE_1
        };

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
        CallbackHandle CallbackHandle{};
        INSERT_PADDING(4);
        MoveType MoveType{};
        INSERT_PADDING(3);
        int field_24{};
        int field_28{};
        int field_2C{};
        csl::math::Vector3 TargetPosition{};
        csl::math::Vector3 field_40{};
        float Speed{};
        int field_54{};
        int field_58{};
        int field_5C{};
        State State{};
        INSERT_PADDING(3);
        int field_64{};
        int field_68{};
        int field_6C{};

    public:
        MoveObjCocco() : MoveController(114)
        {
            MoveType = MoveType::MOVE_TARGET_POINT;
        }

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
            csl::math::Vector3 originalPosition{};
            csl::math::Vector3 targetPosition{};
            csl::math::Vector3 movePosition{};

            game::GOCMovement* gocMovement = GetOwnerMovement();
            int* contextParam = game::GOCMovement::GetContextParam((int*)gocMovement);
            originalPosition = *(csl::math::Vector3*)contextParam;
            math::Vector3Rotate(&upVector, (csl::math::Quaternion*)contextParam + 1, &upVector);
            math::Vector3Rotate(&depthVector, (csl::math::Quaternion*)contextParam + 1, &depthVector);

            switch (MoveType)
            {
            case MoveType::MOVE_TARGET_POINT:
                targetPosition = TargetPosition;
                math::Vector3Subtract(&targetPosition, (csl::math::Vector3*)contextParam, &movePosition);
                break;
            case MoveType::MOVE_TARGET_RELATIVE_POINT:
                break;
            case MoveType::MOVE_TARGET:
            case MoveType::MOVE_TARGET_JUMP:
                targetPosition = *(csl::math::Vector3*)contextParam;
                movePosition = TargetPosition;
                break;
            case MoveType::MOVE_TARGET_PLAYER:
                int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)(((GameObject*)(((int*)gocMovement)[5]))->field_24[1]), 0);
                if (!playerInfo)
                {
                    movePosition = TargetPosition;
                    break;
                }

                float magnitude{};
                math::Vector3SquareMagnitude((csl::math::Vector3*)playerInfo + 3, &magnitude);
                if (magnitude <= 40000)
                {
                    math::Vector3Subtract((csl::math::Vector3*)playerInfo + 1, (csl::math::Vector3*)contextParam, &movePosition);
                }
                else
                {
                    math::Vector3NormalizeZero((csl::math::Vector3*)playerInfo + 3, &movePosition);
                    math::Vector3Scale(&movePosition, 100, &movePosition);
                    math::Vector3Add((csl::math::Vector3*)playerInfo + 1, &movePosition, &movePosition);
                    math::Vector3Subtract(&movePosition, (csl::math::Vector3*)contextParam, &movePosition);
                }
                break;
            }

            csl::math::Vector3 moveOffset{};
            csl::math::Vector3 scaledUpVector{};

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
            }
            else
            {
                float someFloat = 12.56637f * updateInfo.deltaTime;
                if (someFloat < acosf(dot))
                {
                    csl::math::Vector3 forwardVector{ 1, 0, 0 };
                    math::Vector3Rotate(&forwardVector, (csl::math::Quaternion*)contextParam + 1, &forwardVector);
                    dot = math::Vector3DotProduct(&forwardVector, &movePosition);
                    dot = csl::math::Select(dot, abs(someFloat), -abs(someFloat));

                    csl::math::Quaternion rotation{};

                    Eigen::Vector3f v(upVector.X, upVector.Y, upVector.Z);
                    Eigen::Quaternionf q(Eigen::AngleAxisf(dot, v));
                    rotation = csl::math::Quaternion(q.x(), q.y(), q.z(), q.w());

                    math::Vector3Rotate(&depthVector, &rotation, &depthVector);

                    dot = math::Vector3DotProduct((csl::math::Vector3*)contextParam + 2, &upVector);
                    math::Vector3Scale(&upVector, dot, &moveOffset);
                }
                else
                {
                    if (MoveType == MoveType::MOVE_TARGET_PLAYER)
                    {
                        TargetPosition = movePosition;
                        MoveType = MoveType::MOVE_TARGET;
                    }

                    dot = math::Vector3DotProduct((csl::math::Vector3*)contextParam + 2, &scaledUpVector);
                    math::Vector3Scale(&scaledUpVector, dot, &moveOffset);
                }
            }

            if (State == State::STATE_1)
            {
                if (MoveType == MoveType::MOVE_TARGET_RELATIVE_POINT)
                    math::Vector3Scale(&-upVector, 2, &vector);
                else
                    math::Vector3Scale(&-upVector, 100, &vector);

                -upVector;
            }

            csl::math::Vector3 scaledDepth{};
            math::Vector3Scale(&depthVector, Speed, &depthVector);
            math::Vector3Add(&depthVector, &moveOffset, &depthVector);
            math::Vector3Scale(&vector, updateInfo.deltaTime, &vector);
            math::Vector3Add(&depthVector, &vector, &depthVector);
            math::Vector3Scale(&depthVector, updateInfo.deltaTime, &scaledDepth);

            math::Vector3Add((csl::math::Vector3*)contextParam, &scaledDepth, (csl::math::Vector3*)contextParam);

            field_40 = Vector3(scaledDepth.X / 10, scaledDepth.Y / 10, scaledDepth.Z / 10);

            csl::math::Vector3 doubleScaledUpVector{};
            csl::math::Vector3 upCross{};

            dot = math::Vector3DotProduct(&field_40, &upVector);
            math::Vector3Scale(&upVector, dot, &scaledUpVector);
            math::Vector3Subtract(&field_40, &scaledUpVector, &scaledUpVector);
            math::Vector3NormalizeZero(&scaledUpVector, &scaledUpVector);
            math::Vector3CrossProduct(&upVector, &scaledUpVector, &doubleScaledUpVector);
            math::Vector3CrossProduct(&doubleScaledUpVector, &upVector, &upCross);

            math::Vector3NormalizeZero(&doubleScaledUpVector, &doubleScaledUpVector);
            csl::math::Matrix34 m{};
            *(csl::math::Vector3*)(m.data[0]) = doubleScaledUpVector;
            *(csl::math::Vector3*)(m.data[1]) = upVector;
            *(csl::math::Vector3*)(m.data[2]) = upCross;

            csl::math::Quaternion rotation = GetRotationFromMatrix(&m);
            *(((csl::math::Quaternion*)contextParam) + 1) = rotation;

            switch (State)
            {
            case State::STATE_0:
            {
                csl::math::Vector3 rayStart{};
                csl::math::Vector3 rayEnd{};

                math::Vector3Scale(&upVector, 5, &scaledUpVector);
                math::Vector3Add((csl::math::Vector3*)contextParam, &scaledUpVector, &rayStart);
                math::Vector3Scale(&upVector, 0.5f, &scaledUpVector);
                math::Vector3Subtract((csl::math::Vector3*)contextParam, &scaledUpVector, &rayEnd);

                game::PhysicsRaycastOutput output{};
                if (*ObjUtil::RaycastNearestCollision(&output, (GameDocument*)(((GameObject*)(((int*)gocMovement)[5]))->field_24[1]), &rayStart, &rayEnd, 51606)
                    && (output.field_24 & 0x10) == 0)
                {
                    dot = math::Vector3DotProduct(&depthVector, &upVector);
                    math::Vector3Scale(&upVector, dot, &scaledUpVector);
                    math::Vector3Subtract(&depthVector, &scaledUpVector, &depthVector);

                    *(csl::math::Vector3*)contextParam = output.field_00;
                }
                else
                {
                    State = State::STATE_1;
                }

                break;
            }
            case State::STATE_1:
            {
                csl::math::Vector3 rayStart{};
                csl::math::Vector3 rayEnd = *(csl::math::Vector3*)contextParam;
                csl::math::Vector3 positionDiff{};
                csl::math::Matrix34 m{};

                math::Vector3Subtract(&originalPosition, (csl::math::Vector3*)contextParam, &positionDiff);
                float length = math::Vector3NormalizeWithLength(&positionDiff, &positionDiff);
                math::Vector3Scale(&positionDiff, length + 0.09999999f, &positionDiff);
                math::Vector3Add((csl::math::Vector3*)contextParam, &positionDiff, &rayStart);

                game::PhysicsRaycastOutput output{};
                if (*ObjUtil::RaycastNearestCollision(&output, (GameDocument*)(((GameObject*)(((int*)gocMovement)[5]))->field_24[1]), &rayStart, &rayEnd, 51606)
                    && (output.field_24 & 0x10) == 0)
                {
                    State = State::STATE_0;
                    *(csl::math::Vector3*)contextParam = output.field_00;
                }

                break;
            }
            }

            if (MoveType == MoveType::MOVE_NONE)
                return 0;

            if (MoveType < MoveType::MOVE_TARGET)
            {
                csl::math::Vector3 positionDiff{};
                csl::math::Vector3 currPositionDiff{};

                math::Vector3Subtract(&targetPosition, &originalPosition, &positionDiff);
                dot = math::Vector3DotProduct(&positionDiff, &scaledUpVector);
                math::Vector3Scale(&scaledUpVector, dot, &doubleScaledUpVector);
                math::Vector3Subtract(&positionDiff, &doubleScaledUpVector, &positionDiff);
                math::Vector3Subtract(&targetPosition, (csl::math::Vector3*)contextParam, &currPositionDiff);
                dot = math::Vector3DotProduct(&currPositionDiff, &scaledUpVector);
                math::Vector3Scale(&scaledUpVector, dot, &doubleScaledUpVector);
                math::Vector3Subtract(&currPositionDiff, &doubleScaledUpVector, &currPositionDiff);
                if (math::Vector3DotProduct(&positionDiff, &currPositionDiff) <= 0 || math::Vector3Magnitude(&currPositionDiff) <= 5)
                {
                    if (CallbackHandle.Object && CallbackHandle.field_06 < 0)
                    {
                        (CallbackHandle.Object->*CallbackHandle.NotifyStopCallback)();
                        game::GOCMovement::DisableMovementFlag((int*)gocMovement, 0);
                    }
                }
            }
            else
            {
                if (MoveType != MoveType::MOVE_TARGET_JUMP)
                    return 0;

                if (State != State::STATE_0)
                    return 0;

                if (CallbackHandle.Object && CallbackHandle.field_06 < 0)
                {
                    (CallbackHandle.Object->*CallbackHandle.NotifyStopCallback)();
                    game::GOCMovement::DisableMovementFlag((int*)gocMovement, 0);
                }
            }

            return 0;
        }

    public:
        void SetRelativeTargetPoint(csl::math::Vector3* target, float speed)
        {
            MoveType = MoveType::MOVE_TARGET_RELATIVE_POINT;
            TargetPosition = *target;
            Speed = speed;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetTargetDirection(csl::math::Vector3* target, float speed)
        {
            MoveType = MoveType::MOVE_TARGET;
            TargetPosition = *target;
            Speed = speed;
            State = State::STATE_1;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetTargetDirectionJump(csl::math::Vector3* target, float speed, float a3)
        {
            MoveType = MoveType::MOVE_TARGET_JUMP;
            TargetPosition = *target;
            Speed = speed;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            int* contextParam = game::GOCMovement::GetContextParam((int*)gocMovement);
            *((csl::math::Vector3*)contextParam + 2) = Vector3(0, a3, 0);
        }

        void SetTargetPlayer(float speed, float a2)
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
            Speed = speed;
            State = State::STATE_1;
            *((csl::math::Vector3*)contextParam + 2) = Vector3(0, a2, 0);

            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetTargetPoint(csl::math::Vector3* target, float speed)
        {
            MoveType = MoveType::MOVE_TARGET_POINT;
            TargetPosition = *target;
            Speed = speed;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetNotifyStopCallback(short a1, void (ObjCocco::*notifyStopCallback)(), ObjCocco* obj)
        {
            CallbackHandle.Object = obj;
            CallbackHandle.field_06 = a1;
            CallbackHandle.NotifyStopCallback = notifyStopCallback;
        }
    };
}