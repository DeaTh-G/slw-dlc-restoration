#pragma once

namespace app
{
    enum class ObjCrayPipeState : int
    {
        STATE_IDLE,
        STATE_PIPE_IN,
        STATE_WARP
    };

    struct ObjCrayPipeData
    {
        float CollisionRadius;
        int TargetID;
        int Direction;
    };

    class ObjCrayPipe : public CSetObjectListener
    {
        INSERT_PADDING(8);
        ObjCrayPipeState State{};
        xgame::MsgGetExternalMovePosition* ExternalMoveMessage = new xgame::MsgGetExternalMovePosition();
        xgame::MsgStayTrigger* StayMessage = new xgame::MsgStayTrigger();
        int PlayerNumber{};
        int field_3B8{};
        int field_3BC{};
        Matrix34 field_3C0;
        float field_400{};
        float Time;
        char Direction;
        bool IsAudioPlaying{};
        INSERT_PADDING(2);
        int field_40C{};

    public:
        ObjCrayPipe()
        {
            State = ObjCrayPipeState::STATE_IDLE;
            field_3C0.data[0][0] = 1;
            field_3C0.data[1][1] = 1;
            field_3C0.data[2][2] = 1;
            field_3C0.data[3][3] = 1;
            Time = 0.0166666675359f;
            Direction = 1;
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCSound);

            ObjCrayPipeData* data = (ObjCrayPipeData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            Direction = data->Direction == 0;

            fnd::GOComponent::BeginSetup(this);
            
            int* gocCollider = app::GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                app::game::ColliSphereShapeCInfo collisionInfo;

                app::game::GOCCollider::Setup(gocCollider, &shapeCount);
                app::game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = data->CollisionRadius;
                app::ObjUtil::SetupCollisionFilter(7, &collisionInfo);
                collisionInfo.field_04 |= 4;

                app::game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
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
            StatePipeIn();
            StateWarp();
        }

    private:
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
            if (State != ObjCrayPipeState::STATE_IDLE)
                return;

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], StayMessage->field_20);
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
            if (!playerInfo)
                return;

            PlayerNumber = playerNo;

            if (Direction && !*((char*)playerInfo + 0x144))
                return;

            int direction = 1;
            if (Direction)
                direction = 2;

            xgame::MsgPLGetInputButton buttonMessage{ direction, 1 };
            if (!ObjUtil::SendMessageImmToPlayer(this, playerNo, &buttonMessage)
                || !buttonMessage.IsPressed)
                return;

            xgame::MsgCatchPlayer catchMessage { 18 };
            if (ObjUtil::SendMessageImmToPlayer(this, playerNo, &catchMessage))
                State = ObjCrayPipeState::STATE_PIPE_IN;
        }

        void StatePipeIn()
        {
            if (State != ObjCrayPipeState::STATE_PIPE_IN)
                return;

            xgame::MsgStopGameTimer stopTimerMessage{};
            if (ObjUtil::SendMessageImmToGameActor(this, &stopTimerMessage))
            {
                EggManager* eggManager = EggManager::GetService(*Document);
                if (eggManager)
                    eggManager->SetForceSpaceShrink(true);
            }

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], ExternalMoveMessage->field_08);
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
            if (!playerInfo)
                return;

            csl::math::Vector3 playerPosition = *(csl::math::Vector3*)(playerInfo + 4);
            csl::math::Vector3 targetPosition = *(csl::math::Vector3*)(gocTransform + 0x50);
            float verticalDestination = 0;

            if (Direction)
                verticalDestination = targetPosition.Y - 12;
            else
                verticalDestination = targetPosition.Y + 12;

            if (std::abs(playerPosition.X - targetPosition.X) > 0.55f)
            {
                if (playerPosition.X < targetPosition.X)
                    playerPosition.X += 0.5f;
                else
                    playerPosition.X -= 0.5f;

                ExternalMoveMessage->Transform->data[3][0] = playerPosition.X;
                ExternalMoveMessage->Transform->data[3][1] = playerPosition.Y;
                ExternalMoveMessage->Transform->data[3][2] = playerPosition.Z;
            }
            else
            {
                if ((std::abs(playerPosition.Y - verticalDestination) > 0.35f))
                {
                    if (Direction)
                        playerPosition.Y -= 0.3f;
                    else
                        playerPosition.Y += 0.3f;

                    ExternalMoveMessage->Transform->data[3][0] = playerPosition.X;
                    ExternalMoveMessage->Transform->data[3][1] = playerPosition.Y;
                    ExternalMoveMessage->Transform->data[3][2] = playerPosition.Z;

                    if (IsAudioPlaying)
                        return;

                    xgame::MsgPLVisibleItemEffect visibleEffectMessage{};
                    ObjUtil::SendMessageImmToPlayer(this, playerNo, &visibleEffectMessage);

                    int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                    if (!gocSound)
                        return;

                    int deviceTag[3]{};
                    app::game::GOCSound::Play3D(gocSound, deviceTag, "obj_yossypipe_in_out", 0);
                    IsAudioPlaying = true;
                }
                else
                {
                    State = ObjCrayPipeState::STATE_WARP;
                }
            }
        }

        void StateWarp()
        {
            if (State != ObjCrayPipeState::STATE_WARP)
                return;

            ObjCrayPipeData* data = (ObjCrayPipeData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            csl::math::Vector3 targetPosition;
            csl::math::Quaternion targetRotation;
            if (!ObjUtil::GetSetObjectTransform(*Document, &data->TargetID,
                &targetPosition, &targetRotation))
                return;

            xgame::MsgNotifyObjectEvent notifyMessage{ 1 };
            ObjUtil::SendMessageImmToSetObject(this, &data->TargetID, &notifyMessage, 0);

            xgame::MsgWarpNewArea warpMessage{ PlayerNumber, true, targetPosition, targetRotation, 5, 1 };
            ObjUtil::SendMessageImmToGameActor(this, &warpMessage);
        }
    };

    inline static ObjCrayPipe* create_ObjCrayPipe()
    {
        return new ObjCrayPipe();
    }
}