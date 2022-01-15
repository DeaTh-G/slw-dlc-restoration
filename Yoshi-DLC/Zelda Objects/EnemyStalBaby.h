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
        INSERT_PADDING(3);
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
            if (IsStalbabyFixed)
                PacfileName = "EnemyStalbabyFix.pac";
            else
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

            ObjUtil::GetAnimationScriptResource(animationScript, "stalbaby", packFile);
            animationScript[1] = Skeleton;
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
        csl::math::Vector3 deathPosition{};
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
                fnd::GOCVisual::SetVisible(gocVisual, 0);

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

                    game::GOCAnimationScript::RegisterCallback(gocAnimation, 1, soundCallback);
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                csl::math::Vector3 position{};
                int shapeCount = 4;
                game::ColliCapsuleShapeCInfo collisionInfo{};
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                // Damage Collider
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
                    ObjUtil::SetEnableColliShape(gocCollider, 0, false);

                // Hand Damage Collider
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
                    ObjUtil::SetEnableColliShape(gocCollider, 1, false);

                game::CharacterRigidBodyCInfo rigidBodyInfo{};
                rigidBodyInfo.field_02 = 30;
                rigidBodyInfo.field_0C = 2;
                rigidBodyInfo.field_40 = 8;
                rigidBodyInfo.field_44 = 0;
                rigidBodyInfo.field_4C = 300;
                position = Vector3(0, 8, 0);
                game::CollisionObjCInfo::SetLocalPosition(&rigidBodyInfo, &position);
                shape = game::GOCCollider::CreateCharacterRigidBody(gocCollider, &rigidBodyInfo);

                // Search Collider
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

                game::MoveCharacterRigidBody::Description description{ 2, 1 };
                movement->Setup(&description);

                CSetAdapter::GetPosition(*(int**)((char*)this + 0x324), &position);
                movement->SetMoveRangeBasePosition(position);
                movement->SetMoveRange(data->MoveRange);

                auto funcPtr = &EnemyStalBaby::NotifyMovementRangeOutCallback;
                game::MoveCharacterRigidBody::FunctionPair functions{ (void*)ASLR(0x0070F590), reinterpret_cast<void*&>(funcPtr) };
                game::MoveCharacterRigidBody::unkStruct unknown{ 0, 5 };
                movement->SetNotifyMoveRangeOutCallback(functions, unknown, 0);
            }

            int* gocEnemyTarget = GameObject::GetGOC(this, GOCEnemyTargetString);
            if (gocEnemyTarget)
            {
                int count = 0;
                fnd::HFrame centerFrame{};

                GOCEnemyTarget::Setup(gocEnemyTarget, &count);
                GOCEnemyTarget::SetColliderEyesight(gocEnemyTarget, 3);
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
            case fnd::PROC_MSG_KICK:
                ProcMsgKick((xgame::MsgKick&)message);
                return true;
            case fnd::PROC_MSG_NOTIFY_OBJECT_EVENT:
                ProcMsgNotifyObjectEvent((xgame::MsgNotifyObjectEvent&)message);
                return true;
            case fnd::PROC_MSG_PL_GET_HOMING_TARGET_INFO:
                ProcMsgPLGetHomingTargetInfo((xgame::MsgPLGetHomingTargetInfo&)message);
                return true;
            default:
                EnemyBase::ProcessMessage(message);
                return true;
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (!movement)
                return;
        
            if (movement->IsOnGround())
            {
                field_628 = 0;
            }
            else
            {
                field_628 += updateInfo.deltaTime;
                if (field_628 < 1)
                    return;

                enemy::DeadEffectCInfo effectInfo{};
                enemy::DeadEffectCInfo::__ct(&effectInfo);
                GameObjectHandleBase::__ct(&effectInfo, this);
                effectInfo.field_60 = 0;
                effectInfo.field_62 &= ~6;

                void* enemyManager = app::EnemyManager::GetService(*app::Document);
                app::EnemyManager::CreateDeadEffect(enemyManager, &effectInfo);
                OnDead();
            }
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
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
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
                int* GOCAnimation;
                Effect::CEffectHandle EffectHandle{};
                float Time;

            public:
                virtual ~Find() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };

                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message)
                {
                    if (message.Type == fnd::PROC_MSG_DLC_ZELDA_NOTICE_STOP_ENEMY)
                        return ProcMsgDlcZeldaNoticeStopEnemy((xgame::MsgDlcZeldaNoticeStopEnemy&)message);
                    else if (message.Type == fnd::PROC_MSG_DLC_ZELDA_NOTICE_ACTIVE_ENEMY)
                        return ProcMsgDlcZeldaNoticeActiveEnemy((xgame::MsgDlcZeldaNoticeActiveEnemy&)message);
                    else
                        return false;
                    return 0;
                };

                virtual int OnEnter(EnemyStalBaby* obj, int a2)
                {
                    csl::math::Vector3 targetPosition{};

                    Time = 0;
                    MoveStop(obj);

                    int* gocEnemyTarget = GameObject::GetGOC(obj, GOCEnemyTargetString);
                    if (!gocEnemyTarget)
                        return 0;

                    if (!GOCEnemyTarget::IsFindTarget(gocEnemyTarget))
                        return 0;

                    GOCEnemyTarget::GetTargetCenterPosition(gocEnemyTarget, &targetPosition);

                    fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(obj, GOCTransformString);
                    if (!gocTransform)
                        return 0;

                    csl::math::Matrix34 m{};
                    m = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);

                    csl::math::Vector3 position = Vector3(m.data[3][0], m.data[3][1], m.data[3][2]);
                    math::Vector3Subtract(&targetPosition, &position, &position);
                    if (!math::Vector3NormalizeIfNotZero(&position, &position))
                        return 0;

                    csl::math::Vector3 leftVector = Vector3(m.data[1][0], m.data[1][1], m.data[1][2]);

                    if (fabs(math::Vector3DotProduct(&leftVector, &position) >= 0.99999899f))
                        return 0;

                    csl::math::Vector3 upVector{};
                    csl::math::Vector3 forwardVector{};
                    math::Vector3CrossProduct(&leftVector, &position, &upVector);
                    math::Vector3CrossProduct(&upVector, &leftVector, &forwardVector);

                    *(csl::math::Vector3*)&m.data[0] = upVector;
                    *(csl::math::Vector3*)&m.data[1] = leftVector;
                    *(csl::math::Vector3*)&m.data[2] = forwardVector;
                    *(csl::math::Vector3*)&m.data[3] = Vector3(0, 0, 0);
                    csl::math::Quaternion rotation = GetRotationFromMatrix(&m);
                    csl::math::QuaternionNormalize(&rotation, &rotation);
                    fnd::GOCTransform::SetLocalRotation((int*)gocTransform, &rotation);

                    GOCAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (!GOCAnimation)
                        return 0;

                    game::GOCAnimationScript::ChangeAnimation(GOCAnimation, "APPEAR");

                    int deviceTag[3]{};
                    int* gocSound = GameObject::GetGOC(obj, GOCSoundString);
                    if (!gocSound)
                        return 0;

                    game::GOCSound::Play3D(gocSound, deviceTag, "enm_stullbaby_appear", 0);

                    int* gocEffect = GameObject::GetGOC(obj, GOCEffectString);
                    if (!gocEffect)
                        return 0;

                    game::GOCEffect::CreateEffectLoop(gocEffect, &EffectHandle, "ef_dl3_stalbaby_appear");

                    return 1;
                };

                virtual int OnLeave(EnemyStalBaby* obj, int a2)
                {
                    obj->SetEnableCollision(1, 0);
                    return 1;
                };

                virtual int Step(EnemyStalBaby* obj, float deltaTime)
                {
                    Time += deltaTime;
                    float effectHeight = (Time * 30) - 15;
                    effectHeight = csl::math::Min(effectHeight, 0);
                    csl::math::Vector3 height{ 0, effectHeight, 0 };

                    int* gocShadow = GameObject::GetGOC(obj, GOCShadowString);
                    if (!gocShadow)
                        return 0;

                    game::GOCShadowSimple::SetLocalOffsetPosition(gocShadow, &height);

                    if (effectHeight < 0 || !game::GOCAnimationScript::IsFinished(GOCAnimation))
                        return 0;

                    EffectHandle.Stop(0);

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return 0;

                    GOCEnemyHsm::ChangeState(gocEnemyHsm, 2);
                    return 1;
                };

            private:
                bool ProcMsgDlcZeldaNoticeStopEnemy(xgame::MsgDlcZeldaNoticeStopEnemy& message)
                {
                    EffectHandle.SetVisible(false);
                    return true;
                }

                bool ProcMsgDlcZeldaNoticeActiveEnemy(xgame::MsgDlcZeldaNoticeActiveEnemy& message)
                {
                    EffectHandle.SetVisible(true);
                    return true;
                }
            };

            class Chase
            {
                char field_00[20];
                int* GocEnemyTarget;

            public:
                virtual ~Chase() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };

                virtual int OnEnter(EnemyStalBaby* obj, int a2)
                {
                    GocEnemyTarget = GameObject::GetGOC(obj, GOCEnemyTargetString);

                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (!GOCAnimation)
                        return 0;

                    game::GOCAnimationScript::ChangeAnimation(gocAnimation, "WALK");
                    return 1;
                };

                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };

                virtual int Step(EnemyStalBaby* obj, float deltaTime)
                {
                    csl::math::Vector3 targetPosition{};

                    if ((obj->Flags & 1))
                    {
                        obj->Flags &= ~1;
                        int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                        if (!gocEnemyHsm)
                            return 0;

                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 6);
                        return 0;
                    }
                    else if (GOCEnemyTarget::IsFindTarget(GocEnemyTarget) &&
                        GOCEnemyTarget::GetTargetCenterPosition(GocEnemyTarget, &targetPosition) &&
                        IsInRangeAttack(obj, targetPosition))
                    {
                        int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                        if (!gocEnemyHsm)
                            return 0;

                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 3);
                        return 0;
                    }
                    else
                    {
                        int* gocMovement = GameObject::GetGOC(obj, GOCMovementString);
                        if (!gocMovement)
                            return 0;

                        if (GOCEnemyTarget::IsFindTarget(GocEnemyTarget) &&
                            GOCEnemyTarget::GetTargetCenterPosition(GocEnemyTarget, &targetPosition))
                        {
                            csl::math::Vector3 turnDirection{};
                            obj->GetTurnDirection(&turnDirection, targetPosition, deltaTime);
                            float magnitude{};
                            math::Vector3SquareMagnitude(&turnDirection, &magnitude);
                            if (magnitude > 0.000001f)
                            {
                                csl::math::Vector3* moveVector = (csl::math::Vector3*)(game::GOCMovement::GetContextParam(gocMovement) + 8);
                                float speed = obj->GetMoveSpeed();
                                math::Vector3Scale(&turnDirection, speed, moveVector);
                                return 0;
                            }
                            else
                            {
                                int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                                if (!gocEnemyHsm)
                                    return 0;

                                GOCEnemyHsm::ChangeState(gocEnemyHsm, 6);
                                return 1;
                            }
                        }
                    }

                    return 0;
                };
            };

            class Attack
            {
                char field_00[20];
                int* GOCAnimation;

            public:
                virtual ~Attack() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };

                virtual int OnEnter(EnemyStalBaby* obj, int a2)
                {
                    GOCAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (!GOCAnimation)
                        return 0;

                    game::GOCAnimationScript::ChangeAnimation(GOCAnimation, "ATTACK");
                    MoveStop(obj);

                    return 1;
                };

                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };

                virtual int Step(EnemyStalBaby* obj, float deltaTime)
                {
                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return 0;

                    if (!game::GOCAnimationScript::IsFinished(GOCAnimation))
                        return 0;

                    if ((obj->Flags & 1))
                    {
                        obj->Flags &= ~1;
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 6);
                    }
                    else
                    {
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 4);
                    }

                    return 1;
                };
            };

            class Gap
            {
                char field_00[20];
                int* GocEnemyTarget;
                float Countdown;

            public:
                virtual ~Gap() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };

                virtual int OnEnter(EnemyStalBaby* obj, int a2)
                {
                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (!gocAnimation)
                        return 0;

                    game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE");

                    GocEnemyTarget = GameObject::GetGOC(obj, GOCEnemyTargetString);
                    Countdown = 2;

                    MoveStop(obj);

                    return 1;
                };

                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };

                virtual int Step(EnemyStalBaby* obj, float deltaTime)
                {
                    csl::math::Vector3 targetPosition{};

                    Countdown -= deltaTime;
                    if (Countdown > 0)
                        return 0;

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return 0;

                    if ((obj->Flags & 1))
                    {
                        obj->Flags &= ~1;
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 6);
                    }
                    else if (GOCEnemyTarget::IsFindTarget(GocEnemyTarget) &&
                        GOCEnemyTarget::GetTargetCenterPosition(GocEnemyTarget, &targetPosition) &&
                        IsInRangeAttack(obj, targetPosition))
                    {
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 3);
                    }
                    else
                    {
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 2);
                    }

                    return 1;
                };
            };

            class Stagger
            {
                char field_00[20];
                int* GOCAnimation;
                float Countdown;
                char field_1C;
                INSERT_PADDING(3);

            public:
                virtual ~Stagger() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };

                virtual int OnEnter(EnemyStalBaby* obj, int a2)
                {
                    GOCAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (!GOCAnimation)
                        return 0;

                    game::GOCAnimationScript::ChangeAnimation(GOCAnimation, "HITBACK");
                    Countdown = 4;
                    field_1C = 0;
                    MoveStop(obj);

                    return 1;
                };

                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };

                virtual int Step(EnemyStalBaby* obj, float deltaTime)
                {
                    Countdown -= deltaTime;
                    if (Countdown > 0)
                        return 0;

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return 0;

                    if (!field_1C)
                    {
                        game::GOCAnimationScript::ExitLoop(GOCAnimation);
                        field_1C = 1;
                        return 0;
                    }

                    if (!game::GOCAnimationScript::IsFinished(GOCAnimation))
                        return 0;

                    if ((obj->Flags & 1))
                    {
                        obj->Flags &= ~1;
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 6);
                        return 0;
                    }

                    int* gocEnemyTarget = GameObject::GetGOC(obj, GOCEnemyTargetString);
                    if (gocEnemyTarget && GOCEnemyTarget::IsFindTarget(gocEnemyTarget))
                    {
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 2);
                        return 0;
                    }

                    GOCEnemyHsm::ChangeState(gocEnemyHsm, 0);

                    return 1;
                };
            };

            class Hide
            {
                char field_00[20];
                Effect::CEffectHandle EffectHandle{};
                float Time;

            public:
                virtual ~Hide() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };

                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message)
                {
                    if (message.Type == fnd::PROC_MSG_DLC_ZELDA_NOTICE_STOP_ENEMY)
                        return ProcMsgDlcZeldaNoticeStopEnemy((xgame::MsgDlcZeldaNoticeStopEnemy&)message);
                    else if (message.Type == fnd::PROC_MSG_DLC_ZELDA_NOTICE_ACTIVE_ENEMY)
                        return ProcMsgDlcZeldaNoticeActiveEnemy((xgame::MsgDlcZeldaNoticeActiveEnemy&)message);
                    else
                        return false;
                    return 0;
                };

                virtual int OnEnter(EnemyStalBaby* obj, int a2)
                {
                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (!gocAnimation)
                        return 0;

                    game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE");
                    obj->SetEnableCollision(false, 0);

                    int* gocEffect = GameObject::GetGOC(obj, GOCEffectString);
                    if (!gocEffect)
                        return 0;

                    game::GOCEffect::CreateEffectLoop(gocEffect, &EffectHandle, "ef_dl3_stalbaby_appear");

                    MoveStop(obj);

                    return 0;
                };

                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };

                virtual int Step(EnemyStalBaby* obj, float deltaTime)
                {
                    Time += deltaTime;
                    float heightOffset = -(Time * 30);
                    csl::math::Vector3 height{ 0, heightOffset, 0 };

                    int* gocVisual = GameObject::GetGOC(obj, GOCVisual);
                    if (!gocVisual)
                        return 0;

                    fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &height);

                    int* gocShadow = GameObject::GetGOC(obj, GOCShadowString);
                    if (!gocShadow)
                        return 0;

                    game::GOCShadowSimple::SetLocalOffsetPosition(gocShadow, &height);

                    if (heightOffset <= -15)
                    {
                        EffectHandle.Stop(0);
                        obj->OnDead();
                    }

                    return 1;
                };

            private:
                bool ProcMsgDlcZeldaNoticeStopEnemy(xgame::MsgDlcZeldaNoticeStopEnemy& message)
                {
                    EffectHandle.SetVisible(false);
                    return true;
                }

                bool ProcMsgDlcZeldaNoticeActiveEnemy(xgame::MsgDlcZeldaNoticeActiveEnemy& message)
                {
                    EffectHandle.SetVisible(true);
                    return true;
                }
            };

            class Dead
            {
                char field_00[20];
                int* GOCAnimation;
                float Time;
                char field_1C;
                INSERT_PADDING(3);

            public:
                virtual ~Dead() {};
                virtual int Trigger(EnemyStalBaby* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyStalBaby* obj) { return 0; };
                virtual int Enter(EnemyStalBaby* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyStalBaby* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyStalBaby* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyStalBaby* obj, fnd::Message& message) { return 0; };

                virtual int OnEnter(EnemyStalBaby* obj, int a2)
                {
                    const char* animationName = "DEAD";

                    GOCAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (!GOCAnimation)
                        return 0;

                    Time = 0;
                    field_1C = (obj->Flags >> 1) & 1;
                    if (field_1C)
                        animationName = "DEAD_NO_HEAD";

                    game::GOCAnimationScript::ChangeAnimation(GOCAnimation, animationName);

                    int* gocEffect = GameObject::GetGOC(obj, GOCEffectString);
                    if (!gocEffect)
                        return 0;

                    game::GOCEffect::CreateEffect(gocEffect, "ef_dl3_stalbaby_dead");
                    obj->SetEnableCollision(false, 0);
                    MoveStop(obj);

                    return 0;
                };

                virtual int OnLeave(EnemyStalBaby* obj, int a2) { return 0; };

                virtual int Step(EnemyStalBaby* obj, float deltaTime)
                {
                    if (field_1C)
                    {
                        EnemyStalBabyInfo* info = (EnemyStalBabyInfo*)ObjUtil::GetObjectInfo((GameDocument*)obj->field_24[1], "EnemyStalBabyInfo");

                        int* gocVisual = GameObject::GetGOC(obj, GOCVisual);
                        if (!gocVisual)
                            return 0;

                        math::Transform transform{};
                        fnd::GOCVisualModel::GetNodeTransform(gocVisual, 0, "head", &transform);

                        csl::math::Matrix34 m{};
                        math::Transform::GetTransformMatrix(&transform, &m);

                        EnemyBlowOffObjectCInfo blowOffInfo{};
                        EnemyBlowOffObjectCInfo::__ct(&blowOffInfo);
                        blowOffInfo.Model = info->HeadModel;
                        blowOffInfo.field_10 = m;
                        blowOffInfo.field_50.Y = 5;
                        blowOffInfo.field_60 = 4;
                        blowOffInfo.field_6C = 5;
                        blowOffInfo.field_70 = obj->deathPosition;
                        blowOffInfo.field_80 = 0;

                        EnemyBase::CreateEnemyBlowOffObject(obj, &blowOffInfo);

                        field_1C = 0;
                    }

                    if (game::GOCAnimationScript::IsFinished(GOCAnimation))
                        obj->OnDead();

                    return 1;
                };
            };

        private:
            static bool IsInRangeAttack(EnemyStalBaby* obj, csl::math::Vector3& targetPosition)
            {
                csl::math::Vector3 position{};
                csl::math::Vector3 upVector{};
                csl::math::Vector3 forwardVector{};

                fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(obj, GOCTransformString);
                if (!gocTransform)
                    return false;

                csl::math::Matrix34 m{};
                m = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);
                upVector = Vector3(m.data[0][0], m.data[0][1], m.data[0][2]);
                forwardVector = Vector3(m.data[2][0], m.data[2][1], m.data[2][2]);
                position = Vector3(m.data[3][0], m.data[3][1], m.data[3][2]);
                math::Vector3Subtract(&targetPosition, &position, &position);

                float magnitude{};
                math::Vector3SquareMagnitude(&position, &magnitude);
                if (magnitude >= obj->field_624)
                    return false;

                math::Vector3NormalizeIfNotZero(&position, &position);

                if (math::Vector3DotProduct(&upVector, &position) <= 0)
                {
                    if (cosf(0.69813168f) > math::Vector3DotProduct(&forwardVector, &position))
                        return false;
                }
                else
                {
                    if (cosf(0.08726646f) > math::Vector3DotProduct(&forwardVector, &position))
                        return false;
                }

                return true;
            }

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
            int* gocCollider = GameObject::GetGOC((GameObject*)((char*)this + 1), GOCColliderString);
            if (!gocCollider)
                return;

            if (!a2)
            {
                if (a3 == 1)
                {
                    ObjUtil::SetEnableColliShape(gocCollider, 1, false);
                }
                else
                {
                    ObjUtil::SetEnableColliShape(gocCollider, 1, true);
                }
            }
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
                    game::GOCSound::Play3D(gocSound, deviceTag, "enm_stullbaby_punch", 0);
                }
                else
                {
                    game::GOCSound::Play3D(gocSound, deviceTag, "enm_stullbaby_walk", 0);
                }
            }
        }

        void NotifyMovementRangeOutCallback()
        {
            Flags |= 1;
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

            csl::math::Vector3 zUp{ 0, 0, 1 };
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
            csl::math::Vector3 rayBasePosition{ rayBase.x(), rayBase.y(), rayBase.z() };

            csl::math::Vector3 leftVector = { m.data[1][0], m.data[1][1], m.data[1][2] };
            math::Vector3Scale(&leftVector, 50, &leftVector);
            math::Vector3Add(&rayBasePosition, &leftVector, &rayEndPosition);
            math::Vector3Subtract(&rayBasePosition, &leftVector, &rayStartPosition);

            game::PhysicsRaycastOutput output{};
            int* rayResult = ObjUtil::RaycastNearestCollision(&output, (GameDocument*)field_24[1], &rayEndPosition, &rayStartPosition, 0xC996);
            fnd::GOCTransform::SetLocalTranslation((int*)gocTransform, &output.field_00);
        }

        void OnDead()
        {
            unsigned int random = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));
            SetStatusTemporaryRetire(((float)random * 2.328306437080797e-10) * 3 + 3);
            Kill();
        }

        float GetMoveSpeed() { return Speed; }

        void SetEnableCollision(bool a1, int a2)
        {
            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (!gocCollider)
                return;

            ObjUtil::SetEnableColliShape(gocCollider, 0, a1);
            ObjUtil::SetEnableColliShape(gocCollider, 1, a2);
            ObjUtil::SetEnableColliShape(gocCollider, 2, a1);

            if (a1)
                Flags |= 4;
            else
                Flags &= ~4;
        }

        void GetTurnDirection(csl::math::Vector3* turnDirection, csl::math::Vector3& targetPosition, float deltaTime)
        {
            csl::math::Vector3 position{};
            csl::math::Vector3 upVector{};
            csl::math::Vector3 leftVector{};
            csl::math::Vector3 forwardVector{};
            csl::math::Vector3 turnDir{};

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Matrix34 m{};
            m = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);
            upVector = Vector3(m.data[0][0], m.data[0][1], m.data[0][2]);
            leftVector = Vector3(m.data[1][0], m.data[1][1], m.data[1][2]);
            forwardVector = Vector3(m.data[2][0], m.data[2][1], m.data[2][2]);
            position = Vector3(m.data[3][0], m.data[3][1], m.data[3][2]);
            math::Vector3Subtract(&targetPosition, &position, &position);
            float leftDirection = math::Vector3DotProduct(&position, &leftVector);
            math::Vector3Scale(&leftVector, leftDirection, &turnDir);
            math::Vector3Subtract(&position, &turnDir, &turnDir);
            float magnitude{};
            math::Vector3SquareMagnitude(&turnDir, &magnitude);
            if (magnitude <= 0.000001f)
            {
                turnDir = Vector3(0, 0, 0);
            }
            else
            {
                float horizontalDirection = math::Vector3DotProduct(&turnDir, &forwardVector);
                csl::math::Vector3Normalize(&turnDir, turnDirection);
                float time = 2.3561945f * deltaTime;
                horizontalDirection = csl::math::Clamp(horizontalDirection, -1, 1);
                if (time < acosf(horizontalDirection))
                {
                    float lookDirection = math::Vector3DotProduct(&upVector, &turnDir);
                    float angle = csl::math::Select(lookDirection, fabs(time), -abs(time));
                    csl::math::Quaternion rotation{ &rotation, &leftVector, angle };
                    math::Vector3Rotate(turnDirection, &rotation, &forwardVector);
                }
            }
        }

        void ProcMsgDamage(xgame::MsgDamage& message)
        {
            if (!EnemyUtil::IsDamage(message.field_2C, 0, message.AttackType))
                return;

            int* gocEnemyHsm = GameObject::GetGOC(this, GOCEnemyHsmString);
            if (!gocEnemyHsm)
                return;

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            if (EnemyUtil::IsFatalDamage(message) || GOCEnemyHsm::GetCurrentStateID(gocEnemyHsm) == 5)
            {
                csl::math::Vector3 position{};

                math::CalculatedTransform::GetTranslation((csl::math::Matrix34*)((int*)gocTransform + 0x44), &position);
                message.SetReply(&position, 1);
                enemy::DeadEffectCInfo effectInfo{};
                enemy::DeadEffectCInfo::__ct(&effectInfo);
                GameObjectHandleBase::__ct(&effectInfo, this);
                enemy::DeadEffectCInfo::SetMsgDamage(&effectInfo, &message);
                enemy::DeadEffectCInfo::SetZeldaStalBaby(&effectInfo);

                void* enemyManager = EnemyManager::GetService((GameDocument*)field_24[1]);
                EnemyManager::CreateDeadEffect(enemyManager, &effectInfo);
                EnemyBase::ProcMission(this, &message);
                ObjUtil::AddScore(this, "STALBABY", &message);

                GOCEnemyHsm::ChangeState(gocEnemyHsm, 7);
            }
            else if (message.field_28 == 3)
            {
                csl::math::Vector3 position{};

                math::CalculatedTransform::GetTranslation((csl::math::Matrix34*)((int*)gocTransform + 0x44), &position);
                message.SetReply(&position, 1);
                ObjUtil::AddScore(this, "STALBABY", &message);
                enemy::DeadEffectCInfo effectInfo{};
                enemy::DeadEffectCInfo::__ct(&effectInfo);
                GameObjectHandleBase::__ct(&effectInfo, this);
                enemy::DeadEffectCInfo::SetMsgDamage(&effectInfo, &message);
                enemy::DeadEffectCInfo::SetZeldaStalBaby(&effectInfo);

                void* enemyManager = EnemyManager::GetService((GameDocument*)field_24[1]);
                EnemyManager::CreateDeadEffect(enemyManager, &effectInfo);
                EnemyBase::ProcMission(this, &message);
                ObjUtil::AddScore(this, "STALBABY", &message);

                GOCEnemyHsm::ChangeState(gocEnemyHsm, 7);
            }
            else
            {
                if (GOCEnemyHsm::GetCurrentStateID(gocEnemyHsm) == 7)
                    return;

                int* colli1 = fnd::Handle::Get(&message.field_18);
                int* colli2 = fnd::Handle::Get(&message.field_20);
                float contactPoint[20]{};
                if (colli1 && colli2 && game::ColliShapeHavok::GetClosestPoint(colli1, colli2, contactPoint))
                {
                    if (AttackType::And(message.AttackType, 64))
                        message.field_90 |= 1;

                    csl::math::Vector3 position{};
                    math::CalculatedTransform::GetTranslation((csl::math::Matrix34*)((int*)gocTransform + 0x44), &position);
                    message.SetReply(&position, 0);

                    int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
                    if (!gocEffect)
                        return;

                    csl::math::Matrix34 m{};
                    m = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);
                    *(csl::math::Vector3*)(&m.data[3][0]) = *(csl::math::Vector3*)&contactPoint;
                    game::GOCEffect::CreateEffectWorld(gocEffect, "ef_en_com_wt1_hit", &m, 1);
                }

                int* gocEnemyTarget = GameObject::GetGOC(this, GOCEnemyTargetString);
                if (!gocEnemyTarget)
                    return;

                if (GOCEnemyTarget::IsFindTarget(gocEnemyTarget))
                    GOCEnemyTarget::LockTarget(gocEnemyTarget);

                int deviceTag[3]{};
                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (!gocSound)
                    return;

                game::GOCSound::Play3D(gocSound, deviceTag, "enm_cmn_damage", 0);
                GOCEnemyHsm::ChangeState(gocEnemyHsm, 5);
            }
        }

        void ProcMsgDlcZeldaNoticeActiveEnemy(xgame::MsgDlcZeldaNoticeActiveEnemy& message)
        {
            int* gocEnemyHsm = GameObject::GetGOC(this, GOCEnemyHsmString);
            if (!gocEnemyHsm)
                return;

            if (!GOCEnemyHsm::GetCurrentStateID(gocEnemyHsm))
            {
                Resume();
                Flags &= ~8;

                return;
            }

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
                fnd::GOCVisual::SetVisible(gocVisual, true);

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
                game::GOCShadow::SetVisible(gocShadow, true);

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
            if (gocAnimation)
                game::GOCAnimationScript::SetSpeed(gocAnimation, 1);

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (gocMovement)
                game::GOCMovement::EnableMovementFlag(gocMovement, false);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
                game::GOCCollider::SetEnable(gocCollider, 1);

            GOCEnemyHsm::Dispatch(gocEnemyHsm, &message);

            Resume();
            Flags &= ~8;
        }

        void ProcMsgDlcZeldaNoticeStopEnemy(xgame::MsgDlcZeldaNoticeStopEnemy& message)
        {
            int* gocEnemyHsm = GameObject::GetGOC(this, GOCEnemyHsmString);
            if (!gocEnemyHsm)
                return;

            if (!GOCEnemyHsm::GetCurrentStateID(gocEnemyHsm))
            {
                Sleep(this);
                Flags |= 8;

                return;
            }

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
                fnd::GOCVisual::SetVisible(gocVisual, false);

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
                game::GOCShadow::SetVisible(gocShadow, false);

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
            if (gocAnimation)
                game::GOCAnimationScript::SetSpeed(gocAnimation, 0);

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (gocMovement)
                game::GOCMovement::DisableMovementFlag(gocMovement, false);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
                game::GOCCollider::SetEnable(gocCollider, 0);

            GOCEnemyHsm::Dispatch(gocEnemyHsm, &message);

            Sleep(this);
            Flags |= 8;
        }

        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            if (Flags & 8)
                return;

            csl::math::Vector3 vector{};

            if (ObjUtil::CheckShapeUserID(message.field_18, 1))
            {
                xgame::MsgDamage msgDamage{ 3, 8, 2, &message, &vector };
                SendMessageImm(message.ActorID, &msgDamage);

                int deviceTag[3]{};
                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (!gocSound)
                    return;

                game::GOCSound::Play3D(gocSound, deviceTag, "enm_pawn_punch_hit", 0);
            }
            else
            {
                if (!ObjUtil::CheckShapeUserID(message.field_18, 3))
                    SendTouchDamage((xgame::MsgDamage&)message);
            }
        }

        void ProcMsgKick(xgame::MsgKick& message)
        {
            xgame::MsgKick::SetReplyForSucceed(&message);
            ObjUtil::AddScore(this, "STALBABY", &message);
            Flags |= 2;
            deathPosition = message.field_40;

            int* gocEnemyHsm = GameObject::GetGOC(this, GOCEnemyHsmString);
            if (!gocEnemyHsm)
                return;

            GOCEnemyHsm::ChangeState(gocEnemyHsm, 7);
        }

        void ProcMsgNotifyObjectEvent(xgame::MsgNotifyObjectEvent& message)
        {
            if (message.field_18 == 1)
                Resume();
        }

        void ProcMsgPLGetHomingTargetInfo(xgame::MsgPLGetHomingTargetInfo& message)
        {
            if (Flags & 4)
                EnemyBase::ProcessMessage(message);
            else
                message.field_18 |= 1;
        }
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