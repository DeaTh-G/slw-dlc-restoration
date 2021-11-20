#pragma once

namespace app
{
    struct EnemyStalBabyData
    {
        float Speed;
        float MoveRange;
        float AppearRange;
        float SearchRange;
        float SearchHeight;
        bool isEventDriven;
    };

    class EnemyStalBabyInfo : public EnemyInfo
    {
    public:
        int Model{};
        int HeadModel{};
        int Skeleton{};
        animation::AnimationResContainer AnimationContainer{};

        EnemyStalBabyInfo()
        {   
            PacfileName = "EnemyStalbaby.pac";
            animation::AnimationResContainer::__ct(&AnimationContainer, (csl::fnd::IAllocator*)pAllocator);
        }

        void Destructor(size_t deletingFlags) override
        {
            EnemyStalBabyInfo::~EnemyStalBabyInfo();
            animation::AnimationResContainer::__dt(&AnimationContainer);
            EnemyInfo::Destructor(deletingFlags);
        }

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            int animationScript[3]{};

            EnemyInfo::GetModelResource(this, &Model, "enm_stalbaby");
            EnemyInfo::GetModelResource(this, &HeadModel, "enm_stalbaby_head");
            EnemyInfo::GetSkeletonResource(this, &Skeleton, "enm_stalbaby");

            EnemyInfo::GetEnemyPackfile(this, &packFile);

            animationScript[1] = Skeleton;
            ObjUtil::GetAnimationScriptResource(animationScript, "stalbaby", packFile);
            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&AnimationContainer, animationScript, packFile);
            animationScript[0] = 0; animationScript[1] = 0; animationScript[2] = 0;
        }

        const char* GetInfoName() override
        {
            return "EnemyStalBabyInfo";
        }
    };

    class EnemyStalBaby : public EnemyBase
    {
    public:
        fnd::HFrame Children{};
        game::MoveCharacterRigidBody* movement{};
        int field_604{};
        int field_608{};
        int field_60C{};
        int field_610{};
        int field_614{};
        int field_618{};
        int field_61C{};
        float Speed{};
        float field_624 = 225;
        float field_628{};
        int Flags{};

        EnemyStalBaby()
        {
            fnd::HFrame::__ct(&Children);
        }

        void Destructor(size_t deletingFlags)
        {
            fnd::HFrame::__dt(&Children, 0);

            EnemyBase::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            csl::math::Vector3 centerFrame{ 0, 8, 0 };

            fnd::GOComponent::Create(this, GOCGravity);
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationScript);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCShadowSimple);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, EnemyTargetGOC);
            fnd::GOComponent::Create(this, GOCEnemyHSM);
            fnd::GOComponent::Create(this, GOCMovementComplex);

            EnemyStalBabyData* data = (EnemyStalBabyData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            EnemyStalBabyInfo* info = (EnemyStalBabyInfo*)ObjUtil::GetObjectInfo(gameDocument, "EnemyStalBabyInfo");
            Speed = data->Speed;

            fnd::GOComponent::BeginSetup(this);

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
            {
                csl::math::Vector3 position{};
                csl::math::Quaternion rotation{};
                CSetAdapter::GetPosition(*(int**)((char*)this + 0x324), &position);
                CSetAdapter::GetRotation(*(int**)((char*)this + 0x324), &rotation);

                EnemyBase::CreateCenterPositionFrame(this, &centerFrame);
                fnd::HFrame* transformFrame = (fnd::HFrame*)((int*)gocTransform + 0x28);
                fnd::HFrame::AddChild(transformFrame, &Children);
                fnd::HFrame::SetLocalTranslation(&Children, &position);
                fnd::HFrame::SetLocalRotation(&Children, &rotation);
                fnd::HFrame::ResetFlag(&Children, 0x10);
                fnd::HFrame::ResetFlag(&Children, 0x20);
            }

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.Animation |= 0x400000;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                //fnd::GOCVisual::SetVisible(gocVisual, 0);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
                if (gocAnimation)
                {
                    animation::AnimationResContainer* animation = &(info->AnimationContainer);
                    game::GOCAnimationScript::Setup(gocAnimation, &animation);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);

                    csl::fnd::IAllocator* allocator{};
                    auto attackFuncPtr = &EnemyStalBaby::AttackCallback;
                    animation::AnimCallbackBridge<EnemyStalBaby>* attackCallback =
                        (animation::AnimCallbackBridge<EnemyStalBaby>*)AnimCallbackBridge_Initialize(allocator);
                    attackCallback->GameObject = this;
                    attackCallback->field_10 = reinterpret_cast<void*&>(attackFuncPtr);
                    attackCallback->field_14 = -1;

                    game::GOCAnimationScript::RegisterCallback(gocAnimation, 0, attackCallback);

                    auto soundFuncPtr = &EnemyStalBaby::SoundCallback;
                    animation::AnimCallbackBridge<EnemyStalBaby>* soundCallback =
                        (animation::AnimCallbackBridge<EnemyStalBaby>*)AnimCallbackBridge_Initialize(allocator);
                    soundCallback->GameObject = this;
                    soundCallback->field_10 = reinterpret_cast<void*&>(soundFuncPtr);
                    soundCallback->field_14 = -1;

                    game::GOCAnimationScript::RegisterCallback(gocAnimation, 1, attackCallback);
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                csl::math::Vector3 position{};
                int shapeCount = 4;
                game::ColliCapsuleShapeCInfo collisionInfo{};
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_CAPSULE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 10;
                collisionInfo.Height = 3;
                collisionInfo.field_04 |= 1;
                collisionInfo.field_08 = 0x20000;
                ObjUtil::SetupCollisionFilter(9, &collisionInfo);
                collisionInfo.Parent = GetCenterPositionFrame();
                int* shape = game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
                if (shape)
                    ObjUtil::SetEnableColliShape(shape, 0, 0);

                collisionInfo = game::ColliCapsuleShapeCInfo();
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_CAPSULE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 4;
                collisionInfo.Height = 7;
                collisionInfo.field_04 |= 1;
                collisionInfo.field_0C = 1;
                ObjUtil::SetupCollisionFilter(9, &collisionInfo);
                position = Vector3(-1, 0, 12);
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &position);
                collisionInfo.Parent = GetCenterPositionFrame();
                shape = game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
                if (shape)
                    ObjUtil::SetEnableColliShape(shape, 0, 0);

                game::CharacterRigidBodyCInfo rigidBodyInfo{};
                rigidBodyInfo.field_02 = 30;
                rigidBodyInfo.field_0C = 2;
                rigidBodyInfo.field_40 = 8;
                rigidBodyInfo.field_44 = 0;
                rigidBodyInfo.field_4C = 300;
                position = Vector3(0, 8, 0);
                game::CollisionObjCInfo::SetLocalPosition(&rigidBodyInfo, &position);
                game::GOCCollider::CreateCharacterRigidBody(gocCollider, &rigidBodyInfo);

                collisionInfo = game::ColliCapsuleShapeCInfo();
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_CAPSULE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = data->SearchRange;
                collisionInfo.Height = csl::math::Max(data->SearchHeight, data->SearchRange);
                collisionInfo.field_04 |= 3;
                collisionInfo.field_0C = 3;
                ObjUtil::SetupCollisionFilter(6, &collisionInfo);
                collisionInfo.Parent = &Children;
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            int* gocEnemyHsm = GameObject::GetGOC(this, GOCEnemyHsmString);
            if (gocEnemyHsm)
            {
                GOCEnemyHsm::Description hsmDescription{ StateDescriptors, 8, 0 };
                GOCEnemyHsm::Setup(gocEnemyHsm, &hsmDescription);
            }

            game::GOCGravity::SimpleSetup(this, 1);
            game::GOCEffect::SimpleSetupEx(this, 1, 0);
            game::GOCSound::SimpleSetup(this, 0, 0);

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                game::ShadowSphereShapeCInfo shadowInfo;
                game::ShadowSphereShapeCInfo* ppShadowInfo = &shadowInfo;

                game::ShadowSphereShapeCInfo::__ct(&shadowInfo, 3.5f);

                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
            }

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (gocMovement)
            {
                csl::math::Vector3 position{};

                void* movementMem = ((app::fnd::ReferencedObject*)gocMovement)->pAllocator->Alloc
                (sizeof(game::MoveCharacterRigidBody), 16);
                movement = new(movementMem) game::MoveCharacterRigidBody();
                game::GOCMovement::SetupController(gocMovement, movement);

                game::MoveCharacterRigidBody::Description description { 2, 1 };
                movement->Setup(&description);

                CSetAdapter::GetPosition(*(int**)((char*)this + 0x324), &position);
                movement->SetMoveRangeBasePosition(position);
                movement->SetMoveRange(data->MoveRange);

                auto funcPtr = &EnemyStalBaby::NotifyMovementRangeOutCallback;
                game::MoveCharacterRigidBody::FunctionPair functions{ (void*)ASLR(0x0070F590), reinterpret_cast<void*&>(funcPtr) };
                game::MoveCharacterRigidBody::unkStruct unknown{ 0, data->MoveRange };
                movement->SetNotifyMoveRangeOutCallback(functions, unknown, 0);
            }

            int* gocEnemyTarget = GameObject::GetGOC(this, GOCEnemyTargetString);
            if (gocEnemyTarget)
            {
                int count = 0;
                csl::math::Vector3 eyeOffset{ 0, 5, 0 };
                fnd::HFrame centerFrame{};

                GOCEnemyTarget::Setup(gocEnemyTarget, &count);
                GOCEnemyTarget::SetColliderEyesight(gocEnemyTarget, 3);
                GOCEnemyTarget::SetEyesightOffset(gocEnemyTarget, &eyeOffset);
                centerFrame = *GetCenterPositionFrame();
                GOCEnemyTarget::SetEyesightFrame(gocEnemyTarget, &centerFrame);
            }
            
            SetRandomPosition();
            if (data->isEventDriven)
            {
                Sleep(this);
                fnd::GOComponent::EndSetup(this);
                return;
            }

            app::CLevelInfo* levelInfo = (app::CLevelInfo*)app::CLevelInfo::GetService(gameDocument);
            if (levelInfo && levelInfo->IsPlayingZeldaEvent())
            {
                Flags |= 8;
                Sleep(this);
            }

            fnd::GOComponent::EndSetup(this); 
        }


        class State
        {
        public:
            class Idle
            {
                char field_00[20];
                int* GocEnemyTarget;

            public:
                virtual ~Idle() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2)
                {
                    return EnemyState::Update(this, obj, a2);
                };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };
                virtual int OnEnter(EnemyStalBaby* obj, int a2)
                {
                    int* gocVisual = GameObject::GetGOC(obj, GOCVisual);
                    if (gocVisual)
                        fnd::GOCVisual::SetVisible(gocVisual, 0);

                    int* gocShadow = GameObject::GetGOC(obj, GOCShadowString);
                    if (gocShadow)
                        game::GOCShadowSimple::SetVisible(gocShadow, 0);

                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (gocAnimation)
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE");

                    GocEnemyTarget = GameObject::GetGOC(obj, GOCEnemyTargetString);

                    MoveStop(obj);

                    return 1;
                };

                virtual int OnLeave(EnemyStalBaby* obj, int a2) 
                {
                    int* gocVisual = GameObject::GetGOC(obj, GOCVisual);
                    if (gocVisual)
                        fnd::GOCVisual::SetVisible(gocVisual, 1);

                    int* gocShadow = GameObject::GetGOC(obj, GOCShadowString);
                    if (gocShadow)
                        game::GOCShadowSimple::SetVisible(gocShadow, 1);

                    return 1;
                };
                
                virtual int Step(EnemyStalBaby* obj, float deltaTime)
                { 
                    if (GOCEnemyTarget::IsFindTarget(GocEnemyTarget))
                    {
                        GOCEnemyTarget::LockTarget(GocEnemyTarget);

                        int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                        if (!gocEnemyHsm)
                            return 0;

                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 1);
                        return 1;
                    }

                    return 0;
                };
            };

            class Find
            {
                char field_00[20];
                int GOCAnimation;
                float Time;

            public:
                virtual ~Find() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };
                virtual int OnEnter(EnemyStalBaby* obj, int a2)
                {
                    /*csl::math::Vector3 targetPosition{};

                    Time = 0;
                    MoveStop(obj);

                    int* gocEnemyTarget = GameObject::GetGOC(obj, GOCEnemyTargetString);
                    if (gocEnemyTarget)
                        return 0;

                    if (!GOCEnemyTarget::IsFindTarget(gocEnemyTarget))
                        return 0;

                    GOCEnemyTarget::GetTargetCenterPosition(gocEnemyTarget, &targetPosition);

                    */
                    return 0;
                };
                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int Step(EnemyStalBaby* obj, float deltaTime) { return 0; };
            };

            class Chase
            {
            public:
                virtual ~Chase() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };
                virtual int OnEnter(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int Step(EnemyStalBaby* obj, float deltaTime) { return 0; };
            };

            class Attack
            {
            public:
                virtual ~Attack() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };
                virtual int OnEnter(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int Step(EnemyStalBaby* obj, float deltaTime) { return 0; };
            };

            class Gap
            {
            public:
                virtual ~Gap() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };
                virtual int OnEnter(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int Step(EnemyStalBaby* obj, float deltaTime) { return 0; };
            };

            class Stagger
            {
            public:
                virtual ~Stagger() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };
                virtual int OnEnter(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int Step(EnemyStalBaby* obj, float deltaTime) { return 0; };
            };

            class Hide
            {
            public:
                virtual ~Hide() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };
                virtual int OnEnter(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int Step(EnemyStalBaby* obj, float deltaTime) { return 0; };
            };

            class Dead
            {
            public:
                virtual ~Dead() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };
                virtual int OnEnter(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };
                virtual int Step(EnemyStalBaby* obj, float deltaTime) { return 0; };
            };

            static void MoveStop(EnemyStalBaby* obj)
            {
                int* gocMovement = GameObject::GetGOC(obj, GOCMovementString);
                if (gocMovement)
                {
                    int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
                    *(csl::math::Vector3*)(contextParam + 8) = Vector3(0, 0, 0);
                }
            }
        };

    private:
        void AttackCallback(int a1, int a2, int a3)
        {
        }

        void SoundCallback(int a1, int a2, int a3)
        {
        }

        void NotifyMovementRangeOutCallback()
        {
        }

        inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new animation::AnimCallbackBridge<EnemyStalBaby>();
        }

        inline static void* Idle_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyStalBaby::State::Idle();
        }

        inline static void* Find_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyStalBaby::State::Find();
        }

        inline static void* Chase_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyStalBaby::State::Chase();
        }

        inline static void* Attack_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyStalBaby::State::Attack();
        }

        inline static void* Gap_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyStalBaby::State::Gap();
        }

        inline static void* Stagger_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyStalBaby::State::Stagger();
        }

        inline static void* Hide_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyStalBaby::State::Hide();
        }

        inline static void* Dead_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new EnemyStalBaby::State::Dead();
        }

        inline static ut::internal::StateDescImpl States[] =
        {
            { "Idle", &Idle_Initialize, -1 },
            { "Find", &Find_Initialize, -1 },
            { "Chase", &Chase_Initialize, -1 },
            { "Attack", &Attack_Initialize, -1 },
            { "Gap", &Gap_Initialize, -1 },
            { "Stagger",&Stagger_Initialize, -1 },
            { "Hide", &Hide_Initialize, -1 },
            { "Dead", &Dead_Initialize, -1 }
        };

        inline static GOCEnemyHsm::StateDesc StateDescriptors[] =
        {
            {0, &States[0]},
            {1, &States[1]},
            {2, &States[2]},
            {3, &States[3]},
            {4, &States[4]},
            {5, &States[5]},
            {6, &States[6]},
            {7, &States[7]},
        };

        void SetRandomPosition()
        {
            unsigned int random = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));

            csl::math::Vector3 zUp { 0, 0, 1 };
            csl::math::Vector3 offsetDir{};
            csl::math::Vector3 offsetAppear{};
            csl::math::Vector3 rayStartPosition{};
            csl::math::Vector3 rayEndPosition{};
            EnemyStalBabyData* data = (EnemyStalBabyData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Matrix34 m{};
            m = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);
            Eigen::MatrixXf transformMatrix(4, 4);
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    transformMatrix(i, j) = m.data[i][j];

            float appearRange = ((float)random * 2.328306437080797e-10f) * (float)data->AppearRange;
            float offsetAngle = ((float)random * 2.328306437080797e-10f) * 3.1415927f;
            offsetAngle += offsetAngle;
            math::Vector3RotateY(&offsetDir, offsetAngle, &zUp);
            math::Vector3Scale(&offsetDir, appearRange, &offsetAppear);
            auto rayBase = (transformMatrix.transpose() *
                Eigen::Vector4f(offsetAppear.X, offsetAppear.Y, offsetAppear.Z, 1)).head<3>();
            csl::math::Vector3 rayBasePosition { rayBase.x(), rayBase.y(), rayBase.z() };

            csl::math::Vector3 leftVector = { -m.data[0][1], m.data[1][1], -m.data[2][1] };
            math::Vector3Scale(&leftVector, 50, &leftVector);
            math::Vector3Add(&rayBasePosition, &leftVector, &rayEndPosition);
            math::Vector3Subtract(&rayBasePosition, &leftVector, &rayStartPosition);
            int* rayResult = ObjUtil::RaycastNearestCollision(&m, (GameDocument*)field_24[1], &rayEndPosition, &rayStartPosition, 0xC996);
            offsetAppear = csl::math::Vector3(m.data[0][0], m.data[0][1], m.data[0][2]);
            fnd::GOCTransform::SetLocalTranslation((int*)gocTransform, &offsetAppear);
        }

        void OnDead()
        {
            unsigned int random = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));
            SetStatusTemporaryRetire(((float)random * 2.328306437080797e-10) * 3 + 3);
            Kill();
        }

        float GetMoveSpeed() { return Speed; }

    };

    inline static EnemyStalBaby* create_EnemyStalBaby()
    {
        return new EnemyStalBaby();
    }

    inline static EnemyStalBabyInfo* createObjInfo_EnemyStalBabyInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) EnemyStalBabyInfo();
    }
}