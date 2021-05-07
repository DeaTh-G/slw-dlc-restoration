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

            void OnEvent(int notifyTiming) override
            {
                pDekunuts->Shot();
            }
        };

    public:
        float field_4D0;
        int Flags;
        Listener AnimationListener{};
        float field_504;
        float field_508;
        float field_50C;
        fnd::HFrame Children;
        csl::math::Matrix34 field_640;
        long long field_680[64];

        EnemyDekunuts()
        {
            AnimationListener.field_20 = 2;
            fnd::HFrame::__ct(&Children);
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

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            fnd::HFrame* transformFrame = (fnd::HFrame*)(GameObject::GetGOC(this, GOCTransformString) + 0x28);
            fnd::HFrame::AddChild(transformFrame, &Children);
            field_640 = *(csl::math::Matrix34*)(gocTransform + 0x44);

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
                csl::math::Vector3 offset { data->RangeAddX, data->RangeAddY, data->RangeAddZ };
                game::CollisionObjCInfo::__ct(&colliderInfo);
                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                colliderInfo.MotionType = 2;
                colliderInfo.Radius = data->AppearRange;
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
                    }
                    else if ((obj->Flags & 4) == 4)
                    {
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
                    }
                    else
                    {
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 2);
                    }

                    return 0;
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

                    for (size_t i = 0; i < 2; i++)
                    {
                        int* model = *(int**)(*gocContainer + 4 * i);
                        fnd::GOCVisual::SetVisible(model, false);
                    }

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return 0;

                    GOCEnemyHsm::SetEnableUpdate(gocEnemyHsm, false);

                    csl::math::Vector3 vector{};
                    enemy::DeadEffectCInfo effectInfo{};
                    xgame::MsgDamage damageMessage { 2, 8, 2, &vector, &vector };
                    enemy::DeadEffectCInfo::SetMsgDamage(&effectInfo, &damageMessage);
                    enemy::DeadEffectCInfo::SetTransform(&effectInfo, &obj->Children.Transform);
                    enemy::DeadEffectCInfo::SetZeldaDekunuts(&effectInfo);

                    void* enemyManager = EnemyManager::GetService((GameDocument*)obj->field_24[1]);
                    EnemyManager::CreateDeadEffect(enemyManager, &effectInfo);
                    EnemyBase::ProcMission(obj, &damageMessage);
                    CSetObjectListener::SetStatusRetire(obj);

                    return 1;
                };
            };
        };

    private:
        void ProcMsgStayTrigger(xgame::MsgStayTrigger& message)
        {
            if (Flags & 1)
                return;
        
            if (!ObjUtil::CheckShapeUserID(message.field_18, 0))
                return;
        
            Flags |= 4;

            if (!ObjUtil::CheckShapeUserID(message.field_18, 1))
                return;

            Flags &= ~4;
        }

        void RequestShot()
        {

        }

        void Shot()
        {

        }

        void RotateTarget(float a1, float a2)
        {

        }

        void KillShots(bool doKill)
        {

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