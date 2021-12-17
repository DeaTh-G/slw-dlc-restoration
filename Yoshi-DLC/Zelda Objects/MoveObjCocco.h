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
        csl::math::Vector3 field_30;
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
            field_30 = *target;
            field_50 = field_30.X;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetTargetDirection(csl::math::Vector3* target, float a2)
        {
            MoveType = MoveType::MOVE_TARGET;
            field_30 = *target;
            field_50 = field_30.X;
            *(char*)&State = 1;

            game::GOCMovement* gocMovement = GetOwnerMovement();
            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetTargetDirectionJump(csl::math::Vector3* target, float a2)
        {
            MoveType = MoveType::MOVE_TARGET_JUMP;
            field_30 = *target;
            field_50 = field_30.X;

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
            field_30 = playerPosition;
            field_50 = a1;
            *(char*)&State = 1;
            *((csl::math::Vector3*)contextParam + 2) = Vector3(dot, 0, a2);

            game::GOCMovement::EnableMovementFlag((int*)gocMovement, 0);
        }

        void SetTargetPoint(csl::math::Vector3* target, float a2)
        {
            MoveType = MoveType::MOVE_TARGET_POINT;
            field_30 = *target;
            field_50 = field_30.X;

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