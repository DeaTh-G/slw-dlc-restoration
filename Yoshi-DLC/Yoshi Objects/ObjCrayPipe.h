#pragma once

namespace app
{
    struct ObjCrayPipeData
    {
        float CollisionRadius;
        int TargetID;
        int Direction;
    };

    class ObjCrayPipe
    {
    public:
        CSetObjectListener* __ct()
        {
            CSetObjectListener::__ct((GameObject*)this);
            *(int*)this = ASLR(0x00D94740);
            *((int*)this + 2) = ASLR(0x00D94724);

            /* isWarping */
            *((bool*)(this + 0x3F0)) = false;

            /* inwardsDirection */
            *((bool*)(this + 0x3F4)) = false;

            /* isAudioPlaying */
            *((bool*)(this + 0x3F8)) = true;

            /* isMessageProcessed */
            *((bool*)(this + 0x3F9)) = true;

            return (CSetObjectListener*)this;
        }

        void AddCallback(GameDocument* gameDocument)
        {
            app::game::ColliSphereShapeCInfo collisionInfo;
            int unit = 1;

            fnd::GOComponent::Create((GameObject*)this, GOCCollider);
            fnd::GOComponent::Create((GameObject*)this, GOCSound);

            fnd::GOComponent::BeginSetup((GameObject*)this);
            ObjCrayPipeData* data = (ObjCrayPipeData*)CSetAdapter::GetData(*(int**)(this + 0x324));
            *((bool*)(this + 0x3F4)) = data->Direction;

            game::GOCSound::SimpleSetup((GameObject*)this, 0, 0);
            
            /* Collider */
            int* gocCollider = app::GameObject::GetGOC((GameObject*)this, GOCColliderString);
            if (gocCollider)
            {
                app::game::GOCCollider::Setup(gocCollider, &unit);
                app::game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = data->CollisionRadius;
                collisionInfo.field_54 = 0;
                collisionInfo.field_44 = 0;
                collisionInfo.field_48 = 0;
                app::ObjUtil::SetupCollisionFilter(7, &collisionInfo);
                collisionInfo.field_04 |= 4;

                app::game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            fnd::GOComponent::EndSetup((GameObject*)this);
        }

        bool ProcessMessage(fnd::Message* message)
        {
            switch (message->field_04)
            {
            case fnd::PROC_MSG_STAY_TRIGGER:
            {
                if (*((bool*)(this + 0x3F1)))
                    StateIdle(message);
                return true;
            }
            case fnd::PROC_MSG_KILL:
            {
                /* These are purely dependant on the object despawning after the teleporting.
                It works for the Yoshi DLC warp points as they are outside of the RangeOut
                values for these objects but, if used outside you will need to make the
                warp points be outside of the RangeOut distance.*/
                *((bool*)(this + 0x3E8)) = false;
                *((bool*)(this + 0x3F0)) = true;
                *((bool*)(this + 0x3F1)) = true;
                return true;
            }
            case fnd::PROC_MSG_GET_EXTERNAL_MOVE_POSITION:
            {
                int* gocTransform = GameObject::GetGOC((GameObject*)(this - 8), GOCTransformString);
                if (gocTransform)
                {
                    int playerNo = ObjUtil::GetPlayerNo(*(int*)(this + 32), ((int*)message)[2]);
                    int* playerInfo = ObjUtil::GetPlayerInformation(*Document, playerNo);
                    csl::math::Vector3 playerPosition = *(csl::math::Vector3*)(playerInfo + 4);
                    csl::math::Vector3 targetPosition = *(csl::math::Vector3*)(gocTransform + 0x50);
                    float verticalDestination = 0;

                    if (*((bool*)(this + 0x3EC)))
                        verticalDestination = targetPosition.Y + 12;
                    else
                        verticalDestination = targetPosition.Y - 12;
                    
                    if (std::abs(playerPosition.X - targetPosition.X) > 0.55f)
                    {
                        if (playerPosition.X < targetPosition.X)
                            playerPosition.X += 0.5f;
                        else
                            playerPosition.X -= 0.5f;

                        ((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][0] = playerPosition.X;
                        ((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][1] = playerPosition.Y;
                        ((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][2] = playerPosition.Z;
                    }
                    else
                    {
                        if ((std::abs(playerPosition.Y - verticalDestination) > 0.35f))
                        {
                            if (*((bool*)(this + 0x3EC)))
                                playerPosition.Y += 0.3f;
                            else
                                playerPosition.Y -= 0.3f;

                            ((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][0] = playerPosition.X;
                            ((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][1] = playerPosition.Y;
                            ((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][2] = playerPosition.Z;

                            if (*((bool*)(this + 0x3F0)))
                            {
                                xgame::MsgPLVisibleItemEffect playerVisibleItemEffectMessage;
                                int* gocSound = GameObject::GetGOC((GameObject*)(this - 8), GOCSoundString);
                                if (gocSound)
                                {
                                    int deviceTag[3]{};

                                    app::game::GOCSound::Play3D(gocSound, deviceTag, "obj_yossypipe_in_out", 0);
                                    *((bool*)(this + 0x3F0)) = false;
                                }

                                fnd::Message::__ct(&playerVisibleItemEffectMessage.Base, fnd::PROC_MSG_PL_VISIBLE_ITEM_EFFECT);
                                playerVisibleItemEffectMessage.field_18 = 0;
                                ObjUtil::SendMessageImmToPlayer((GameObject*)(this - 8), playerNo, (int*)&playerVisibleItemEffectMessage);
                                xgame::MsgExtendPlayer::__dt((int*)&playerVisibleItemEffectMessage);
                            }
                        }
                        else
                        {
                            if (!*((bool*)(this + 0x3E8)))
                            {
                                StateWarp(message);
                                *((bool*)(this + 0x3E8)) = true;
                            }
                        }
                    }
                }
                return true;
            }
            default:
            {
                CSetObjectListener::f_ProcessMessage((CSetObjectListener*)this, message);
                return true;
            }
            }
        }

    private:
        void StateIdle(fnd::Message* parentMessage)
        {
            switch (parentMessage->field_04)
            {
            case fnd::PROC_MSG_STAY_TRIGGER:
            {
                xgame::MsgCatchPlayer catchPlayerMessage;
                xgame::MsgPLGetInputButton playerGetInputButtonMessage;

                bool isGrounded = true;

                int direction = 0;

                int playerNo = ObjUtil::GetPlayerNo(*(int*)(this + 32), ((int*)parentMessage)[8]);
                int* playerInfo = ObjUtil::GetPlayerInformation(*Document, playerNo);
                if (playerInfo)
                {
                    isGrounded = *((char*)(playerInfo + 0x51));

                    /* Direction is downwards. */
                    direction = 2;

                    /* Direction is upwards. */
                    if (!isGrounded)
                        direction = 1;
                }

                fnd::Message::__ct((fnd::Message*)&playerGetInputButtonMessage, fnd::PROC_MSG_PL_GET_INPUT_BUTTON);
                playerGetInputButtonMessage.field_18 = direction;
                playerGetInputButtonMessage.field_1C = 1;
                playerGetInputButtonMessage.field_20 = 0;

                if (ObjUtil::SendMessageImmToPlayer((GameObject*)(this - 8), playerNo, (int*)&playerGetInputButtonMessage)
                    && playerGetInputButtonMessage.field_20 != 0)
                {
                    if (*((bool*)(this + 0x3F1)) == false)
                        return;

                    *((bool*)(this + 0x3F1)) = false;

                    fnd::Message::__ct((fnd::Message*)&catchPlayerMessage, fnd::PROC_MSG_CATCH_PLAYER);
                    catchPlayerMessage.field_18 = 0;
                    catchPlayerMessage.field_60 = 0x12;
                    catchPlayerMessage.field_64 = 0;
                    if (ObjUtil::SendMessageImmToPlayer((GameObject*)(this - 8), playerNo, (int*)&catchPlayerMessage))
                        StatePipeIn(parentMessage);

                    xgame::MsgExtendPlayer::__dt((int*)&catchPlayerMessage);
                }
                xgame::MsgExtendPlayer::__dt((int*)&playerGetInputButtonMessage);
            }
            default:
                break;
            }
        }

        void StatePipeIn(fnd::Message* parentMessage)
        {
            xgame::MsgStopGameTimer stopGameTimerMessage;

            // Stopping Game Timer
            fnd::Message::__ct(&stopGameTimerMessage.Base, fnd::PROC_MSG_STOP_GAME_TIMER);
            if (ObjUtil::SendMessageImmToGameActor((GameObject*)(this - 8), (int*)&stopGameTimerMessage))
            {
                EggManager* eggManager = EggManager::GetService(*Document);
                if (eggManager)
                    eggManager->SetForceSpaceShrink(true);

                xgame::MsgExtendPlayer::__dt((int*)&stopGameTimerMessage);
            }
        }

        void StateWarp(fnd::Message* parentMessage)
        {
            csl::math::Vector3 targetPosition;
            csl::math::Quaternion targetRotation;
            xgame::MsgNotifyObjectEvent notifyObjevtEventMessage;
            xgame::MsgWarpNewArea warpNewAreaMessage;

            ObjCrayPipeData* data = (ObjCrayPipeData*)CSetAdapter::GetData(*(int**)(this + 0x31C));

            if (ObjUtil::GetSetObjectTransform((GameDocument*)*Document,
                &data->TargetID, &targetPosition, &targetRotation))
            {
                fnd::Message::__ct((fnd::Message*)&notifyObjevtEventMessage, fnd::PROC_MSG_NOTIFY_OBJECT_EVENT);
                notifyObjevtEventMessage.field_18 = 1;
                ObjUtil::SendMessageImmToSetObject((GameObject*)(this - 8), &data->TargetID, (int*)&notifyObjevtEventMessage, 0);

                fnd::Message::__ct(&warpNewAreaMessage.Base, fnd::PROC_MSG_WARP_NEW_AREA);
                warpNewAreaMessage.field_18 = ObjUtil::GetPlayerNo(*(int*)(this + 32), ((int*)parentMessage)[2]);
                warpNewAreaMessage.IsDestination2D = true;
                warpNewAreaMessage.TargetPosition = targetPosition;
                warpNewAreaMessage.TargetRotation = targetRotation;
                warpNewAreaMessage.field_40 = 5;
                warpNewAreaMessage.field_45 = 1;
                ObjUtil::SendMessageImmToGameActor((GameObject*)(this - 8), (int*)&warpNewAreaMessage);

                xgame::MsgExtendPlayer::__dt((int*)&notifyObjevtEventMessage);
                xgame::MsgExtendPlayer::__dt((int*)&warpNewAreaMessage);
            }
        }
    };

    inline static GameObject* create_ObjCrayPipe()
    {
        GameObject* object = GameObject::New(0x400);
        if (!object)
            return 0;
        ((ObjCrayPipe*)object)->__ct();
        return object;
    }
}