#pragma once

namespace app
{
    enum class ObjGossipStoneState : int
    {
        STATE_IDLE,
        STATE_SHAKE,
        STATE_COUNTDOWN,
        STATE_ROCKET,
    };

    struct ObjGossipStoneData
    {
        float CheckRadius;
    };

    class ObjGossipStone : public CSetObjectListener
    {
    public:
        int PlayerNumber{};
        fnd::MessageType MessageType{};
        INSERT_PADDING(8);
        ObjGossipStoneState State{};
        GameObjectHandleBase ObjectHandle{};
        Effect::CEffectHandle EffectHandle{};
        INSERT_PADDING(4);
        bool IsDamaged{};
        bool IsRocket{};
        bool IsAnimationSet{};
        INSERT_PADDING(1);
        float Time{};
        float RocketTime{};
        float VerticalPosition{};
        INSERT_PADDING(8);

        void Destructor(size_t deletingFlags)
        {
            CSetObjectListener::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCCollider);

            ObjGossipStoneInfo* info = (ObjGossipStoneInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjGossipStoneInfo");
            ObjGossipStoneData* data = (ObjGossipStoneData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            fnd::GOComponent::BeginSetup(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->BaseModel;
                visualDescriptor.Skeleton = info->BaseSkeleton;

                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
                if (gocAnimation)
                {
                    int animCount = 1;
                    game::GOCAnimationSimple::Setup(gocAnimation, &animCount);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    game::GOCAnimationSimple::Add(gocAnimation, "ACTION", info->BaseAnimation, 0);
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 3;
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                // Hit Collider
                game::ColliSphereShapeCInfo checkCollisionInfo;
                game::CollisionObjCInfo::__ct(&checkCollisionInfo);
                checkCollisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                checkCollisionInfo.MotionType = 2;
                checkCollisionInfo.Radius = data->CheckRadius;
                checkCollisionInfo.field_04 |= 1;
                checkCollisionInfo.field_0C = 0;
                ObjUtil::SetupCollisionFilter(7, &checkCollisionInfo);
                game::GOCCollider::CreateShape(gocCollider, &checkCollisionInfo);

                // Damage Collider
                game::ColliCapsuleShapeCInfo collisionInfo;
                csl::math::Vector3 colliderOffset { 0, 5, 0 };
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_CAPSULE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 8;
                collisionInfo.Height = 10;
                ObjUtil::SetupCollisionFilter(0, &collisionInfo);
                collisionInfo.field_04 |= 1;
                collisionInfo.field_0C = 1;
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &colliderOffset);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

                // Solid Collider
                colliderOffset = Vector3(0, 4, 0);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_CAPSULE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 6;
                collisionInfo.Height = 8;
                collisionInfo.field_04 |= 0x100;
                collisionInfo.field_08 = 0x40053;
                collisionInfo.field_0C = 2;
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &colliderOffset);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            game::GOCEffect::SimpleSetupEx(this, 2, 0);
            game::GOCSound::SimpleSetup(this, 0, 0);

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            MessageType = message.Type;
            switch (message.Type)
            {
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return true;
            case fnd::PROC_MSG_DAMAGE:
                ProcMsgDamage((xgame::MsgDamage&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == ObjGossipStoneState::STATE_SHAKE)
                StateShake();

            if (State == ObjGossipStoneState::STATE_COUNTDOWN)
                StateCountdown(updateInfo);

            if (State == ObjGossipStoneState::STATE_ROCKET)
                StateRocket(updateInfo);
        }

    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            if (ObjUtil::CheckShapeUserID(message.field_18, 0))
            {
                OnSnapShot(message);
                return;
            }
            
            if (!ObjUtil::CheckShapeUserID(message.field_18, 1))
                return;

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], message.ActorID);
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
            if (!playerInfo)
                return;

            if (((char*)playerInfo)[0x144])
                return;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;
            
            csl::math::Vector3 yVector = *(csl::math::Vector3*)(gocTransform + 0x48);
            csl::math::Vector3 contactPoint;
            message.GetContactPointNormal(&contactPoint);
            math::Vector3Scale(&contactPoint, -1, &contactPoint);
            float dot = math::Vector3DotProduct(&yVector, &contactPoint);
            math::Vector3Scale(&yVector, dot, &yVector);
            math::Vector3Subtract(&contactPoint, &yVector, &contactPoint);
            math::Vector3NormalizeZero(&contactPoint, &contactPoint);
            math::Vector3Scale(&contactPoint, 30, &contactPoint);
            math::Vector3Scale(&contactPoint, 2, &contactPoint);
            math::Vector3Add(&yVector, &contactPoint, &contactPoint);

            xgame::MsgAppeareKnockback knockbackMessage { contactPoint };
            SendMessageImm(message.ActorID, &knockbackMessage);

            switch (State)
            {
            case app::ObjGossipStoneState::STATE_IDLE:
            case app::ObjGossipStoneState::STATE_SHAKE:
            {
                // StateShake Enter
                State = ObjGossipStoneState::STATE_SHAKE;
                game::GOCAnimationSimple::SetAnimation(gocAnimation, "ACTION");

                int deviceTag[3];
                game::GOCSound::Play3D(gocSound, deviceTag, "obj_gossipstone_bibe", 0);

                break;
            }
            }

            message.field_0C = 1;
        }

        void ProcMsgDamage(xgame::MsgDamage& message)
        {
            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], message.field_08);
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
            if (!playerInfo)
                return;

            if (playerInfo[0x58] == -1)
            {
                int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
                if (!gocTransform)
                    return;

                float distance = math::Vector3DotProduct(&message.field_40, (csl::math::Vector3*)(gocTransform + 0x48));
                if (distance < 0.9f)
                {
                    if (message.field_54 == 3 && playerInfo && playerInfo[0x58] == 7)
                    {
                        switch (State)
                        {
                        case app::ObjGossipStoneState::STATE_IDLE:
                        {
                            State = ObjGossipStoneState::STATE_COUNTDOWN;
                            break;
                        }
                        case app::ObjGossipStoneState::STATE_SHAKE:
                        {
                            IsDamaged = true;
                            break;
                        }
                        }
                    }
                    else
                    {
                        switch (State)
                        {
                        case app::ObjGossipStoneState::STATE_IDLE:
                        {
                            // StateShake Enter
                            State = ObjGossipStoneState::STATE_SHAKE;
                            game::GOCAnimationSimple::SetAnimation(gocAnimation, "ACTION");

                            int deviceTag[3];
                            game::GOCSound::Play3D(gocSound, deviceTag, "obj_gossipstone_bibe", 0);

                            break;
                        }
                        case app::ObjGossipStoneState::STATE_SHAKE:
                        {
                            if (IsDamaged)
                                break;

                            game::GOCAnimationSimple::SetAnimation(gocAnimation, "ACTION");

                            int deviceTag[3];
                            game::GOCSound::Play3D(gocSound, deviceTag, "obj_gossipstone_bibe", 0);
                            break;
                        }
                        }
                    }

                    message.field_0C = 1;

                    message.SetReply(&message.field_30, 0);
                    message.field_90 |= 1;
                }
            }
            else
            {
                if (message.field_54 == 3 && playerInfo && playerInfo[0x58] == 7)
                {
                    switch (State)
                    {
                    case app::ObjGossipStoneState::STATE_IDLE:
                    {
                        State = ObjGossipStoneState::STATE_COUNTDOWN;
                        break;
                    }
                    case app::ObjGossipStoneState::STATE_SHAKE:
                    {
                        IsDamaged = true;
                        break;
                    }
                    }
                }
                else
                {
                    switch (State)
                    {
                    case app::ObjGossipStoneState::STATE_IDLE:
                    {
                        // StateShake Enter
                        State = ObjGossipStoneState::STATE_SHAKE;
                        game::GOCAnimationSimple::SetAnimation(gocAnimation, "ACTION");

                        int deviceTag[3];
                        game::GOCSound::Play3D(gocSound, deviceTag, "obj_gossipstone_bibe", 0);

                        break;
                    }
                    case app::ObjGossipStoneState::STATE_SHAKE:
                    {
                        if (IsDamaged)
                            break;

                        game::GOCAnimationSimple::SetAnimation(gocAnimation, "ACTION");

                        int deviceTag[3];
                        game::GOCSound::Play3D(gocSound, deviceTag, "obj_gossipstone_bibe", 0);
                        break;
                    }
                    }

                    message.field_0C = 1;
                }

                message.SetReply(&message.field_30, 0);
                message.field_90 |= 1;
                message.field_90 |= 32;
            }
        }

        void StateShake()
        {
            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (!gocCollider)
                return;

            if (!game::GOCAnimationSimple::IsFinished(gocAnimation))
                return;
        
            if (IsDamaged)
            {
                // StateCountdown Enter
                State = ObjGossipStoneState::STATE_COUNTDOWN;
                ObjUtil::SetEnableColliShape(gocCollider, 0, 0);
                Time = 0;
            }
            else
            {
                State = ObjGossipStoneState::STATE_IDLE;
            }
        }

        void StateCountdown(const fnd::SUpdateInfo& updateInfo)
        {
            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (!gocVisual)
                return;

            int deviceTag[3];
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;

            float oldTime = Time;
            Time += updateInfo.deltaTime;
            
            if (Time >= 5)
            {
                // StateRocket Enter
                State = ObjGossipStoneState::STATE_ROCKET;
                RocketTime = 0;
                VerticalPosition = 0;

                EffectHandle.Stop(0);
                game::GOCEffect::CreateEffectLoop(gocEffect, &EffectHandle, "ef_dl3_gossipstone_fire");

                game::GOCSound::Play3D(gocSound, deviceTag, "obj_gossipstone_rocket", 0);

                return;
            }

            if ((int)oldTime < (int)Time)
            {
                if ((int)Time == 4)
                {
                    float color[]{ 2, 0.5f, 0.5f, 1 };
                    fnd::GOCVisualModel::SetMaterialColor(gocVisual, color);

                    game::GOCSound::Play(gocSound, deviceTag, "obj_gossipstone_count1", 0);
                }
                else
                {
                    float color[]{ 0.55f, 0.55f, 0.55f, 1 };
                    fnd::GOCVisualModel::SetMaterialColor(gocVisual, color);

                    int deviceTag[3];
                    game::GOCSound::Play(gocSound, deviceTag, "obj_gossipstone_count2", 0);
                }

                return;
            }

            if (oldTime - (int)oldTime < 0.3f && Time - (int)Time >= 0.3f)
            {
                fnd::GOCVisualModel::SetMaterialColor(gocVisual, nullptr);
                
                if ((int)Time == 4)
                    game::GOCEffect::CreateEffectLoop(gocEffect, &EffectHandle, "ef_dl3_gossipstone_fog");
            }
        }

        void StateRocket(const fnd::SUpdateInfo& updateInfo)
        {
            RocketTime += updateInfo.deltaTime;
            VerticalPosition += 100 * updateInfo.deltaTime;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Vector3 upVec{ 0, VerticalPosition * updateInfo.deltaTime, 0 };
            csl::math::Vector3 position = MultiplyMatrixByVector((csl::math::Matrix34*)(gocTransform + 0x44), &upVec);
            fnd::GOCTransform::SetLocalTranslation(gocTransform, &position);
            if (RocketTime >= 5)
            {
                SetStatusRetire();
                GameObject::Kill();
            }
        }

        void OnSnapShot(xgame::MsgHitEventCollision& message)
        {
            if (ObjectHandle.IsValid())
                return;
        
            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            ObjGossipStoneFairy::CInfo* fairyInfo = new ObjGossipStoneFairy::CInfo(*(csl::math::Matrix34*)(gocTransform + 0x44));
            ObjGossipStoneFairy* fairy = ObjGossipStoneFairy::Create((GameDocument&)field_24[1], *fairyInfo);
            if (fairy)
                ObjectHandle = fairy;
            
            xgame::MsgDlcZeldaHeartAllRecovery recoverMessage{};
            int playerNo = ObjUtil::GetPlayerNo(field_24[1], message.ActorID);
            ObjUtil::SendMessageImmToPlayer(this, playerNo, &recoverMessage);

            int deviceTag[3];
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play3D(gocSound, deviceTag, "obj_gossipstone_check", 0);
        }
    };

    inline static ObjGossipStone* create_ObjGossipStone() { return new ObjGossipStone(); }

    inline static ObjGossipStoneInfo* createObjInfo_ObjGossipStoneInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjGossipStoneInfo();
    }
}