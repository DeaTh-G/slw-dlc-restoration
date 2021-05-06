#pragma once

namespace app
{
    inline static const char* DEKUNUTS_MODEL_NAME[] = { "enm_dekunuts_base", "enm_dekunuts" };

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
        bool IsMessageBoot;
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
            IsMessageBoot = data->MessageBoot != 0;

            fnd::GOComponent::BeginSetup(this);

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            //fnd::HFrame::AddChild((fnd::HFrame*)(gocTransform + 0x28), &Parent);
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
                game::GOCCollider::CreateShape(gocCollider, &colliderInfo);

                // Unknown Collider
                game::CollisionObjCInfo::__ct(&colliderInfo);
                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                colliderInfo.MotionType = 2;
                colliderInfo.Radius = 50;
                ObjUtil::SetupCollisionFilter(6, &colliderInfo);
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

            if (IsMessageBoot)
                game::GOCCollider::SetEnable(gocCollider, 0);

            fnd::GOComponent::EndSetup(this);
        }
        class State
        {
        public:
            class Appear
            {
                char field_00[20];
            public:
                virtual ~Appear() {};
                virtual int Trigger(EnemyDekunuts* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyDekunuts* obj) { return 0; };
                virtual int Enter(EnemyDekunuts* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyDekunuts* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyDekunuts* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int Step(EnemyDekunuts* obj, float deltaTime) { return 0; };
            };

            class Attack
            {
                char field_00[20];
            public:
                virtual ~Attack() {};
                virtual int Trigger(EnemyDekunuts* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyDekunuts* obj) { return 0; };
                virtual int Enter(EnemyDekunuts* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyDekunuts* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyDekunuts* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int Step(EnemyDekunuts* obj, float deltaTime) { return 0; };
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
                virtual int OnEnter(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int Step(EnemyDekunuts* obj, float deltaTime) { return 0; };
            };

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
                virtual int OnEnter(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int Step(EnemyDekunuts* obj, float deltaTime) { return 0; };
            };

            class Retreat
            {
                char field_00[20];
            public:
                virtual ~Retreat() {};
                virtual int Trigger(EnemyDekunuts* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyDekunuts* obj) { return 0; };
                virtual int Enter(EnemyDekunuts* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyDekunuts* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyDekunuts* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyDekunuts* obj, int a2) { return 0; };
                virtual int Step(EnemyDekunuts* obj, float deltaTime) { return 0; };
            };
        };

    private:
        void RequestShot()
        {

        }

        void Shot()
        {

        }

        void AnimationCallback(int a1, int a2, int a3)
        {
            RequestShot();
        }

        inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new animation::AnimCallbackBridge<EnemyDekunuts>();
        }

        inline static void* Appear_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyDekunuts::State::Appear();
        }

        inline static void* Attack_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyDekunuts::State::Attack();
        }

        inline static void* Disappear_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyDekunuts::State::Disappear();
        }

        inline static void* Hide_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyDekunuts::State::Hide();
        }

        inline static void* Retreat_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyDekunuts::State::Retreat();
        }

        inline static ut::internal::StateDescImpl States[] =
        {
            { "Appear", &Appear_Initialize, -1 },
            { "Attack", &Attack_Initialize, -1 },
            { "Disappear", &Disappear_Initialize, -1 },
            { "Hide", &Hide_Initialize, -1 },
            { "Retreat", &Retreat_Initialize, -1 }
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