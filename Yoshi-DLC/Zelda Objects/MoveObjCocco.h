#pragma once

namespace app
{
    class ObjCocco;

	class MoveObjCocco : public game::MoveController
	{
    protected:
        enum class MoveType : char
        {
            MOVE_NONE,
            MOVE_TARGET_POINT,
            MOVE_TARGET_RELATIVE_POINT,
            MOVE_TARGET_DIRECTION,
            MOVE_TARGET_DIRECTION_JUMP,
            MOVE_TARGET_PLAYER
        };

        ObjCocco* Object{};
        INSERT_PADDING(2);
        short field_06{};
        void (ObjCocco::* NotifyStopCallback)();
        INSERT_PADDING(4);
        MoveType MovementType{};
        INSERT_PADDING(15);
        csl::math::Vector3 TargetPosition{};
        csl::math::Vector3 field_40{};
        float Speed{};
        INSERT_PADDING(12);
        char State{};
        INSERT_PADDING(15);

    public:
        MoveObjCocco() : MoveController(114)
        {
            State = 1;
            MovementType = MoveType::MOVE_TARGET_POINT;
        }

    protected:
        int OnEnter() override
        {
            csl::math::Vector3 depthVector { 0, 0, 1 };

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::ContextParam* contextParam = (game::GOCMovement::ContextParam*)game::GOCMovement::GetContextParam((int*)gocMovement);

            math::Vector3Rotate(&field_40, &contextParam->Rotation, &depthVector);
            return 1;
        }

        int Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (MovementType == MoveType::MOVE_NONE)
                return 0;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::ContextParam* contextParam = (game::GOCMovement::ContextParam*)game::GOCMovement::GetContextParam((int*)gocMovement);

            csl::math::Vector3 currentPosition = contextParam->Position;

            csl::math::Vector3 inverseUpVector{};
            csl::math::Vector3 upVector = Vector3(0, 1, 0);
            csl::math::Vector3 depthVector = Vector3(0, 0, 1);
            app::math::Vector3Rotate(&upVector, &contextParam->Rotation, &upVector);
            app::math::Vector3Rotate(&depthVector, &contextParam->Rotation, &depthVector);

            csl::math::Vector3 movePosition{};
            csl::math::Vector3 targetPosition{};

            switch (MovementType)
            {
            case MoveType::MOVE_TARGET_POINT:
            {
                csl::math::Vector3 scaledUpVector{};

                targetPosition = TargetPosition;
                math::Vector3Subtract(&targetPosition, &contextParam->Position, &movePosition);

                float dot = math::Vector3DotProduct(&movePosition, &upVector);
                math::Vector3Scale(&upVector, dot, &scaledUpVector);
                math::Vector3Subtract(&movePosition, &scaledUpVector, &movePosition);
                math::Vector3NormalizeZero(&movePosition, &movePosition);
                break;
            }
            case MoveType::MOVE_TARGET_DIRECTION:
            {
                csl::math::Vector3 scaledUpVector{};

                targetPosition = contextParam->Position;
                movePosition = TargetPosition;

                float dot = math::Vector3DotProduct(&movePosition, &upVector);
                math::Vector3Scale(&upVector, dot, &scaledUpVector);
                math::Vector3Subtract(&movePosition, &scaledUpVector, &movePosition);
                math::Vector3NormalizeZero(&movePosition, &movePosition);
                break;
            }
            case MoveType::MOVE_TARGET_DIRECTION_JUMP:
            {
                csl::math::Vector3 scaledUpVector{};

                targetPosition = contextParam->Position;
                movePosition = TargetPosition;

                float dot = math::Vector3DotProduct(&movePosition, &upVector);
                math::Vector3Scale(&upVector, dot, &scaledUpVector);
                math::Vector3Subtract(&movePosition, &scaledUpVector, &movePosition);
                math::Vector3NormalizeZero(&movePosition, &movePosition);
                break;
            }
            case MoveType::MOVE_TARGET_PLAYER:
            {
                csl::math::Vector3 scaledUpVector{};

                int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)(((GameObject*)(((int*)gocMovement)[5]))->field_24[1]), 0);
                targetPosition = contextParam->Position;
                if (!playerInfo)
                {
                    movePosition = TargetPosition;
                    break;
                }

                float magnitude;
                math::Vector3SquareMagnitude((csl::math::Vector3*)playerInfo + 3, &magnitude);
                if (magnitude <= 40000)
                {
                    math::Vector3Subtract((csl::math::Vector3*)playerInfo + 1, &contextParam->Position, &movePosition);

                    float dot = math::Vector3DotProduct(&movePosition, &upVector);
                    math::Vector3Scale(&upVector, dot, &scaledUpVector);
                    math::Vector3Subtract(&movePosition, &scaledUpVector, &movePosition);
                    math::Vector3NormalizeZero(&movePosition, &movePosition);
                }
                else
                {
                    math::Vector3NormalizeZero((csl::math::Vector3*)playerInfo + 3, &movePosition);
                    math::Vector3Scale(&movePosition, 100, &movePosition);
                    math::Vector3Add((csl::math::Vector3*)playerInfo + 1, &movePosition, &movePosition);
                    math::Vector3Subtract(&movePosition, &contextParam->Position, &movePosition);

                    float dot = math::Vector3DotProduct(&movePosition, &upVector);
                    math::Vector3Scale(&upVector, dot, &scaledUpVector);
                    math::Vector3Subtract(&movePosition, &scaledUpVector, &movePosition);
                    math::Vector3NormalizeZero(&movePosition, &movePosition);
                }

                break;
            }
            default:
                return 0;
            }

            csl::math::Vector3 forwardVector = Vector3(1, 0, 0);
            float scale = csl::math::Clamp(math::Vector3DotProduct(&movePosition, &depthVector), -1, 1);
            if (fabs(acosf(scale)) > 0.000001f)
            {
                float someFloat = 12.56637f * updateInfo.deltaTime;
                if (someFloat < acosf(scale))
                {
                    math::Vector3Rotate(&forwardVector, &contextParam->Rotation, &forwardVector);
                    float dot = math::Vector3DotProduct(&forwardVector, &movePosition);
                    dot = csl::math::Select(dot, fabs(someFloat), -fabs(someFloat));

                    csl::math::Quaternion rotation{};
                    Eigen::Vector3f v(upVector.X, upVector.Y, upVector.Z);
                    Eigen::Quaternionf q(Eigen::AngleAxisf(dot, v));
                    rotation = csl::math::Quaternion(q.x(), q.y(), q.z(), q.w());
                    math::Vector3Rotate(&movePosition, &rotation, &depthVector);
                }
                else
                {
                    if (MovementType == MoveType::MOVE_TARGET_PLAYER)
                    {
                        TargetPosition = movePosition;
                        MovementType = MoveType::MOVE_TARGET_DIRECTION;
                    }
                }
            }

            csl::math::Vector3 scaledUpVector{};

            float dot = math::Vector3DotProduct(&contextParam->field_20, &upVector);
            math::Vector3Scale(&upVector, dot, &scaledUpVector);
            if (State == 1)
            {
                inverseUpVector = Vector3(-upVector.X, -upVector.Y, -upVector.Z);

                if (MovementType == MoveType::MOVE_TARGET_RELATIVE_POINT)
                    math::Vector3Scale(&inverseUpVector, 2, &inverseUpVector);
                else
                    math::Vector3Scale(&inverseUpVector, 100, &inverseUpVector);
            }

            csl::math::Vector3 scaledMove{};
            csl::math::Vector3 moveOffset{};

            math::Vector3Scale(&movePosition, Speed, &scaledMove);
            math::Vector3Add(&scaledMove, &scaledUpVector, &scaledMove);

            math::Vector3Scale(&inverseUpVector, updateInfo.deltaTime, &inverseUpVector);
            math::Vector3Add(&scaledMove, &inverseUpVector, &contextParam->field_20);

            math::Vector3Scale(&contextParam->field_20, updateInfo.deltaTime, &moveOffset);
            math::Vector3Add(&contextParam->Position, &moveOffset, &contextParam->Position);

            field_40 = movePosition;
            dot = math::Vector3DotProduct(&field_40, &upVector);
            math::Vector3Scale(&upVector, dot, &moveOffset);
            
            csl::math::Vector3 moveDifference{};
            math::Vector3Subtract(&field_40, &moveOffset, &moveDifference);
            math::Vector3NormalizeZero(&moveDifference, &moveDifference);

            math::Vector3CrossProduct(&upVector, &moveDifference, &forwardVector);
            math::Vector3CrossProduct(&forwardVector, &upVector, &moveDifference);
            math::Vector3NormalizeZero(&forwardVector, &forwardVector);

            csl::math::Matrix34 matrix{};
            *(csl::math::Vector3*)(matrix.data[0]) = forwardVector;
            *(csl::math::Vector3*)(matrix.data[1]) = upVector;
            *(csl::math::Vector3*)(matrix.data[2]) = moveDifference;

            contextParam->Rotation = GetRotationFromMatrix(&matrix);

            csl::math::Vector3 rayStart{};
            csl::math::Vector3 rayEnd{};
            game::PhysicsRaycastOutput output{};
            switch (State)
            {
            case 0:
            {
                math::Vector3Scale(&upVector, 5, &scaledUpVector);
                math::Vector3Add(&contextParam->Position, &scaledUpVector, &rayEnd);

                math::Vector3Scale(&upVector, 0.5f, &scaledUpVector);
                math::Vector3Subtract(&contextParam->Position, &scaledUpVector, &rayStart);
                if (*ObjUtil::RaycastNearestCollision(&output, (GameDocument*)(((GameObject*)(((int*)gocMovement)[5]))->field_24[1]), &rayEnd, &rayStart, 0xC996))
                {
                    if (!(output.field_24 & 0x10))
                    {
                        dot = math::Vector3DotProduct(&contextParam->field_20, &upVector);
                        math::Vector3Scale(&upVector, dot, &scaledUpVector);
                        math::Vector3Subtract(&contextParam->field_20, &scaledUpVector, &contextParam->field_20);
                        contextParam->Position = output.field_00;
                    }
                }
                else
                {
                    State = 1;
                }
                break;
            }
            case 1:
            {
                rayStart = contextParam->Position;

                math::Vector3Subtract(&currentPosition, &rayStart, &moveOffset);
                dot = math::Vector3NormalizeWithLength(&moveOffset, &moveOffset);

                math::Vector3Scale(&moveOffset, dot + 0.099999994, &rayEnd);
                math::Vector3Add(&contextParam->Position, &rayEnd, &rayEnd);

                if (*ObjUtil::RaycastNearestCollision(&output, (GameDocument*)(((GameObject*)(((int*)gocMovement)[5]))->field_24[1]), &rayEnd, &rayStart, 0xC996)
                    && !(output.field_24 & 0x10))
                {
                    State = 0;
                    contextParam->Position = output.field_00;
                }
                break;
            }
            }

            if (MovementType == MoveType::MOVE_NONE)
                return 0;

            if (MovementType <= MoveType::MOVE_TARGET_RELATIVE_POINT)
            {
                csl::math::Vector3 targetOffset{};

                math::Vector3Subtract(&targetPosition, &currentPosition, &targetOffset);
                dot = math::Vector3DotProduct(&targetOffset, &upVector);
                math::Vector3Scale(&upVector, dot, &scaledUpVector);
                math::Vector3Subtract(&targetOffset, &scaledUpVector, &targetOffset);

                math::Vector3Subtract(&targetPosition, &contextParam->Position, &moveOffset);
                dot = math::Vector3DotProduct(&moveOffset, &upVector);
                math::Vector3Scale(&upVector, dot, &scaledUpVector);
                math::Vector3Subtract(&moveOffset, &scaledUpVector, &moveOffset);

                if (math::Vector3DotProduct(&targetOffset, &moveOffset) <= 0 ||
                    math::Vector3Magnitude(&moveOffset) <= 5)
                {
                    if (Object && field_06 < 0)
                    {
                        (Object->*NotifyStopCallback)();
                        game::GOCMovement::DisableMovementFlag((int*)gocMovement, 0);
                    }
                }
            }
            else
            {
                if (MovementType != MoveType::MOVE_TARGET_DIRECTION_JUMP)
                    return 0;

                if (State != 0)
                    return 0;

                if (Object && field_06 < 0)
                {
                    (Object->*NotifyStopCallback)();
                    game::GOCMovement::DisableMovementFlag((int*)gocMovement, 0);
                }
            }

            return 0;
        }

    public:
        void SetNotifyStopCallback(short a1, void (ObjCocco::*notifyStopCallback)(), ObjCocco* obj)
        {
            Object = obj;
            field_06 = a1;
            NotifyStopCallback = notifyStopCallback;
        }

        void SetTargetPoint(const csl::math::Vector3& targetPoint, const float speed)
        {
            MovementType = MoveType::MOVE_TARGET_POINT;
            TargetPosition = targetPoint;
            Speed = speed;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetTargetDirectionJump(const csl::math::Vector3& targetPoint, const float speed, const float height)
        {
            MovementType = MoveType::MOVE_TARGET_DIRECTION_JUMP;
            TargetPosition = targetPoint;
            Speed = speed;

            State = 1;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::ContextParam* contextParam = (game::GOCMovement::ContextParam*)game::GOCMovement::GetContextParam((int*)gocMovement);
            contextParam->field_20 = Vector3(0, height, 0);
        }

        void SetTargetPlayer(const float speed, const float height)
        {
            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::ContextParam* contextParam = (game::GOCMovement::ContextParam*)game::GOCMovement::GetContextParam((int*)gocMovement);

            csl::math::Vector3 upVector = Vector3(0, 1, 0);
            app::math::Vector3Rotate(&upVector, &contextParam->Rotation, &upVector);

            // TODO: Fix this being only Player 1
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)(((GameObject*)(((int*)gocMovement)[5]))->field_24[1]), 0);
            if (!playerInfo)
                return;

            csl::math::Vector3 targetPosition{};
            csl::math::Vector3 scaledUpVector{};

            math::Vector3Subtract((csl::math::Vector3*)playerInfo + 1, &contextParam->Position, &targetPosition);
            float dot = math::Vector3DotProduct(&targetPosition, &upVector);
            math::Vector3Scale(&upVector, dot, &scaledUpVector);
            math::Vector3Subtract(&targetPosition, &scaledUpVector, &targetPosition);
            math::Vector3NormalizeZero(&targetPosition, &targetPosition);
            
            MovementType = MoveType::MOVE_TARGET_PLAYER;
            TargetPosition = targetPosition;
            Speed = speed;

            State = 1;

            *((csl::math::Vector3*)contextParam + 2) = Vector3(0, height, 0);
            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }
	};
}