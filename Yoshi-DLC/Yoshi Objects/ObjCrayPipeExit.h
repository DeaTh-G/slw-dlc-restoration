#pragma once

namespace app
{
    struct ObjCrayPipeExitData
    {
        int Direction;
    };

    enum class ObjCrayPipeExitState : int
    {
        STATE_IDLE,
        STATE_PIPE_OUT,
    };

    class ObjCrayPipeExit : public CSetObjectListener
    {
        INSERT_PADDING(8);
        ObjCrayPipeExitState State;
        xgame::MsgGetExternalMovePosition* ExternalMoveMessage = new xgame::MsgGetExternalMovePosition();
        xgame::MsgStayTrigger* StayMessage = new xgame::MsgStayTrigger();
        int PlayerNumber{};
        float Time{};
        char Direction{};
        char IsInPosition{};
        char IsTimerEnabled{};
        char IsNotMoving{};

    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCSound);

            ObjCrayPipeExitData* data = (ObjCrayPipeExitData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            Direction = data->Direction;

            fnd::GOComponent::BeginSetup(this);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                game::ColliSphereShapeCInfo collisionInfo;

                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 3;
                ObjUtil::SetupCollisionFilter(7, &collisionInfo);
                collisionInfo.field_04 |= 4;

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

                uint32_t extUserData = GetExtUserData(0);
                if (!extUserData)
                    game::GOCCollider::SetEnable(gocCollider, false);
            }

            game::GOCSound::SimpleSetup(this, 0, 0);

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            switch (message.Type)
            {
            case fnd::PROC_MSG_NOTIFY_OBJECT_EVENT:
                ProcMsgNotifyObjectEvent((xgame::MsgNotifyObjectEvent&)message);
                return true;
            case fnd::PROC_MSG_STAY_TRIGGER:
                ProcMsgStayTrigger((xgame::MsgStayTrigger&)message);
                return true;
            case fnd::PROC_MSG_GET_EXTERNAL_MOVE_POSITION:
                ProcMsgGetExternalMovePosition((xgame::MsgGetExternalMovePosition&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            StateIdle();
            StatePipeOut(updateInfo);
        }

    private:
        void ProcMsgNotifyObjectEvent(xgame::MsgNotifyObjectEvent& message)
        {
            if (message.field_18 == 1)
                SetExtUserData(0, 1);
            else if (message.field_18 == 2)
                SetExtUserData(0, 0);
        }

        void ProcMsgGetExternalMovePosition(xgame::MsgGetExternalMovePosition& message)
        {
            *ExternalMoveMessage = message;
        }

        void ProcMsgStayTrigger(xgame::MsgStayTrigger& message)
        {
            *StayMessage = message;
        }

        void StateIdle()
        {
            if (State != ObjCrayPipeExitState::STATE_IDLE)
                return;

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], StayMessage->field_20);
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
            if (!playerInfo)
                return;
            PlayerNumber = playerNo;

            xgame::MsgCatchPlayer catchMessage{ 18 };
            if (ObjUtil::SendMessageImmToPlayer(this, playerNo, &catchMessage))
                State = ObjCrayPipeExitState::STATE_PIPE_OUT;
        }

        void StatePipeOut(const fnd::SUpdateInfo& updateInfo)
        {
            if (IsInPosition & 0x20)
                return;

            if (State != ObjCrayPipeExitState::STATE_PIPE_OUT)
                return;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], PlayerNumber);
            if (!playerInfo)
                return;

            csl::math::Vector3 playerPosition = *(csl::math::Vector3*)(playerInfo + 4);
            csl::math::Vector3 targetPosition = *(csl::math::Vector3*)(gocTransform + 0x50);
            float verticalDestination = 0;

            if (!(IsInPosition & 1))
            {
                if (Direction)
                {
                    verticalDestination = targetPosition.Y += 2;
                    if (std::abs(playerPosition.Y - verticalDestination) > 1.95f)
                    {
                        ExternalMoveMessage->Transform->data[3][0] = playerPosition.X;
                        ExternalMoveMessage->Transform->data[3][1] = verticalDestination;
                        ExternalMoveMessage->Transform->data[3][2] = playerPosition.Z;
                    }
                    else
                    {
                        IsInPosition = 1;
                        return;
                    }
                }
                else
                {
                    verticalDestination = targetPosition.Y -= 12;
                    if (std::abs(playerPosition.Y - verticalDestination) > 11.95f)
                    {
                        ExternalMoveMessage->Transform->data[3][0] = playerPosition.X;
                        ExternalMoveMessage->Transform->data[3][1] = verticalDestination;
                        ExternalMoveMessage->Transform->data[3][2] = playerPosition.Z;
                    }
                    else
                    {
                        IsInPosition = 1;
                        return;
                    }
                }
            }

            if (!IsTimerEnabled)
            {
                xgame::MsgResumeGameTimer timerMessage{};
                if (!ObjUtil::SendMessageImmToGameActor(this, &timerMessage))
                    return;
                
                EggManager* eggManager = EggManager::GetService(*Document);
                if (eggManager)
                    eggManager->SetForceSpaceShrink(false);

                IsTimerEnabled = true;
            }

            if (Direction)
            {
                verticalDestination = targetPosition.Y + 14;

                if ((std::abs(playerPosition.Y - verticalDestination) < 23.65f))
                    PipeOutMotion(playerPosition, updateInfo);
                else
                    IsInPosition |= 0x10;
            }
            else
            {
                verticalDestination = targetPosition.Y;
                if ((std::abs(playerPosition.Y - verticalDestination) > 0.35f))
                    PipeOutMotion(playerPosition, updateInfo);
                else
                    IsInPosition |= 0x10;
            }

            if (!(IsInPosition & 0x10))
                return;

            IsInPosition |= 0x20;
            xgame::MsgCatchEndPlayer catchEndMessage { false };
            ObjUtil::SendMessageImmToPlayer(this, PlayerNumber, &catchEndMessage);

            xgame::MsgPLVisibleItemEffect visibleEffectMessage{ 1 };
            ObjUtil::SendMessageImmToPlayer(this, PlayerNumber, &visibleEffectMessage);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (!gocCollider)
                return;

            game::GOCCollider::SetEnable(gocCollider, false);
        }

        void PipeOutMotion(csl::math::Vector3 playerPos, const fnd::SUpdateInfo& updateInfo)
        {
            float currTime = Time;
            Time += updateInfo.deltaTime;
            if (currTime < 0.5f && 0.5f <= Time)
            {
                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (!gocSound)
                    return;

                int deviceTag[3]{};

                game::GOCSound::Play3D(gocSound, deviceTag, "obj_yossypipe_in_out", 0);
                IsNotMoving = true;
            }

            if (!IsNotMoving)
                return;

            if (Direction)
                playerPos.Y -= 0.3f;
            else
                playerPos.Y += 0.3f;

            ExternalMoveMessage->Transform->data[3][0] = playerPos.X;
            ExternalMoveMessage->Transform->data[3][1] = playerPos.Y;
            ExternalMoveMessage->Transform->data[3][2] = playerPos.Z;
        }
    };

    inline static ObjCrayPipeExit* create_ObjCrayPipeExit()
    {
        return new ObjCrayPipeExit();
    }
}