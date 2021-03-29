#pragma once

inline static bool IsShyGuyShadowOn;

namespace app
{
    struct EnemyShyGuyData
    {
        float Speed;
        float MaxMoveDistance;
        char IsEventDriven;
        char Direction;
        INSERT_PADDING(2);
        float DepthOffset;
    };

    class EnemyShyGuyInfo : public EnemyInfo
    {
    public:
        int Model;
        int Skeleton;
        animation::AnimationResContainer AnimationContainer;
        int AnimationLeft;
        int AnimationRight;

        EnemyShyGuyInfo()
        {
            PacfileName = "EnemyHeyho.pac";
            animation::AnimationResContainer::__ct(&AnimationContainer, (csl::fnd::IAllocator*)pAllocator);
        }

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            int animationScript[3]{};

            EnemyInfo::GetModelResource(this, &Model, "enm_heyho");
            EnemyInfo::GetSkeletonResource(this, &Skeleton, "enm_heyho");

            EnemyInfo::GetEnemyPackfile(this, &packFile);
            ObjUtil::GetAnimationScriptResource(animationScript, "shyguy", packFile);
            animationScript[1] = Skeleton;

            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&AnimationContainer, animationScript, packFile);

            EnemyInfo::GetAnimationResource(this, &AnimationLeft, "hey_walk_left_loop");
            EnemyInfo::GetAnimationResource(this, &AnimationRight, "hey_walk_right_loop");
        }

        const char* GetInfoName() override
        {
            return "EnemyShyGuyInfo";
        }
    };

    class EnemyShyGuy : public EnemyBase
    {
    public:
        int Flags{};
        int field_4D4{};
        int field_4D8{};
        int field_4DC{};

        EnemyShyGuy()
        {
            ObjUtil::SetPropertyLockonTarget(this);
        }
    
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCGravity);
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationScript);
            fnd::GOComponent::Create(this, GOCShadowSimple);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCEnemyHSM);
            fnd::GOComponent::Create(this, GOCMovementComplex);

            EnemyShyGuyInfo* info = (EnemyShyGuyInfo*)ObjUtil::GetObjectInfo(gameDocument, "EnemyShyGuyInfo");
            EnemyShyGuyData* data = (EnemyShyGuyData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            if (data->IsEventDriven)
                Flags |= 1;
            if (data->Direction)
                Flags |= 4;

            fnd::GOComponent::BeginSetup(this);

            csl::math::Vector3 reticlePosition { 0, 5, 0 };
            EnemyBase::CreateCenterPositionFrame(this, &reticlePosition);
            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.Animation |= 0x400000;
                visualDescriptor.ZIndex = data->DepthOffset;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
                if (gocAnimation)
                {
                    animation::AnimationResContainer* animation = &(info->AnimationContainer);

                    game::GOCAnimationScript::Setup(gocAnimation, &animation);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);

                    csl::fnd::IAllocator* allocator{};
                    auto funcPtr = &EnemyShyGuy::SoundCallback;
                    animation::AnimCallbackBridge<EnemyShyGuy>* callback = (animation::AnimCallbackBridge<EnemyShyGuy>*)AnimCallbackBridge_Initialize(allocator);
                    callback->GameObject = this;
                    callback->field_10 = reinterpret_cast<void*&>(funcPtr);
                    callback->field_14 = -1;

                    game::GOCAnimationScript::RegisterCallback(gocAnimation, 1, callback);

                    game::GOCAnimationScript::SetAnimation(gocAnimation, "WALK_L");
                }
            }

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                game::ShadowSphereShapeCInfo shadowInfo;

                game::ShadowSphereShapeCInfo::__ct(&shadowInfo, 4);

                game::ShadowSphereShapeCInfo* ppShadowInfo = &shadowInfo;
                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                app::game::ColliSphereShapeCInfo collisionInfo;

                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.field_48 = 0;
                collisionInfo.field_44 = 0;
                collisionInfo.Radius = 4;
                ObjUtil::SetupCollisionFilter(9, &collisionInfo);
                collisionInfo.field_08 = 0x20000;
                collisionInfo.field_04 |= 1;
                collisionInfo.Parent = EnmBase::GetCenterPositionFrame(this);

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            game::GOCGravity::SimpleSetup(this, 1);

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (gocMovement)
            {
                game::MoveStraight::MoveParameter moveParameter{};

                CSetAdapter::GetPosition(*(int**)((char*)this + 0x324), &moveParameter.Position);
                CSetAdapter::GetRotation(*(int**)((char*)this + 0x324), &moveParameter.Rotation);
                moveParameter.field_20 = 100;
                moveParameter.field_24 = 4;
                moveParameter.field_28 = 0x101;
                moveParameter.field_2A = 0;
                moveParameter.field_30.Y = 5;
                moveParameter.field_30.Z = 6;
                moveParameter.field_40.Z = 10;
                moveParameter.field_50.Z = 3;
                moveParameter.field_60 = 10;

                void* moveStraight = ((csl::fnd::IAllocator*)(((int**)gocMovement)[2]))->Alloc(240, 16);
                int* ms = game::MoveStraight::__ct(moveStraight);
                game::GOCMovement::SetupController(gocMovement, moveStraight);
                game::MoveStraight::SetupParamater(moveStraight, &moveParameter);
                game::MoveStraight::SetMoveDistance(moveStraight, data->MaxMoveDistance, 0.0);

                auto funcPtr = &EnemyShyGuy::NotifyMovementStopCallback;
                game::MoveStraight::FunctionPair functions{ (void*)ASLR(0x0070F590), reinterpret_cast<void*&>(funcPtr) };
                game::MoveStraight::unkStruct unknown{ 0, data->MaxMoveDistance };
                game::MoveStraight::SetNotifyStopCallback(moveStraight, functions, unknown, 0);

                int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
                *((float*)(contextParam + 8)) = data->Speed * -1;
                if ((Flags & 4) == 4)
                    *((float*)(contextParam + 8)) = data->Speed;
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

            int* gocEnemyHsm = GameObject::GetGOC(this, GOCEnemyHsmString);
            if (gocEnemyHsm)
            {
                GOCEnemyHsm::Description hsmDescription{ StateDescriptors, 3, -1 };
                if (Flags & 1)
                    hsmDescription.field_08 = 2;
                else
                    hsmDescription.field_08 = 0;

                GOCEnemyHsm::Setup(gocEnemyHsm, &hsmDescription);
            }

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::MessageNew& message) override
        {
            if (PreProcessMessage(message))
                return true;

            switch (message.Type)
            {
            case fnd::PROC_MSG_DAMAGE:
                ProcMsgDamage((xgame::MsgDamage&)message);
                return true;
            case fnd::PROC_MSG_KICK:
                ProcMsgKick((xgame::MsgKick&)message);
                return true;
            case fnd::PROC_MSG_NOTIFY_OBJECT_EVENT:
                ProcMsgNotifyObjectEvent((xgame::MsgNotifyObjectEvent&)message);
                return true;
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return true;
            default:
                EnemyBase::ProcessMessage(message);
                return true;
            }
        }

        class State
        {
        public:
            class Move
            {
                char field_00[20];
            public:
                virtual ~Move() {};
                virtual int Trigger(EnemyShyGuy* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyShyGuy* obj) { return 0; };
                virtual int Enter(EnemyShyGuy* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyShyGuy* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyShyGuy* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyShyGuy* obj, int a2) { return 0; };

                virtual int OnEnter(EnemyShyGuy* obj, int a2)
                {
                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (gocAnimation)
                    {
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "WALK_L");
                        if ((obj->Flags & 4) == 4)
                            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "WALK_R");
                    }
                    return 1;
                };

                virtual int OnLeave(EnemyShyGuy* obj, int a2) { return 0; };

                virtual int Step(EnemyShyGuy* obj, float a2)
                {
                    if ((obj->Flags & 2) == 2)
                    {
                        int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                        if (gocEnemyHsm)
                            GOCEnemyHsm::ChangeState(gocEnemyHsm, 1);
                    }
                    return 1;
                };
            };

            class Stop
            {
                char field_00[20];
            private:
                int ProcMsgNotifyObjectEvent(EnemyShyGuy* obj, int stateID)
                {
                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (gocAnimation)
                    {
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "WALK_L");
                        if ((obj->Flags & 4) == 4)
                            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "WALK_R");
                    }

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (gocEnemyHsm)
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, stateID);

                    return 1;
                }

            public:
                virtual ~Stop() {};
                virtual int Trigger(EnemyShyGuy* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyShyGuy* obj) { return 0; };
                virtual int Enter(EnemyShyGuy* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyShyGuy* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyShyGuy* obj, float a2) { return EnemyState::Update(this, obj, a2); };

                virtual int ProcessMessage(EnemyShyGuy* obj, fnd::Message* message)
                {
                    if (message->field_04 == fnd::PROC_MSG_NOTIFY_OBJECT_EVENT)
                        return ProcMsgNotifyObjectEvent(obj, 0);
                    else
                        return 0;
                };

                virtual int OnEnter(EnemyShyGuy* obj, int a2)
                {
                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (gocAnimation)
                    {
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_L");
                        if ((obj->Flags & 4) == 4)
                            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_R");
                    }

                    int* gocMovement = GameObject::GetGOC(obj, GOCMovementString);
                    if (gocMovement)
                        game::GOCMovement::DisableMovementFlag(gocMovement, false);

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (gocEnemyHsm)
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 0);

                    return 1;
                };

                virtual int OnLeave(EnemyShyGuy* obj, int a2)
                {
                    int* gocMovement = GameObject::GetGOC(obj, GOCMovementString);
                    if (gocMovement)
                        game::GOCMovement::EnableMovementFlag(gocMovement, false);

                    return 1;
                };

                virtual int Step(EnemyShyGuy* obj, float a2)
                {
                    if (!(obj->Flags & 2))
                    {
                        int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                        if (gocAnimation)
                        {
                            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_L");
                            if ((obj->Flags & 4) == 4)
                                game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_R");
                        }
                    }

                    return 1;
                };
            };

            class Turnaround
            {
                char field_00[20];
                int SubStateID;
                float field_18;
                int field_1C;
                csl::math::Quaternion Rotation;
                int field_30;
                int field_34;
                int field_38;
                int field_3C;
            private:
                void ChangeSubState(int stateID)
                {
                    SubStateID = stateID;
                    field_18 = 0;
                }

            public:
                virtual ~Turnaround() {};
                virtual int Trigger(EnemyShyGuy* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyShyGuy* obj) { return 0; };
                virtual int Enter(EnemyShyGuy* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyShyGuy* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyShyGuy* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyShyGuy* obj, int a2) { return 0; };

                virtual int OnEnter(EnemyShyGuy* obj, int a2)
                {
                    ChangeSubState(0);
                    field_1C = 0;
                    field_30 = 1;
                    obj->Flags &= ~(1 << 1);

                    int* gocTransform = GameObject::GetGOC(obj, GOCTransformString);
                    if (gocTransform)
                        Rotation = *(csl::math::Quaternion*)(gocTransform + 0x1C);

                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (gocAnimation)
                    {
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_L");
                        if ((obj->Flags & 4) == 4)
                            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_R");
                    }

                    return 1;
                };

                virtual int OnLeave(EnemyShyGuy* obj, int a2)
                {
                    EnemyShyGuyData* data = (EnemyShyGuyData*)CSetAdapter::GetData(*(int**)((char*)obj + 0x324));

                    int* gocMovement = GameObject::GetGOC(obj, GOCMovementString);
                    if (!gocMovement)
                        return 0;

                    int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
                    *((float*)(contextParam + 8)) = data->Speed * -1;
                    if ((obj->Flags & 4) == 4)
                        *((float*)(contextParam + 8)) = data->Speed;

                    game::GOCMovement::EnableMovementFlag(gocMovement, false);

                    return 1;
                };

                virtual int Step(EnemyShyGuy* obj, float a2)
                {
                    field_18 += a2;
                    switch (SubStateID)
                    {
                    case 0:
                    {
                        if (0.3 <= field_18)
                        {
                            field_30 = 0;
                            ChangeSubState(1);
                        }
                        break;
                    }
                    case 1:
                    {
                        int* gocTransform = GameObject::GetGOC(obj, GOCTransformString);

                        if (!field_30)
                        {
                            field_30 = 1;
                            int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                            if (gocAnimation)
                            {
                                game::GOCAnimationScript::ChangeAnimation(gocAnimation, "LOOKAROUND_R");
                                if ((obj->Flags & 4) == 4)
                                    game::GOCAnimationScript::ChangeAnimation(gocAnimation, "LOOKAROUND_L");
                            }
                        }

                        if (!gocTransform)
                            return 0;

                        if (field_18 > 0.11)
                        {
                            obj->Flags ^= 4;
                            ChangeSubState(2);
                        }
                        else
                        {
                            if (field_18 > 1)
                                field_18 = 1;
                            else if (field_18 < 0)
                                field_18 = 0;

                            csl::math::Quaternion multiplier{ 0, 0.259f, 0, 0.966f };
                            if ((obj->Flags & 4) == 4)
                                multiplier.Y = -0.259f;

                            csl::math::Quaternion rotation;
                            csl::math::Quaternion::QuaternionMultiply(&Rotation, &Rotation, &multiplier);
                            fnd::GOCTransform::SetLocalRotation(gocTransform, &Rotation);
                        }
                        break;
                    }

                    case 2:
                    {
                        if (0.3 <= field_18)
                        {
                            field_1C += 1;
                            if (field_1C >= 3)
                            {
                                int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                                if (!gocAnimation)
                                    return 0;

                                game::GOCAnimationScript::ChangeAnimation(gocAnimation, "LOOKAROUND_R");
                                if ((obj->Flags & 4) == 4)
                                    game::GOCAnimationScript::ChangeAnimation(gocAnimation, "LOOKAROUND_L");

                                int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                                if (gocEnemyHsm)
                                    GOCEnemyHsm::ChangeState(gocEnemyHsm, 0);
                            }
                            else
                            {
                                field_30 = 0;
                                ChangeSubState(1);
                            }
                        }

                        break;
                    }
                    }
                    return 1;
                };
            };
        };
    private:
        void ProcMsgDamage(xgame::MsgDamage& message)
        {
            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Vector3 translation{};
            enemy::DeadEffectCInfo effectInfo{};

            math::CalculatedTransform::GetTranslation((csl::math::Matrix34*)(gocTransform + 0x44), &translation);
            xgame::MsgDamage::SetReply(&message, &translation, 1);
            ObjUtil::AddScore(this, "SHYGUY", &message);

            enemy::DeadEffectCInfo::__ct(&effectInfo);
            GameObjectHandleBase::__ct(&effectInfo, this);
            enemy::DeadEffectCInfo::SetMsgDamage(&effectInfo, &message);
            enemy::DeadEffectCInfo::SetYoshiIsland(&effectInfo);

            math::Transform transform{};
            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::GetNodeTransform(gocVisual, 0, "Spine", &transform);
                effectInfo.field_20.data[3][0] = transform.Position.X;
                effectInfo.field_20.data[3][1] = transform.Position.Y;
                effectInfo.field_20.data[3][2] = transform.Position.Z;
                effectInfo.field_62 |= 1;
            }

            void* enemyManager = EnemyManager::GetService((GameDocument*)field_24[1]);
            EnemyManager::CreateDeadEffect(enemyManager, &effectInfo);
            EnemyBase::ProcMission(this, &message);
            CSetObjectListener::SetStatusRetire(this);
            GameObject::Kill(this);
        }

        void ProcMsgKick(xgame::MsgKick& message)
        {
            EnemyBlowOffObjectCInfo blowOffInfo;
            EnemyShyGuyInfo* info = (EnemyShyGuyInfo*)ObjUtil::GetObjectInfo(*Document, "EnemyShyGuyInfo");
            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            EnemyBlowOffObjectCInfo::__ct(&blowOffInfo);
            EnemyBlowOffObjectCInfo::SetParamByMessage(&blowOffInfo, &message);
            blowOffInfo.Model = info->Model;
            blowOffInfo.Skeleton = info->Skeleton;
            blowOffInfo.Animation = info->AnimationLeft;
            if ((Flags & 4) == 4)
                blowOffInfo.Animation = info->AnimationRight;
            blowOffInfo.field_10 = *(csl::math::Matrix34*)(gocTransform + 0x44);
            blowOffInfo.field_50.Y = 5;
            blowOffInfo.field_60 = 4;
            blowOffInfo.field_6C = 3;
            EnmBase::CreateEnemyBlowOffObject(this, &blowOffInfo);
            xgame::MsgKick::SetReplyForSucceed(&message);
            ObjUtil::AddScore(this, "SHYGUY", &message);
            CSetObjectListener::SetStatusRetire(this);
            GameObject::Kill(this);
        }

        void ProcMsgNotifyObjectEvent(xgame::MsgNotifyObjectEvent& message)
        {
            if (!(Flags & 1))
                return;
        
            int* gocEnemyHsm = GameObject::GetGOC(this, GOCEnemyHsmString);
            if (gocEnemyHsm)
                GOCEnemyHsm::Dispatch(gocEnemyHsm, &message);
        }

        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            EnemyBase::SendTouchDamage(this, (xgame::MsgDamage&)message);
        }

        void NotifyMovementStopCallback()
        {
            Flags |= 2;
        }

        void SoundCallback(int a1, int a2, int a3)
        {
            int* gocSound = GameObject::GetGOC((GameObject*)((char*)this + 1), GOCSoundString);
            int deviceTag[3]{};
            if (!gocSound)
                return;

            if (!a2)
            {
                if (a3 == 1)
                {
                    game::GOCSound::Play3D(gocSound, deviceTag, "enm_heyho_foot", 0);
                    game::GOCSound::Play3D(gocSound, deviceTag, "enm_heyho_voice_hey", 0);
                }
                else
                {
                    game::GOCSound::Play3D(gocSound, deviceTag, "enm_heyho_foot", 0);
                    game::GOCSound::Play3D(gocSound, deviceTag, "enm_heyho_voice_ho", 0);
                }
            }
        }

        inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new animation::AnimCallbackBridge<EnemyShyGuy>();
        }

        inline static void* Move_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyShyGuy::State::Move();
        }

        inline static void* Stop_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyShyGuy::State::Stop();
        }

        inline static void* Turnaround_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyShyGuy::State::Turnaround();
        }

        inline static ut::internal::StateDescImpl States[] =
        {
            { "Move",& Move_Initialize, -1 },
            { "Turnaround", &Turnaround_Initialize, -1 },
            { "Stop",& Stop_Initialize, -1 }
        };
        
        inline static app::GOCEnemyHsm::StateDesc StateDescriptors[] =
        {
            {0, &States[0]},
            {1, &States[1]},
            {2, &States[2]},
        };
    };

    inline static EnemyShyGuy* create_EnemyShyGuy()
    {
        return new EnemyShyGuy();
    }

    inline static EnemyShyGuyInfo* createObjInfo_EnemyShyGuyInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) EnemyShyGuyInfo();
    }
}