#pragma once

namespace app
{
    inline static const char* DEKUNUTS_MODEL_NAME[] = { "enm_dekunuts_base", "enm_dekunuts" };
    inline static const char* DEKUNUTS_ANIM_NAME[] = { "IDLE_LOOP", "UNDERGROUND_LOOP", "APPEARE", "ESCAPE", "DAMAGE", "ATTACK" };

    struct EnemyDekunutsData
    {
        bool MessageBoot;
        float AppearRange;
        float SearchRange;
        float RangeAddX;
        float RangeAddY;
        float RangeAddZ;
    };

    class EnemyDekunutsInfo : public EnemyInfo
    {
    public:
        int Models[2]{};
        int Skeletons[2]{};
        animation::AnimationResContainer DekunutsAnimationContainer{};
        animation::AnimationResContainer BaseAnimationContainer{};
        int BulletModel;

        EnemyDekunutsInfo()
        {
            PacfileName = "EnemyDekunuts.pac";
            SoundfileName = "sound/se_enemy_kokeko_fv.acb";
            animation::AnimationResContainer::__ct(&DekunutsAnimationContainer, (csl::fnd::IAllocator*)pAllocator);
            animation::AnimationResContainer::__ct(&BaseAnimationContainer, (csl::fnd::IAllocator*)pAllocator);
        }

        void Destructor(size_t deletingFlags) override
        {
            EnemyDekunutsInfo::~EnemyDekunutsInfo();
            animation::AnimationResContainer::__dt(&DekunutsAnimationContainer);
            animation::AnimationResContainer::__dt(&BaseAnimationContainer);
            EnemyInfo::Destructor(deletingFlags);
        }

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            int animationScript[3]{};

            for (size_t i = 0; i < 2; i++)
            {
                EnemyInfo::GetModelResource(this, &Models[i], DEKUNUTS_MODEL_NAME[i]);
                EnemyInfo::GetSkeletonResource(this, &Skeletons[i], DEKUNUTS_MODEL_NAME[i]);
            }
            EnemyInfo::GetEnemyPackfile(this, &packFile);

            animationScript[1] = Skeletons[0];
            ObjUtil::GetAnimationScriptResource(animationScript, "dekunuts", packFile);
            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&DekunutsAnimationContainer, animationScript, packFile);
            animationScript[0] = 0; animationScript[1] = 0; animationScript[2] = 0;

            ObjUtil::GetAnimationScriptResource(animationScript, "dekunutsbase", packFile);
            animationScript[1] = Skeletons[1];
            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&BaseAnimationContainer, animationScript, packFile);

            EnemyInfo::GetModelResource(this, &BulletModel, "enm_dekunuts_bullet");
        }

        const char* GetInfoName() override
        {
            return "EnemyDekunutsInfo";
        }
    };

    class EnemyDekunuts : public EnemyBase
    {
        class Listener : public animation::AnimationListener
        {
        public:
            EnemyDekunuts* pDekunuts;
            bool DoShoot{};

            void OnEvent(int notifyTiming) override
            {
                if (DoShoot)
                {
                    pDekunuts->Shot();
                    DoShoot = false;
                }
            }
        };

    public:
        float field_4D0{};
        int Flags{};
        Listener AnimationListener{};
        int ActorID{};
        float field_50C{};
        fnd::HFrame Children{};
        csl::math::Matrix34 field_640{};
        GameObjectHandleBase Shots[64]{};

        EnemyDekunuts()
        {
            AnimationListener.field_20 = 2;
            AnimationListener.DoShoot = false;
            fnd::HFrame::__ct(&Children);

            for (size_t i = 0; i < 64; i++)
            {
                GameObjectHandleBase::__ct(&Shots[i], nullptr);
            }
        }

        void Destructor(size_t deletingFlags)
        {
            fnd::HFrame::__dt(&Children, 0);
            AnimationListener.Destructor(0);

            EnemyBase::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            csl::math::Vector3 centerFrame{ 0, 0, 0 };

            fnd::GOComponent::Create(this, GOCVisualContainer);
            fnd::GOComponent::Create(this, GOCAnimationContainer);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCEnemyHSM);
            fnd::GOComponent::Create(this, EnemyTargetGOC);
            fnd::GOComponent::Create(this, GOCSound);

            EnemyDekunutsData* data = (EnemyDekunutsData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            EnemyDekunutsInfo* info = (EnemyDekunutsInfo*)ObjUtil::GetObjectInfo(gameDocument, "EnemyDekunutsInfo");
            Flags |= data->MessageBoot != 0;

            fnd::GOComponent::BeginSetup(this);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
            {
                fnd::HFrame* transformFrame = (fnd::HFrame*)(GameObject::GetGOC(this, GOCTransformString) + 0x28);
                fnd::HFrame::AddChild(transformFrame, &Children);
                field_640 = *(csl::math::Matrix34*)(gocTransform + 0x44);
                csl::math::Matrix34Inverse(&field_640, &field_640);
            }

            int* gocVContainer = GameObject::GetGOC(this, GOCVisual);
            if (gocVContainer)
            {
                int modelCount = 2;
                fnd::GOCVisualContainer::Setup(gocVContainer, &modelCount);

                int* gocAContainer = GameObject::GetGOC(this, GOCAnimation);
                if (gocAContainer)
                {
                    int animationCount = 2;
                    game::GOCAnimationContainer::Setup(gocAContainer, &animationCount);
                }

                for (size_t i = 0; i < 2; i++)
                {
                    int* gocVisual = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
                    if (gocVisual)
                    {
                        fnd::GOCVisualModel::VisualDescription visualDescriptor{};
                        fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                        visualDescriptor.Model = info->Models[i];
                        visualDescriptor.Skeleton = info->Skeletons[i];
                        if (i)
                            visualDescriptor.Parent = &Children;
                        fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                        fnd::GOCVisualContainer::Add(gocVContainer, gocVisual);

                        int* gocAnimation = fnd::GOComponent::CreateSingle(this, GOCAnimationScript);
                        if (gocAnimation)
                        {
                            animation::AnimationResContainer* animation;

                            if (!i)
                                animation = &(info->BaseAnimationContainer);
                            else
                                animation = &(info->DekunutsAnimationContainer);

                            game::GOCAnimationScript::Setup(gocAnimation, &animation);
                            fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                            game::GOCAnimationScript::SetAnimation(gocAnimation, "UNDERGROUND_LOOP");
                            game::GOCAnimationContainer::Add(gocAContainer, gocAnimation);

                            if (i)
                            {
                                csl::fnd::IAllocator* allocator{};
                                auto funcPtr = &EnemyDekunuts::AnimationCallback;
                                animation::AnimCallbackBridge<EnemyDekunuts>* callback =
                                    (animation::AnimCallbackBridge<EnemyDekunuts>*)AnimCallbackBridge_Initialize(allocator);
                                callback->GameObject = this;
                                callback->field_10 = reinterpret_cast<void*&>(funcPtr);
                                callback->field_14 = -1;

                                game::GOCAnimationScript::RegisterCallback(gocAnimation, 0, callback);

                                AnimationListener.pDekunuts = this;
                                game::GOCAnimationSimple::AddListener(gocAnimation, &AnimationListener);
                            }
                        }
                    }
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 3;
                game::ColliSphereShapeCInfo colliderInfo{};
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                // Search Collider
                csl::math::Vector3 offset{ data->RangeAddX, data->RangeAddY, data->RangeAddZ };
                game::CollisionObjCInfo::__ct(&colliderInfo);
                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                colliderInfo.MotionType = 2;
                colliderInfo.Radius = data->AppearRange;
                colliderInfo.field_54 = 0;
                colliderInfo.field_48 = 0;
                colliderInfo.field_44 = 0;
                ObjUtil::SetupCollisionFilter(6, &colliderInfo);
                game::CollisionObjCInfo::SetLocalPosition(&colliderInfo, &offset);
                colliderInfo.field_04 = 6;
                colliderInfo.field_0C = 0;
                game::GOCCollider::CreateShape(gocCollider, &colliderInfo);

                // Unknown Collider
                game::CollisionObjCInfo::__ct(&colliderInfo);
                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                colliderInfo.MotionType = 2;
                colliderInfo.Radius = 50;
                colliderInfo.field_54 = 0;
                colliderInfo.field_48 = 0;
                colliderInfo.field_44 = 0;
                ObjUtil::SetupCollisionFilter(6, &colliderInfo);
                colliderInfo.field_04 = 6;
                colliderInfo.field_0C = 1;
                game::GOCCollider::CreateShape(gocCollider, &colliderInfo);

                // Unknown Collider
                offset = Vector3(0, 6.5f, 0);
                game::CollisionObjCInfo::__ct(&colliderInfo);
                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                colliderInfo.MotionType = 2;
                colliderInfo.Radius = 6.5f;
                colliderInfo.field_54 = 0;
                colliderInfo.field_48 = 0;
                colliderInfo.field_44 = 0;
                ObjUtil::SetupCollisionFilter(9, &colliderInfo);
                game::CollisionObjCInfo::SetLocalPosition(&colliderInfo, &offset);
                colliderInfo.field_04 = 1;
                colliderInfo.field_0C = 2;
                game::GOCCollider::CreateShape(gocCollider, &colliderInfo);
            }

            int* gocEnemyTarget = GameObject::GetGOC(this, GOCEnemyTargetString);
            if (gocEnemyTarget)
            {
                int count = 0;
                csl::math::Vector3 eyeOffset{ data->RangeAddX, data->RangeAddY, data->RangeAddZ };

                GOCEnemyTarget::Setup(gocEnemyTarget, &count);
                GOCEnemyTarget::SetSphereEyesight(gocEnemyTarget, data->SearchRange);
                GOCEnemyTarget::SetEyesightOffset(gocEnemyTarget, &eyeOffset);
            }

            int* gocEnemyHsm = GameObject::GetGOC(this, GOCEnemyHsmString);
            if (gocEnemyHsm)
            {
                GOCEnemyHsm::Description hsmDescription{ StateDescriptors, 5, 0 };
                GOCEnemyHsm::Setup(gocEnemyHsm, &hsmDescription);
            }

            if (Flags & 1)
                game::GOCCollider::SetEnable(gocCollider, 0);

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            switch (message.Type)
            {
            case fnd::PROC_MSG_DAMAGE:
                ProcMsgDamage((xgame::MsgDamage&)message);
                return true;
            case fnd::PROC_MSG_DLC_ZELDA_NOTICE_ACTIVE_ENEMY:
                ProcMsgDlcZeldaNoticeActiveEnemy((xgame::MsgDlcZeldaNoticeActiveEnemy&)message);
                return true;
            case fnd::PROC_MSG_DLC_ZELDA_NOTICE_STOP_ENEMY:
                ProcMsgDlcZeldaNoticeStopEnemy((xgame::MsgDlcZeldaNoticeStopEnemy&)message);
                return true;
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return true;
            case fnd::PROC_MSG_LEAVE_EVENT_COLLISION:
                ProcMsgLeaveEventCollision((xgame::MsgLeaveEventCollision&)message);
                return true;
            case fnd::PROC_MSG_NOTIFY_OBJECT_EVENT:
                ProcMsgNotifyObjectEvent((xgame::MsgNotifyObjectEvent&)message);
                return true;
            case fnd::PROC_MSG_STAY_TRIGGER:
                ProcMsgStayTrigger((xgame::MsgStayTrigger&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

        class State
        {
        public:
            class Hide
            {
                char field_00[20];
            public:
                virtual ~Hide() {};
                virtual int Trigger(EnemyDekunuts* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyDekunuts* obj) { return 0; };
                virtual int Enter(EnemyDekunuts* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyDekunuts* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyDekunuts* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyDekunuts* obj, int a2)
                {
                    obj->SetBodyCollisionEnable(0);
                    obj->SetAnimation(1);
                    return 1;

                    return 0;
                };
                virtual int OnLeave(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int Step(EnemyDekunuts* obj, float deltaTime)
                {
                    if ((obj->Flags & 4) == 4)
                    {
                        int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                        if (!gocEnemyHsm)
                            return 0;

                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 1);
                    }
                    return 1;
                };
            };

            class Appear
            {
                char field_00[20];
                float Time;
            public:
                virtual ~Appear() {};
                virtual int Trigger(EnemyDekunuts* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyDekunuts* obj) { return 0; };
                virtual int Enter(EnemyDekunuts* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyDekunuts* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyDekunuts* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyDekunuts* obj, int a2)
                {
                    obj->SetBodyCollisionEnable(0);
                    obj->SetAnimation(2);

                    int deviceTag[3]{};
                    int* gocSound = GameObject::GetGOC(obj, GOCSoundString);
                    if (!gocSound)
                        return 0;

                    game::GOCSound::Play3D(gocSound, deviceTag, "enm_dekunuts_appear", 0);

                    return 1;
                };
                virtual int OnLeave(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int Step(EnemyDekunuts* obj, float deltaTime)
                {
                    Time += deltaTime;
                    float rotateTime = csl::math::Clamp(deltaTime * 2, 0, 1);
                    obj->RotateTarget(-1, rotateTime);
                    if (Time > 0.5f)
                    {
                        int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                        if (!gocEnemyHsm)
                            return 0;

                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 3);
                    }

                    return 1;
                };
            };

            class Retreat
            {
                char field_00[20];
                float Time;
            public:
                virtual ~Retreat() {};
                virtual int Trigger(EnemyDekunuts* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyDekunuts* obj) { return 0; };
                virtual int Enter(EnemyDekunuts* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyDekunuts* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyDekunuts* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyDekunuts* obj, int a2)
                {
                    obj->SetBodyCollisionEnable(0);
                    obj->SetAnimation(3);

                    int deviceTag[3]{};
                    int* gocSound = GameObject::GetGOC(obj, GOCSoundString);
                    if (!gocSound)
                        return 0;

                    game::GOCSound::Play3D(gocSound, deviceTag, "enm_dekunuts_appear", 0);

                    return 1;
                };
                virtual int OnLeave(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int Step(EnemyDekunuts* obj, float deltaTime)
                {
                    Time += deltaTime;
                    if (Time > 0.416f)
                    {
                        int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                        if (!gocEnemyHsm)
                            return 0;

                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 0);
                    }

                    return 1;
                };
            };

            class Attack
            {
                char field_00[20];
                float Time;
            public:
                virtual ~Attack() {};
                virtual int Trigger(EnemyDekunuts* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyDekunuts* obj) { return 0; };
                virtual int Enter(EnemyDekunuts* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyDekunuts* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyDekunuts* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyDekunuts* obj, int a2)
                {
                    obj->SetBodyCollisionEnable(1);
                    Time = 0;

                    return 1;
                };
                virtual int OnLeave(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int Step(EnemyDekunuts* obj, float deltaTime)
                {
                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return 0;

                    if ((obj->Flags & 8) == 8)
                    {
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 4);
                        return 0;
                    }

                    if (!(obj->Flags & 4))
                    {
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 2);
                        return 1;
                    }

                    obj->RotateTarget(2.7925267 * deltaTime, -1);

                    int* gocEnemyTarget = GameObject::GetGOC(obj, GOCEnemyTargetString);
                    if (!gocEnemyTarget)
                        return 0;

                    bool isTargetFound = GOCEnemyTarget::IsFindTarget(gocEnemyTarget);
                    if (!isTargetFound)
                        return 0;

                    if (obj->IsCurrentAnimation(5))
                        return 0;

                    Time -= deltaTime;
                    if (Time < 0)
                    {
                        obj->SetAnimation(5);
                        Time = 2;
                    }

                    return 1;
                };
            };

            class Disappear
            {
                char field_00[20];
            public:
                virtual ~Disappear() {};
                virtual int Trigger(EnemyDekunuts* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyDekunuts* obj) { return 0; };
                virtual int Enter(EnemyDekunuts* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyDekunuts* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyDekunuts* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyDekunuts* obj, int a2)
                {
                    obj->SetAnimation(4);
                    obj->KillShots(true);

                    int deviceTag[3]{};
                    int* gocSound = GameObject::GetGOC(obj, GOCSoundString);
                    if (!gocSound)
                        return 0;

                    game::GOCSound::Play3D(gocSound, deviceTag, "enm_dekunuts_appear", 0);

                    return 1;
                };
                virtual int OnLeave(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int Step(EnemyDekunuts* obj, float deltaTime)
                {
                    if (!obj->IsEndAnimation(4))
                        return 0;

                    int* gocContainer = GameObject::GetGOC(obj, GOCVisual) + 0x10;
                    if (!gocContainer)
                        return 0;

                    int* baseModel = *(int**)(*gocContainer + 4);
                    fnd::GOCVisual::SetVisible(baseModel, false);

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return 0;

                    GOCEnemyHsm::SetEnableUpdate(gocEnemyHsm, false);

                    csl::math::Vector3 vector{};
                    enemy::DeadEffectCInfo effectInfo{};
                    xgame::MsgDamage damageMessage{ 2, 8, 2, &vector, &vector };
                    enemy::DeadEffectCInfo::SetMsgDamage(&effectInfo, &damageMessage);
                    enemy::DeadEffectCInfo::SetTransform(&effectInfo, &obj->Children.Transform);
                    enemy::DeadEffectCInfo::SetZeldaDekunuts(&effectInfo);

                    void* enemyManager = EnemyManager::GetService((GameDocument*)obj->field_24[1]);
                    EnemyManager::CreateDeadEffect(enemyManager, &effectInfo);
                    EnemyBase::ProcMission(obj, &damageMessage);
                    obj->SetStatusRetire();

                    return 1;
                };
            };
        };

    private:
        void ProcMsgDamage(xgame::MsgDamage& message)
        {
            if (Flags & 1)
                return;

            if (!ObjUtil::CheckShapeUserIDFromHandle((uintptr_t)&message.field_18, 2))
                return;

            if (!EnemyUtil::IsDamage(message.field_2C, 0, message.AttackType))
                return;
            
            if (!(Flags & 2))
                return;
        
            if (message.field_28 != 2)
                return;
        
            csl::math::Vector3 translation{};

            math::CalculatedTransform::GetTranslation(&Children.Transform, &translation);
            message.SetReply(&translation, 1);
            ObjUtil::AddScore2(this, "DEKUNUTS", 0, &translation);

            int deviceTag[3]{};
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play3D(gocSound, deviceTag, "enm_dekunuts_cry", 0);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (!gocCollider)
                return;

            game::GOCCollider::SetEnable(gocCollider, 0);

            int* gocEnemyTarget = GameObject::GetGOC(this, GOCEnemyTargetString);
            if (!gocEnemyTarget)
                return;

            GOCEnemyTarget::SetEnableEyesight(gocEnemyTarget, false);

            Flags |= 8;
        }

        void ProcMsgDlcZeldaNoticeActiveEnemy(xgame::MsgDlcZeldaNoticeActiveEnemy& message)
        {
            if (!(Flags & 16))
                return;
        
            Flags &= ~16;
            Activate(true);
        }

        void ProcMsgDlcZeldaNoticeStopEnemy(xgame::MsgDlcZeldaNoticeStopEnemy& message)
        {
            if (Flags & 8)
                return;

            if (!Activate(false))
                return;

            KillShots(false);
            Flags |= 16;
        }

        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            csl::math::Vector3 vector{};

            if (Flags & 1)
                return;
        
            if (!(Flags & 2))
                return;

            if (!ObjUtil::CheckShapeUserID(message.field_18, 2))
                return;
        
            xgame::MsgDamage damageMessage{ 3, 8, 1, &message, &vector};
            SendMessageImm(message.ActorID, &damageMessage);
        }

        void ProcMsgLeaveEventCollision(xgame::MsgLeaveEventCollision& message)
        {
            // Clear ActorID for Multiplayer Support
            if (ObjUtil::CheckShapeUserIDFromHandle((uintptr_t)&message.field_18, 0) && message.ActorID == ActorID)
                ActorID = 0;

            if (Flags & 1)
                return;
            
            if (ObjUtil::CheckShapeUserIDFromHandle((uintptr_t)&message.field_18, 1))
                Flags |= 4;

            if (!ObjUtil::CheckShapeUserIDFromHandle((uintptr_t)&message.field_18, 0))
                return;

            Flags &= ~4;
        }

        void ProcMsgNotifyObjectEvent(xgame::MsgNotifyObjectEvent& message)
        {
            if (message.field_18 != 1)
            {
                if (message.field_18 != 2)
                    return;

                message.field_18 = 0;
            }

            Activate(message.field_18);
        }

        void ProcMsgStayTrigger(xgame::MsgStayTrigger& message)
        {
            // Store ActorID for Multiplayer Support
            if (ObjUtil::CheckShapeUserID(message.field_18, 0) && !ActorID)
                ActorID = message.field_20;

            if (Flags & 1)
                return;

            if (ObjUtil::CheckShapeUserID(message.field_18, 0))
                Flags |= 4;

            if (!ObjUtil::CheckShapeUserID(message.field_18, 1))
                return;

            Flags &= ~4;
        }

        bool Activate(bool doActivate)
        {
            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (!gocCollider)
                return false;

            if (doActivate)
            {
                if (!(Flags & 1))
                    return false;

                game::GOCCollider::SetEnable(gocCollider, true);
                Flags &= ~1;
                return true;
            }
            else if (!(Flags & 1))
            {
                game::GOCCollider::SetEnable(gocCollider, false);
                Flags |= 1;
                Flags &= ~4;
                return true;
            }

            return false;
        }

        void RequestShot()
        {
            ((EnemyDekunuts*)((char*)this + 1))->AnimationListener.DoShoot = true;
        }

        void Shot()
        {
            EnemyDekunutsInfo* info = (EnemyDekunutsInfo*)ObjUtil::GetObjectInfo((GameDocument*)field_24[1], "EnemyDekunutsInfo");

            math::Transform transform{};
            int* gocContainer = GameObject::GetGOC(this, GOCVisual) + 0x10;
            if (!gocContainer)
                return;

            int* shrubModel = *(int**)(*gocContainer + 4);
            fnd::GOCVisualModel::GetNodeTransform(shrubModel, 0, "Muzzle", &transform);

            csl::math::Matrix34 matrix{};
            math::Matrix34AffineTransformation(&matrix, &transform.Position, &transform.Rotation);
            dekunuts_shot::CreateInfo shotInfo{&matrix, info->BulletModel, 80, 3.5f};
            EnemyDekunutsShot* shot = dekunuts_shot::Create(&shotInfo, *(GameDocument*)field_24[1]);
            if (!shot)
                return;
            
            for (size_t i = 0; i < 64; i++)
            {
                if (!Shots[i].IsValid())
                {
                    Shots[i] = shot;

                    int deviceTag[3]{};
                    int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                    if (!gocSound)
                        continue;

                    game::GOCSound::Play3D(gocSound, deviceTag, "enm_dekunuts_bullet", 0);
                    break;
                }
            }
        }

        void RotateTarget(float a1, float a2)
        {
            int playerNo = ObjUtil::GetPlayerNo(field_24[1], ActorID);
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
            if (!playerInfo)
                return;

            csl::math::Vector3 upVec { 0, 1, 0 };
            csl::math::Vector3 depthVec { 0, 0, 1 };
            csl::math::Vector3 scaledUpVec{};
            csl::math::Vector3 playerDistance{};
            csl::math::Vector3 rotationAngle{};
            playerDistance = MultiplyMatrixByVector(&field_640, (csl::math::Vector3*)(playerInfo + 0x3C));
            float playerDir = math::Vector3DotProduct(&playerDistance, &upVec);
            math::Vector3Scale(&upVec, playerDir, &scaledUpVec);
            math::Vector3Subtract(&playerDistance, &scaledUpVec, &playerDistance);
            bool isNormalized = math::Vector3NormalizeIfNotZero(&playerDistance, &playerDistance);
            if (!isNormalized)
                return;
            
            math::Vector3RotateY(&rotationAngle, field_4D0, &depthVec);
            float playerDist = math::Vector3DotProduct(&rotationAngle, &playerDistance);
            float clampedDist = csl::math::Clamp(playerDist, -1, 1);
            float dist = acosf(clampedDist);
            if (a1 > 0)
                csl::math::Min(dist, a1);
            if (a2 > 1)
                dist *= a2;

            Eigen::Vector3f v(0, 1, 0);
            Eigen::Vector3f rv(rotationAngle.X, rotationAngle.Y, rotationAngle.Z);
            v = v.cross(rv);
            csl::math::Vector3 posDifference { v.x(), v.y(), v.z() };
            float posDiff = math::Vector3DotProduct(&posDifference, &playerDistance);
            float diff = csl::math::Select(posDiff, fabs(dist), -abs(dist));
            field_4D0 += diff;

            Eigen::Quaternion<float> q;
            q = Eigen::AngleAxis<float>(field_4D0, Eigen::Vector3f(0, 1, 0));
            csl::math::Quaternion rotation{ q.x(), q.y(), q.z(), q.w() };

            fnd::HFrame::SetLocalRotation(&Children, &rotation);
        }

        void KillShots(bool notKill)
        {
            if (notKill)
            {
                for (size_t i = 0; i < 64; i++)
                {
                    if (Shots[i].Get())
                    {
                        ((EnemyDekunutsShot*)Shots[i].Get())->Explosion();
                        continue;
                    }
                    
                    break;
                }
            }
            else
            {
                for (size_t i = 0; i < 64; i++)
                {
                    if (Shots[i].Get())
                    {
                        (EnemyDekunutsShot*)Shots[i].Get()->Kill();
                        continue;
                    }
                    
                    break;
                }
            }
        }

        void SetBodyCollisionEnable(int a2)
        {
            if (a2)
                Flags |= 2;
            else
                Flags &= ~2;
        }

        void SetAnimation(int animationID)
        {
            int* gocContainer = GameObject::GetGOC(this, GOCAnimation) + 0xF;
            if (!gocContainer)
                return;

            for (size_t i = 0; i < 2; i++)
            {
                int* animation = *(int**)(*gocContainer + 4 * i);
                game::GOCAnimationScript::ChangeAnimation(animation, DEKUNUTS_ANIM_NAME[animationID]);
            }
        }

        bool IsEndAnimation(int animationID)
        {
            if (!IsCurrentAnimation(animationID))
                return false;

            int* gocContainer = GameObject::GetGOC(this, GOCAnimation) + 0xF;
            if (!gocContainer)
                return false;

            size_t i = 0;
            while (true)
            {
                int* animation = *(int**)(*gocContainer + 4 * i);
                if (!game::GOCAnimationScript::IsFinished(animation))
                    break;

                if (++i >= 2)
                    return true;
            }

            return false;
        }

        bool IsCurrentAnimation(int animationID)
        {
            int* gocContainer = GameObject::GetGOC(this, GOCAnimation) + 0xF;
            if (!gocContainer)
                return false;

            size_t i = 0;
            while (true)
            {
                int* animation = *(int**)(*gocContainer + 4 * i);
                if (!game::GOCAnimationScript::IsCurrentAnimation(animation, DEKUNUTS_ANIM_NAME[animationID]))
                    break;

                if (++i >= 2)
                    return true;
            }

            return false;
        }

        void AnimationCallback(int a1, int a2, int a3)
        {
            if (a2 == 1)
                RequestShot();
        }

        inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new animation::AnimCallbackBridge<EnemyDekunuts>();
        }

        inline static void* Hide_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyDekunuts::State::Hide();
        }

        inline static void* Appear_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyDekunuts::State::Appear();
        }

        inline static void* Retreat_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyDekunuts::State::Retreat();
        }

        inline static void* Attack_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyDekunuts::State::Attack();
        }

        inline static void* Disappear_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyDekunuts::State::Disappear();
        }

        inline static ut::internal::StateDescImpl States[] =
        {
            { "Hide", &Hide_Initialize, -1 },
            { "Appear", &Appear_Initialize, -1 },
            { "Retreat", &Retreat_Initialize, -1 },
            { "Attack", &Attack_Initialize, -1 },
            { "Disappear", &Disappear_Initialize, -1 }
        };

        inline static GOCEnemyHsm::StateDesc StateDescriptors[] =
        {
            {0, &States[0]},
            {1, &States[1]},
            {2, &States[2]},
            {3, &States[3]},
            {4, &States[4]},
        };
    };

    inline static EnemyDekunuts* create_EnemyDekunuts()
    {
        return new EnemyDekunuts();
    }

    inline static EnemyDekunutsInfo* createObjInfo_EnemyDekunutsInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) EnemyDekunutsInfo();
    }
}