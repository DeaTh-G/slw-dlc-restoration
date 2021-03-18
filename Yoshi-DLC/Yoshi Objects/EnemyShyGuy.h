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
        char field_0A;
        char field_0B;
        float DepthOffset;
    };

    class EnemyShyGuyInfo
    {
    public:
        int field_00;
        int field_04;
        int field_08;
        int field_0C;
        const char* Pacfile;
        int field_14;
        int Model;
        int Skeleton;
        animation::AnimationResContainer AnimationContainer;
        int AnimationLeft;
        int AnimationRight;

        EnemyShyGuyInfo* __ct(EnemyShyGuyInfo* a1)
        {
            CObjInfo::__ct((CObjInfo*)a1);
            a1->Pacfile = "EnemyHeyho.pac";
            a1->field_14 = 0;
            a1->field_00 = ASLR(0x00D93A44);
            a1->Model = 0;
            a1->Skeleton = 0;
            animation::AnimationResContainer::__ct(&(a1->AnimationContainer), a1->field_08);
            a1->AnimationLeft = 0;
            a1->AnimationRight = 0;

            return a1;
        }

        void Initialize(GameDocument* gameDocument)
        {
            int packFile = 0;
            int animationScript = 0;
            int animation = 0;

            EnemyInfo::GetModelResource((int*)this, &(this->Model), "enm_heyho");
            EnemyInfo::GetSkeletonResource((int*)this, &(this->Skeleton), "enm_heyho");

            EnemyInfo::GetEnemyPackfile((int*)this, &packFile);
            ObjUtil::GetAnimationScriptResource(&animationScript, "shyguy", packFile);

            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&this->AnimationContainer, &animationScript, packFile);

            EnemyInfo::GetAnimationResource((int*)this, &(this->AnimationLeft), "hey_walk_left_loop");
            EnemyInfo::GetAnimationResource((int*)this, &(this->AnimationRight), "hey_walk_right_loop");
        }

        const char* GetInfoName()
        {
            return "EnemyShyGuyInfo";
        }
    };

    class EnemyShyGuy
    {
    public:
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
                    int* gocAnimation = GameObject::GetGOC((GameObject*)obj, GOCAnimationString);
                    if (gocAnimation)
                    {
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "WALK_L");
                        if ((*(int*)(obj + 0x4C0) & 4) == 4)
                            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "WALK_R");
                    }
                    return 1;
                };
                
                virtual int OnLeave(EnemyShyGuy* obj, int a2) { return 0; };
                
                virtual int Step(EnemyShyGuy* obj, float a2)
                {
                    if ((*(int*)(obj + 0x4C0) & 2) == 2)
                    {
                        int* gocEnemyHsm = GameObject::GetGOC((GameObject*)obj, GOCEnemyHsmString);
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
                    int* gocAnimation = GameObject::GetGOC((GameObject*)obj, GOCAnimationString);
                    if (gocAnimation)
                    {
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "WALK_L");
                        if ((*(int*)(obj + 0x4C0) & 4) == 4)
                            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "WALK_R");
                    }

                    int* gocEnemyHsm = GameObject::GetGOC((GameObject*)obj, GOCEnemyHsmString);
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
                    int* gocAnimation = GameObject::GetGOC((GameObject*)obj, GOCAnimationString);
                    if (gocAnimation)
                    {
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_L");
                        if ((*(int*)(obj + 0x4C0) & 4) == 4)
                            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_R");
                    }

                    int* gocMovement = GameObject::GetGOC((GameObject*)obj, GOCMovementString);
                    if (gocMovement)
                        game::GOCMovement::DisableMovementFlag(gocMovement, false);

                    int* gocEnemyHsm = GameObject::GetGOC((GameObject*)obj, GOCEnemyHsmString);
                    if (gocEnemyHsm)
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 0);
                    
                    return 1;
                };
                
                virtual int OnLeave(EnemyShyGuy* obj, int a2)
                {
                    int* gocMovement = GameObject::GetGOC((GameObject*)obj, GOCMovementString);
                    if (gocMovement)
                        game::GOCMovement::EnableMovementFlag(gocMovement, false);
                    
                    return 1;
                };
                
                virtual int Step(EnemyShyGuy* obj, float a2)
                {
                    if (!(*(int*)(obj + 0x4C0) & 2))
                    {
                        int* gocAnimation = GameObject::GetGOC((GameObject*)obj, GOCAnimationString);
                        if (gocAnimation)
                        {
                            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_L");
                            if ((*(int*)(obj + 0x4C0) & 4) == 4)
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
                    this->SubStateID = stateID;
                    this->field_18 = 0;
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
                    this->field_1C = 0;
                    this->field_30 = 1;
                    *(int*)(obj + 0x4C0) &= ~(1 << 1);

                    int* gocTransform = GameObject::GetGOC((GameObject*)obj, GOCTransformString);
                    if (gocTransform)
                        this->Rotation = *(csl::math::Quaternion*)(gocTransform + 0x1C);

                    int* gocAnimation = GameObject::GetGOC((GameObject*)obj, GOCAnimationString);
                    if (gocAnimation)
                    {
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_L");
                        if ((*(int*)(obj + 0x4C0) & 4) == 4)
                            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_R");
                    }

                    return 1;
                };

                virtual int OnLeave(EnemyShyGuy* obj, int a2)
                {
                    EnemyShyGuyData* data = (EnemyShyGuyData*)CSetAdapter::GetData(*(int**)(obj + 0x324));

                    int* gocMovement = GameObject::GetGOC((GameObject*)obj, GOCMovementString);
                    if (gocMovement)
                    {
                        int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
                        *((float*)(contextParam + 8)) = data->Speed * -1;
                        if ((*(int*)(obj + 0x4C0) & 4) == 4)
                            *((float*)(contextParam + 8)) = data->Speed;

                        game::GOCMovement::EnableMovementFlag(gocMovement, false);
                    }

                    return 1;
                };

                virtual int Step(EnemyShyGuy* obj, float a2)
                {
                    this->field_18 += a2;
                    switch (this->SubStateID)
                    {
                    case 0:
                    {
                        if (0.3 <= this->field_18)
                        {
                            this->field_30 = 0;
                            ChangeSubState(1);
                        }
                        break;
                    }
                    case 1:
                    {
                        int* gocTransform = GameObject::GetGOC((GameObject*)obj, GOCTransformString);

                        if (!this->field_30)
                        {
                            this->field_30 = 1;
                            int* gocAnimation = GameObject::GetGOC((GameObject*)obj, GOCAnimationString);
                            if (gocAnimation)
                            {
                                game::GOCAnimationScript::ChangeAnimation(gocAnimation, "LOOKAROUND_R");
                                if ((*(int*)(obj + 0x4C0) & 4) == 4)
                                    game::GOCAnimationScript::ChangeAnimation(gocAnimation, "LOOKAROUND_L");
                            }
                        }

                        if (gocTransform)
                        {
                            if (this->field_18 > 0.11)
                            {
                                *(int*)(obj + 0x4C0) ^= 4;
                                ChangeSubState(2);
                            }
                            else
                            {
                                if (this->field_18 > 1)
                                    this->field_18 = 1;
                                else if (this->field_18 < 0)
                                    this->field_18 = 0;

                                csl::math::Quaternion multiplier{ 0, 0.259f, 0, 0.966f };
                                if ((*(int*)(obj + 0x4C0) & 4) == 4)
                                    multiplier.Y = -0.259f;

                                csl::math::Quaternion rotation;
                                csl::math::Quaternion::QuaternionMultiply(&this->Rotation, &this->Rotation, &multiplier);
                                fnd::GOCTransform::SetLocalRotation(gocTransform, &this->Rotation);
                            }
                        }
                        break;
                    }

                    case 2:
                    {
                        if (0.3 <= this->field_18)
                        {
                            this->field_1C += 1;
                            if (this->field_1C >= 3)
                            {
                                int* gocAnimation = GameObject::GetGOC((GameObject*)obj, GOCAnimationString);
                                if (gocAnimation)
                                {
                                    game::GOCAnimationScript::ChangeAnimation(gocAnimation, "LOOKAROUND_R");
                                    if ((*(int*)(obj + 0x4C0) & 4) == 4)
                                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "LOOKAROUND_L");

                                    int* gocEnemyHsm = GameObject::GetGOC((GameObject*)obj, GOCEnemyHsmString);
                                    if (gocEnemyHsm)
                                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 0);
                                }
                            }
                            else
                            {
                                this->field_30 = 0;
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

        inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new animation::AnimCallbackBridge<EnemyShyGuy>();
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

        CSetObjectListener* __ct()
        {
            EnemyBase::__ct((GameObject*)this);
            *(int*)this = ASLR(0x00D93A14);
            *((int*)this + 2) = ASLR(0x00D939F8);

            *(int*)(this + 0x4C0) = 0;
            ObjUtil::SetPropertyLockonTarget((GameObject*)this);

            return (CSetObjectListener*)this;
        }

        void NotifyMovementStopCallback()
        {
            *(int*)(this + 0x4C0) |= 2;
        }

        void SoundCallback(int a1, int a2, int a3)
        {
            int* gocSound = GameObject::GetGOC((GameObject*)(this + 1), GOCSoundString);
            int deviceTag[3]{};
            if (gocSound)
            {
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
        }

        void AddCallback(GameDocument* gameDocument)
        {
            csl::math::Vector3 position;
            csl::math::Quaternion rotation;
            csl::math::Vector3 reticlePosition{};
            reticlePosition.Y = 5;
            fnd::GOCVisualModel::VisualDescription visualDescriptor;
            game::ShadowSphereShapeCInfo shadowInfo;
            app::game::ColliSphereShapeCInfo collisionInfo;
            int unit = 1;

            fnd::GOComponent::Create((GameObject*)this, GOCGravity);
            fnd::GOComponent::Create((GameObject*)this, GOCVisualModel);
            fnd::GOComponent::Create((GameObject*)this, GOCAnimationScript);

            if (IsShyGuyShadowOn)
                fnd::GOComponent::Create((GameObject*)this, GOCShadowSimple);

            fnd::GOComponent::Create((GameObject*)this, GOCEffect);
            fnd::GOComponent::Create((GameObject*)this, GOCSound);
            fnd::GOComponent::Create((GameObject*)this, GOCCollider);
            fnd::GOComponent::Create((GameObject*)this, GOCEnemyHSM);
            fnd::GOComponent::Create((GameObject*)this, GOCMovementComplex);
            fnd::GOComponent::BeginSetup((GameObject*)this);

            EnemyShyGuyInfo* info = (EnemyShyGuyInfo*)ObjUtil::GetObjectInfo(gameDocument, "EnemyShyGuyInfo");
            EnemyShyGuyData* data = (EnemyShyGuyData*)CSetAdapter::GetData(*(int**)(this + 0x324));

            if (data->IsEventDriven)
                *(int*)(this + 0x4C0) |= 1;
            if (data->Direction)
                *(int*)(this + 0x4C0) |= 4;

            EnemyBase::CreateCenterPositionFrame((GameObject*)this, &reticlePosition);
            int* gocVisual = GameObject::GetGOC((GameObject*)this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.Animation |= 0x400000;
                visualDescriptor.ZIndex = data->DepthOffset;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC((GameObject*)this, GOCAnimationString);
                if (gocAnimation)
                {
                    animation::AnimationResContainer* animation = &(info->AnimationContainer);

                    game::GOCAnimationScript::Setup(gocAnimation, (int*)&animation);
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
            
            int* gocShadow = GameObject::GetGOC((GameObject*)this, GOCShadowString);
            if (gocShadow)
            {
                game::ShadowSphereShapeCInfo::__ct(&shadowInfo, 4);

                game::ShadowSphereShapeCInfo* ppShadowInfo = &shadowInfo;
                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
            }

            int* gocCollider = GameObject::GetGOC((GameObject*)this, GOCColliderString);
            if (gocCollider)
            {
                game::GOCCollider::Setup(gocCollider, &unit);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.field_48 = 0;
                collisionInfo.field_44 = 0;
                collisionInfo.Radius = 4;
                ObjUtil::SetupCollisionFilter(9, &collisionInfo);
                collisionInfo.field_08 = 0x20000;
                collisionInfo.field_04 |= 1;
                collisionInfo.Parent = EnemyBase::GetCenterPositionFrame((GameObject*)this);

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            game::GOCGravity::SimpleSetup((GameObject*)this, 1);
            int* gocMovement = GameObject::GetGOC((GameObject*)this, GOCMovementString);
            if (gocMovement)
            {
                game::MoveStraight::MoveParameter moveParameter{};

                CSetAdapter::GetPosition(*(int**)(this + 0x324), &moveParameter.Position);
                CSetAdapter::GetRotation(*(int**)(this + 0x324), &moveParameter.Rotation);
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
                game::MoveStraight::FunctionPair functions { (void*)ASLR(0x0070F590), reinterpret_cast<void*&>(funcPtr) };
                game::MoveStraight::unkStruct unknown { 0, data->MaxMoveDistance };
                game::MoveStraight::SetNotifyStopCallback(moveStraight, functions, unknown, 0);

                int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
                *((float*)(contextParam + 8)) = data->Speed * -1;
                if ((*(int*)(this + 0x4C0) & 4) == 4)
                    *((float*)(contextParam + 8)) = data->Speed;
            }

            game::GOCEffect::SimpleSetup((GameObject*)this);
            game::GOCSound::SimpleSetup((GameObject*)this, 0, 0);

            int* gocEnemyHsm = GameObject::GetGOC((GameObject*)this, GOCEnemyHsmString);
            if (gocEnemyHsm)
            {
                GOCEnemyHsm::Description hsmDescription{ StateDescriptors, 3, -1 };
                if ((*(int*)(this + 0x4C0) & 1))
                    hsmDescription.field_08 = 2;
                else
                    hsmDescription.field_08 = 0;

                GOCEnemyHsm::Setup(gocEnemyHsm, &hsmDescription);
            }

            fnd::GOComponent::EndSetup((GameObject*)this);
        }

        bool ProcessMessage(fnd::Message* message)
        {
            switch (message->field_04)
            {
            case fnd::PROC_MSG_DAMAGE:
                ProcMsgDamage((xgame::MsgDamage*)message);
                break;
            case fnd::PROC_MSG_KICK:
                ProcMsgKick(message);
                return true;
                break;
            case fnd::PROC_MSG_NOTIFY_OBJECT_EVENT:
                ProcMsgNotifyObjectEvent(message);
                return true;
                break;
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision(message);
                return true;
                break;
            default:
                EnemyBase::ProcessMessage((int*)this, message);
                return true;
            }
        }
    private:
        void ProcMsgDamage(xgame::MsgDamage* message)
        {
            int* gocTransform = GameObject::GetGOC((GameObject*)(this - 8), GOCTransformString);
            if (gocTransform)
            {
                csl::math::Vector3 translation{};
                enemy::DeadEffectCInfo effectInfo;

                math::CalculatedTransform::GetTranslation((csl::math::Matrix34*)(gocTransform + 0x44), &translation);
                xgame::MsgDamage::SetReply(message, &translation, 1);
                ObjUtil::AddScore((GameObject*)(this - 8), "SHYGUY", message);

                enemy::DeadEffectCInfo::__ct(&effectInfo);
                GameObjectHandleBase::__ct(&effectInfo, (GameObject*)(this - 8));
                enemy::DeadEffectCInfo::SetMsgDamage(&effectInfo, message);
                enemy::DeadEffectCInfo::SetYoshiIsland(&effectInfo);

                csl::math::Matrix34 transform{};
                int* gocVisual = GameObject::GetGOC((GameObject*)(this - 8), GOCVisual);
                if (gocVisual)
                {
                    fnd::GOCVisualModel::GetNodeTransform(gocVisual, 0, "Spine", &transform);
                    effectInfo.field_20.data[3][0] = transform.data[0][0];
                    effectInfo.field_20.data[3][1] = transform.data[0][1];
                    effectInfo.field_20.data[3][2] = transform.data[0][2];
                    effectInfo.field_62 |= 1;
                }

                void* enemyManager = EnemyManager::GetService(*(GameDocument**)(this + 0x20));
                EnemyManager::CreateDeadEffect(enemyManager, &effectInfo);
                EnemyBase::ProcMission((GameObject*)(this - 8), message);
                CSetObjectListener::SetStatusRetire((GameObject*)(this - 8));
                GameObject::Kill((GameObject*)(this - 8));
            }
        }

        void ProcMsgKick(fnd::Message* message)
        {
            EnemyBlowOffObjectCInfo blowOffInfo;
            EnemyShyGuyInfo* info = (EnemyShyGuyInfo*)ObjUtil::GetObjectInfo(*Document, "EnemyShyGuyInfo");
            int* gocTransform = GameObject::GetGOC((GameObject*)(this - 8), GOCTransformString);
            if (gocTransform)
            {
                EnemyBlowOffObjectCInfo::__ct(&blowOffInfo);
                EnemyBlowOffObjectCInfo::SetParamByMessage(&blowOffInfo, message);
                blowOffInfo.Model = info->Model;
                blowOffInfo.Skeleton = info->Skeleton;
                blowOffInfo.Animation = info->AnimationLeft;
                if ((*(int*)(this + 0x4C0) & 4) == 4)
                    blowOffInfo.Animation = info->AnimationRight;
                blowOffInfo.field_10 = *(csl::math::Matrix34*)(gocTransform + 0x44);
                blowOffInfo.field_50.Y = 5;
                blowOffInfo.field_60 = 4;
                blowOffInfo.field_6C = 3;
                EnemyBase::CreateEnemyBlowOffObject((GameObject*)(this - 8), &blowOffInfo);
                xgame::MsgKick::SetReplyForSucceed(message);
                ObjUtil::AddScore((GameObject*)(this - 8), "SHYGUY", message);
                CSetObjectListener::SetStatusRetire((GameObject*)(this - 8));
                GameObject::Kill((GameObject*)(this - 8));
            }
        }

        void ProcMsgNotifyObjectEvent(fnd::Message* message)
        {
            if ((*(int*)(this + 0x4B8) & 1))
            {
                int* gocEnemyHsm = GameObject::GetGOC((GameObject*)(this - 8), GOCEnemyHsmString);
                if (gocEnemyHsm)
                    GOCEnemyHsm::Dispatch(gocEnemyHsm, message);
            }
        }

        void ProcMsgHitEventCollision(fnd::Message* message)
        {
            EnemyBase::SendTouchDamage((int*)this - 2, message);
        }
    };

    inline static GameObject* create_EnemyShyGuy()
    {
        GameObject* object = GameObject::New(0x4D0);
        if (!object)
            return 0;
        ((EnemyShyGuy*)object)->__ct();
        return object;
    }

    inline static EnemyShyGuyInfo* create_EnemyShyGuy_EnemyShyGuyInfo(csl::fnd::IAllocator* a1)
    {
        EnemyShyGuyInfo* result = (EnemyShyGuyInfo*)app::fnd::ReferencedObject::f_new(0x64, a1);
        if (result)
            result->__ct(result);
        return result;
    }
}