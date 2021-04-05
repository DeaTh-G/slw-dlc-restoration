#pragma once

namespace app
{
    struct EnemyPiranhaPlantData
    {
        float SearchRadius;
        int Direction;
    };

    class EnemyPiranhaPlantInfo : public EnemyInfo
    {
    public:
        int Model{};
        int Skeleton{};
        animation::AnimationResContainer AnimationContainer{};
        int TexSrtAnimation{};

        EnemyPiranhaPlantInfo()
        {
            PacfileName = "EnemyPackunFlower.pac";
            animation::AnimationResContainer::__ct(&AnimationContainer, (csl::fnd::IAllocator*)pAllocator);
        }

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            int animationScript[3]{};

            sizeof(EnemyInfo);

            EnemyInfo::GetModelResource(this, &Model, "enm_packunflower");
            EnemyInfo::GetSkeletonResource(this, &Skeleton, "enm_packunflower");

            EnemyInfo::GetEnemyPackfile(this, &packFile);
            ObjUtil::GetAnimationScriptResource(animationScript, "piranhaplant", packFile);
            animationScript[1] = Skeleton;

            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&AnimationContainer, animationScript, packFile);

            EnemyInfo::GetTexSrtAnimationResource(this, &TexSrtAnimation, "enm_packunflower_head-0000");
        }

        const char* GetInfoName() override
        {
            return "EnemyPiranhaPlantInfo";
        }
    };

    class EnemyPiranhaPlant : public EnemyBase
    {
        class AnimListener : public animation::AnimationListener
        {
        public:
            EnemyPiranhaPlant* pPiranhaPlant;

            void OnEvent(int notifyTiming) override
            {
                pPiranhaPlant->AfterUpdatePoseCallback();
            }
        };

    public:
        AnimListener AnimationListener{};
        void* TexSrtControl{};
        fnd::HFrame Parent{};
        float HeadRotation{};
        float Scale{};
        xgame::MsgDamage* Message{};
        int Direction{};

        EnemyPiranhaPlant()
        {
            AnimationListener.field_20 = 2;
            fnd::HFrame::__ct(&Parent);
            Scale = 1;
            HeadRotation = -(3.1415927f * 0.5f);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            csl::math::Vector3 centerFrame { 0, 0, 0 };

            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationScript);
            fnd::GOComponent::Create(this, GOCShadowSimple);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, EnemyTargetGOC);
            fnd::GOComponent::Create(this, GOCEnemyHSM);
            
            EnemyPiranhaPlantData* data = (EnemyPiranhaPlantData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            EnemyPiranhaPlantInfo* info = (EnemyPiranhaPlantInfo*)ObjUtil::GetObjectInfo(gameDocument, "EnemyPiranhaPlantInfo");
            Direction |= !data->Direction;

            fnd::GOComponent::BeginSetup(this);

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            if (fnd::GOCTransform::IsExistParent(gocTransform))
            {
                fnd::GOCTransform::SetInheriteFlags(gocTransform, 1);
                centerFrame = Vector3(0, 2.5, 0);
                Direction |= 16;
            }

            EnemyBase::CreateCenterPositionFrame(this, &centerFrame);
            fnd::HFrame* centerPos = EnemyBase::GetCenterPositionFrame(this);
            fnd::HFrame::AddChild(centerPos, &Parent);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.Parent = EnemyBase::GetCenterPositionFrame(this);
                visualDescriptor.Animation |= 0x400000;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
                if (gocAnimation)
                {
                    animation::AnimationResContainer* animation = &(info->AnimationContainer);
                    int texSrtAnimation[2]{};
                    texSrtAnimation[0] = info->TexSrtAnimation;
                    texSrtAnimation[1] = 1;

                    game::GOCAnimationScript::Setup(gocAnimation, &animation);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    TexSrtControl = fnd::GOCVisualModel::SetTexSrtAnimation(gocVisual, texSrtAnimation);
                    fnd::TexSrtControlHH::SetSpeed(TexSrtControl, 0);

                    if (Direction)
                        game::GOCAnimationScript::SetAnimation(gocAnimation, "IDLE_L");
                    else
                        game::GOCAnimationScript::SetAnimation(gocAnimation, "IDLE_R");

                    AnimationListener.pPiranhaPlant = this;
                    game::GOCAnimationSimple::AddListener(gocAnimation, &AnimationListener);

                    csl::fnd::IAllocator* allocator{};
                    auto funcPtr = &EnemyPiranhaPlant::SoundCallback;
                    animation::AnimCallbackBridge<EnemyPiranhaPlant>* callback =
                        (animation::AnimCallbackBridge<EnemyPiranhaPlant>*)AnimCallbackBridge_Initialize(allocator);
                    callback->GameObject = this;
                    callback->field_10 = reinterpret_cast<void*&>(funcPtr);
                    callback->field_14 = -1;

                    game::GOCAnimationScript::RegisterCallback(gocAnimation, 1, callback);
                }
            }
            
            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                game::ShadowHemisphereShapeCInfo shadowInfo;
                game::ShadowHemisphereShapeCInfo* ppShadowInfo = &shadowInfo;

                game::ShadowHemisphereShapeCInfo::__ct(&shadowInfo, 4);
                shadowInfo.field_04 = 6;

                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
                if (Direction & 16)
                    game::GOCShadowSimple::SetVisible(gocShadow, false);
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 3;
                game::ColliSphereShapeCInfo collisionInfo{};
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                // Search Collider
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = data->SearchRadius;
                ObjUtil::SetupCollisionFilter(6, &collisionInfo);
                collisionInfo.field_0C = 0;
                collisionInfo.field_04 |= 3;
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

                /* Unknown Collision */
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 6;
                collisionInfo.field_44 = 0;
                collisionInfo.field_48 = 0;
                collisionInfo.field_54 = 0;
                ObjUtil::SetupCollisionFilter(9, &collisionInfo);
                collisionInfo.field_0C = 1;
                collisionInfo.field_04 |= 1;
                collisionInfo.Parent = EnemyBase::GetCenterPositionFrame(this);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

                /* Unknown Collision */
                csl::math::Vector3 positionOffset { 7, 0, 0 };
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 3;
                collisionInfo.field_44 = 0;
                collisionInfo.field_48 = 0;
                collisionInfo.field_54 = 0;
                ObjUtil::SetupCollisionFilter(9, &collisionInfo);
                collisionInfo.field_0C = 2;
                collisionInfo.field_04 |= 1;
                collisionInfo.Parent = &Parent;
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &positionOffset);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            int* gocEnemyTarget = GameObject::GetGOC(this, GOCEnemyTargetString);
            if (gocEnemyTarget)
            {
                int count = 0;
                csl::math::Vector3 eyeOffset { 0, 5, 0 };
                
                GOCEnemyTarget::Setup(gocEnemyTarget, &count);
                GOCEnemyTarget::SetColliderEyesight(gocEnemyTarget, 0);
                GOCEnemyTarget::SetEyesightOffset(gocEnemyTarget, &eyeOffset);
            }

            int* gocEnemyHsm = GameObject::GetGOC(this, GOCEnemyHsmString);
            if (gocEnemyHsm)
            {
                GOCEnemyHsm::Description hsmDescription{ StateDescriptors, 5, 0 };
                GOCEnemyHsm::Setup(gocEnemyHsm, &hsmDescription);
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

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
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return true;
            default:
                return EnemyBase::ProcessMessage(message);
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
                virtual int Trigger(EnemyPiranhaPlant* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyPiranhaPlant* obj) { return 0; };
                virtual int Enter(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyPiranhaPlant* obj, float a2) { return EnemyState::Update(this, obj, a2); };

                virtual bool ProcessMessage(EnemyPiranhaPlant* obj, fnd::Message& message)
                {
                    if (message.Type == fnd::PROC_MSG_DAMAGE)
                        return ProcMsgDamage(obj, (xgame::MsgDamage&)message);
                    else
                        return false;
                };

                virtual int OnEnter(EnemyPiranhaPlant* obj, int a2)
                {
                    GocEnemyTarget = GameObject::GetGOC(obj, GOCEnemyTargetString);
                    if (!GocEnemyTarget)
                        return 0;

                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (!gocAnimation)
                        return 0;

                    obj->SetEnableDamageCollision(false);
                    obj->SetScale(0.4f);
                    if (obj->Direction & 1)
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_L");
                    else
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_R");

                    return 1;
                };

                virtual int OnLeave(EnemyPiranhaPlant* obj, int a2) { return 0; };

                virtual int Step(EnemyPiranhaPlant* obj, float deltaTime)
                {
                    obj->HeadRotation = csl::math::Lerp(obj->HeadRotation,
                        csl::math::Select(obj->HeadRotation, abs(1.2217305f), -abs(1.2217305f)), deltaTime * 5);
                    UpdateHeadPosture(obj, 0);
                    
                    if (!GocEnemyTarget || !GOCEnemyTarget::IsFindTarget(GocEnemyTarget))
                        return 0;

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return 0;

                    GOCEnemyHsm::ChangeState(gocEnemyHsm, 1);

                    return 1;
                };

            private:
                bool ProcMsgDamage(EnemyPiranhaPlant* obj, xgame::MsgDamage& message)
                {
                    csl::math::Vector3 position{};
                    obj->Message = &message;

                    fnd::HFrame* centerPos = EnemyBase::GetCenterPositionFrame(obj);
                    math::CalculatedTransform::GetTranslation(&centerPos->Transform, &position);
                    xgame::MsgDamage::SetReply(&message, &position, 1);

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return false;

                    GOCEnemyHsm::ChangeState(gocEnemyHsm, 4);
                    return true;
                }
            };

            class ShiftAttack
            {
                char field_00[20];
                float Time;
            public:
                virtual ~ShiftAttack() {};
                virtual int Trigger(EnemyPiranhaPlant* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyPiranhaPlant* obj) { return 0; };
                virtual int Enter(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyPiranhaPlant* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyPiranhaPlant* obj, fnd::Message& message)
                {
                    if (message.Type == fnd::PROC_MSG_DAMAGE)
                        return ProcMsgDamage(obj, (xgame::MsgDamage&)message);
                    else
                        return false;
                };

                virtual int OnEnter(EnemyPiranhaPlant* obj, int a2)
                {
                    Time = 0;
                    obj->SetEnableDamageCollision(false);
                    return true;
                };

                virtual int OnLeave(EnemyPiranhaPlant* obj, int a2) { return 0; };

                virtual int Step(EnemyPiranhaPlant* obj, float deltaTime)
                {
                    UpdateHeadPosture(obj, false);
                    Time += deltaTime;
                    if (Time < 0.1f)
                    {
                        float multiplier = sinf(1.5707964f * (Time / 0.1f));
                        obj->SetScale((0.60000002f * multiplier) + 0.40000001f);
                    }
                    else
                    {
                        int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                        if (!gocEnemyHsm)
                            return 0;

                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 2);
                        return 1;
                    }

                    return 1;
                };

            private:
                bool ProcMsgDamage(EnemyPiranhaPlant* obj, xgame::MsgDamage& message)
                {
                    csl::math::Vector3 position{};
                    obj->Message = &message;

                    fnd::HFrame* centerPos = EnemyBase::GetCenterPositionFrame(obj);
                    math::CalculatedTransform::GetTranslation(&centerPos->Transform, &position);
                    xgame::MsgDamage::SetReply(&message, &position, 1);

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return false;

                    GOCEnemyHsm::ChangeState(gocEnemyHsm, 4);
                    return true;
                }
            };
            
            class Attack
            {
                char field_00[20];
            public:
                virtual ~Attack() {};
                virtual int Trigger(EnemyPiranhaPlant* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyPiranhaPlant* obj) { return 0; };
                virtual int Enter(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyPiranhaPlant* obj, float a2) { return EnemyState::Update(this, obj, a2); };

                virtual bool ProcessMessage(EnemyPiranhaPlant* obj, fnd::Message& message)
                {
                    if (message.Type == fnd::PROC_MSG_DAMAGE)
                        return ProcMsgDamage(obj, (xgame::MsgDamage&)message);
                    else
                        return false;
                };

                virtual int OnEnter(EnemyPiranhaPlant* obj, int a2)
                {
                    int* gocEnemyTarget = GameObject::GetGOC(obj, GOCEnemyTargetString);
                    if (!gocEnemyTarget)
                        return 0;

                    obj->SetEnableDamageCollision(true);
                    obj->SetScale(1);

                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (!gocAnimation)
                        return 0;

                    if (obj->Direction & 1)
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "ATTACK_L");
                    else
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "ATTACK_R");

                    return true;
                };

                virtual int OnLeave(EnemyPiranhaPlant* obj, int a2) { return 0; };
                
                virtual int Step(EnemyPiranhaPlant* obj, float deltaTime)
                {
                    csl::math::Vector3 playerPos{};
                    csl::math::Vector3 objectPos{};

                    int* gocEnemyTarget = GameObject::GetGOC(obj, GOCEnemyTargetString);
                    if (!gocEnemyTarget)
                        return 0;

                    if (!GOCEnemyTarget::IsFindTarget(gocEnemyTarget))
                    {
                        int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                        if (!gocEnemyHsm)
                            return 0;

                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 3);
                        return 1;
                    }

                    GOCEnemyTarget::GetTargetCenterPosition(gocEnemyTarget, &playerPos);

                    fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(obj, GOCTransformString);
                    if (!gocTransform)
                        return 0;

                    fnd::HFrame* center = EnemyBase::GetCenterPositionFrame(obj);
                    math::CalculatedTransform::GetTranslation(&center->Transform, &objectPos);
                    csl::math::Vector3 rotX = *(csl::math::Vector3*)&center->Transform.data[1][0];
                    csl::math::Vector3 rotY = *(csl::math::Vector3*)&center->Transform.data[2][0];

                    math::Vector3Subtract(&playerPos, &objectPos, &playerPos);
                    csl::math::Vector3NormalizeZero(&playerPos, &playerPos);
                    float xDot = math::Vector3DotProduct(&rotX, &playerPos);
                    xDot = csl::math::Clamp(xDot, -1, 1);
                    float xMin = csl::math::Min(acosf(xDot), 1.2217305f);
                    float yDot = math::Vector3DotProduct(&rotY, &playerPos);
                    obj->HeadRotation = csl::math::Lerp(obj->HeadRotation, csl::math::Select(yDot, abs(xMin), -abs(xMin)), (deltaTime * 5));

                    UpdateHeadPosture(obj, true);
                    
                    return 1;
                };

            private:
                bool ProcMsgDamage(EnemyPiranhaPlant* obj, xgame::MsgDamage& message)
                {
                    csl::math::Vector3 position{};
                    obj->Message = &message;

                    fnd::HFrame* centerPos = EnemyBase::GetCenterPositionFrame(obj);
                    math::CalculatedTransform::GetTranslation(&centerPos->Transform, &position);
                    xgame::MsgDamage::SetReply(&message, &position, 1);

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return false;

                    GOCEnemyHsm::ChangeState(gocEnemyHsm, 4);
                    return true;
                }
            };

            class ShiftIdle
            {
                char field_00[20];
                float Time;
            public:
                virtual ~ShiftIdle() {};
                virtual int Trigger(EnemyPiranhaPlant* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyPiranhaPlant* obj) { return 0; };
                virtual int Enter(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyPiranhaPlant* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual bool ProcessMessage(EnemyPiranhaPlant* obj, fnd::Message& message)
                {
                    if (message.Type == fnd::PROC_MSG_DAMAGE)
                        return ProcMsgDamage(obj, (xgame::MsgDamage&)message);
                    else
                        return false;
                };

                virtual int OnEnter(EnemyPiranhaPlant* obj, int a2)
                {
                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (!gocAnimation)
                        return 0;

                    Time = 0;

                    obj->SetEnableDamageCollision(false);
                    if (obj->Direction & 1)
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_L");
                    else
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE_R");

                    return 1;
                };
                virtual int OnLeave(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int Step(EnemyPiranhaPlant* obj, float deltaTime)
                {
                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return 0;

                    obj->HeadRotation = csl::math::Lerp(obj->HeadRotation,
                        csl::math::Select(obj->HeadRotation, fabs(1.2217305f), -fabs(1.2217305f)), (deltaTime * 5));
                    UpdateHeadPosture(obj, false);

                    Time += deltaTime;
                    if (Time < 0.1f)
                        obj->SetScale(-(0.6f * sinf(1.5707964f * (Time / 0.1f)) - 1));
                    else
                        GOCEnemyHsm::ChangeState(gocEnemyHsm, 0);

                    return 1;
                };

            private:
                bool ProcMsgDamage(EnemyPiranhaPlant* obj, xgame::MsgDamage& message)
                {
                    csl::math::Vector3 position{};
                    obj->Message = &message;

                    fnd::HFrame* centerPos = EnemyBase::GetCenterPositionFrame(obj);
                    math::CalculatedTransform::GetTranslation(&centerPos->Transform, &position);
                    xgame::MsgDamage::SetReply(&message, &position, 1);

                    int* gocEnemyHsm = GameObject::GetGOC(obj, GOCEnemyHsmString);
                    if (!gocEnemyHsm)
                        return false;

                    GOCEnemyHsm::ChangeState(gocEnemyHsm, 4);
                    return true;
                }
            };

            class Dead
            {
                char field_00[20];
                int State;
                float Time;
                float AbsoluteRotation;
                float Scale;
            public:
                virtual ~Dead() {};
                virtual int Trigger(EnemyPiranhaPlant* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyPiranhaPlant* obj) { return 0; };
                virtual int Enter(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyPiranhaPlant* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyPiranhaPlant* obj, int a2)
                {
                    AbsoluteRotation = fabs(obj->HeadRotation);
                    Scale = obj->Scale;

                    obj->SetEnableCollision(false);
                    ChangeSubState(0);                    
                    
                    int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                    if (!gocAnimation)
                        return 0;

                    if (obj->Direction & 1)
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "DAMAGE_L");
                    else
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "DAMAGE_R");

                    obj->Direction |= 4;

                    return 1;
                };
                virtual int OnLeave(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int Step(EnemyPiranhaPlant* obj, float deltaTime)
                {
                    Time += deltaTime;

                    if (!State)
                    {
                        Time = csl::math::Clamp(Time / 0.2f, 0, 1);
                        obj->HeadRotation = (-0.34906584f - AbsoluteRotation) *
                            csl::math::Clamp(sinf(Time * 1.5707964f), 0, 1) + AbsoluteRotation;

                        if (Time >= 1)
                        {
                            ChangeSubState(1);
                            return 1;
                        }
                    }
                    else if (State == 1)
                    {
                        int deviceTag[3]{};

                        if (Time < 0.4f)
                            return 1;

                        int* gocVisual = GameObject::GetGOC(obj, GOCVisual);
                        if (!gocVisual)
                            return 0;

                        int* gocSound = GameObject::GetGOC(obj, GOCSoundString);
                        if (!gocSound)
                            return 0;

                        fnd::TexSrtControlHH::SetSpeed(obj->TexSrtControl, 1);

                        game::GOCSound::Play3D(gocSound, deviceTag, "enm_pakkunflower_down", 0);
                        ChangeSubState(2);
                        return 1;
                    }
                    else if (State == 2)
                    {
                        float scalar = csl::math::Clamp(Time / 0.6f, 0, 1);
                        obj->SetScale(((0.2f - Scale) * scalar) + Scale);
                        if (scalar >= 1)
                        {
                            ChangeSubState(3);
                            return 1;
                        }
                    }
                    else if (State == 3)
                    {
                        enemy::DeadEffectCInfo effectInfo{};
                        math::Transform transform{};

                        fnd::HFrame* center = EnemyBase::GetCenterPositionFrame(obj);
                        csl::math::Vector3 position = *(csl::math::Vector3*)&center->Transform.data[3][0];
                        ObjUtil::AddScore(obj, "PIRANHAPLANT", obj->Message);
                        
                        enemy::DeadEffectCInfo::__ct(&effectInfo);
                        enemy::DeadEffectCInfo::SetMsgDamage(&effectInfo, obj->Message);
                        enemy::DeadEffectCInfo::SetYoshiIsland(&effectInfo);

                        int* gocVisual = GameObject::GetGOC(obj, GOCVisual);
                        if (!gocVisual)
                            return 0;

                        fnd::GOCVisualModel::GetNodeTransform(gocVisual, 1, "Mouth", &transform);
                        math::Vector3Scale(&transform.Position, 0.2f, &transform.Position);
                        math::Vector3Add(&transform.Position, (csl::math::Vector3*)&center->Transform.data[3][0], &transform.Position);
                        transform.SetFlag(1);

                        effectInfo.field_20.data[3][0] = transform.Position.X;
                        effectInfo.field_20.data[3][1] = transform.Position.Y;
                        effectInfo.field_20.data[3][2] = transform.Position.Z;
                        effectInfo.field_62 |= 1;

                        void* enemyManager = EnemyManager::GetService((GameDocument*)obj->field_24[1]);
                        EnemyManager::CreateDeadEffect(enemyManager, &effectInfo);
                        EnemyBase::ProcMission(obj, obj->Message);
                        CSetObjectListener::SetStatusRetire(obj);
                        GameObject::Kill(obj);
                        
                        return 1;
                    }
                };

            private:
                void ChangeSubState(int stateID)
                {
                    Time = 0;
                    State = stateID;
                }
            };

        private:
            static void UpdateHeadPosture(EnemyPiranhaPlant* obj, bool changeAnimation)
            {
                if (obj->HeadRotation >= 0)
                {
                    if (obj->Direction & 2)
                    {
                        obj->Direction ^= 2;
                        if (changeAnimation)
                        {
                            int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                            if (!gocAnimation)
                                return;

                            if (obj->Direction & 1)
                                game::GOCAnimationScript::ChangeAnimation(gocAnimation, "ATTACK_L");
                            else
                                game::GOCAnimationScript::ChangeAnimation(gocAnimation, "ATTACK_R");
                        }
                    }

                    csl::math::Quaternion rotation { 0, 0, 0, 1 };
                    fnd::HFrame* center = EnemyBase::GetCenterPositionFrame(obj);
                    fnd::HFrame::SetLocalRotation(center, &rotation);
                }
                else
                {
                    if (!(obj->Direction & 2))
                    {
                        obj->Direction ^= 2;
                        if (changeAnimation)
                        {
                            int* gocAnimation = GameObject::GetGOC(obj, GOCAnimationString);
                            if (!gocAnimation)
                                return;

                            if (obj->Direction & 1)
                                game::GOCAnimationScript::ChangeAnimation(gocAnimation, "ATTACK_L");
                            else
                                game::GOCAnimationScript::ChangeAnimation(gocAnimation, "ATTACK_R");
                        }
                    }
                }
            }
        };
     private:
         void ProcMsgDamage(xgame::MsgDamage& message)
         {
             int* gocEnemyHsm = GameObject::GetGOC(this, GOCEnemyHsmString);
             if (!gocEnemyHsm)
                 return;

             if (message.field_28 == 3)
                 GOCEnemyHsm::Dispatch(gocEnemyHsm, (fnd::Message*) & message);
             else if (message.field_54 == 3)
                 if (ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], 0) &&
                     Transform.Data[0x24] == 7)
                     GOCEnemyHsm::Dispatch(gocEnemyHsm, (fnd::Message*) & message);
         }

         void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
         {
             if (Direction & 8 && !ObjUtil::CheckShapeUserID(message.field_18, 0))
             {
                 csl::math::Vector3 position{};
                 xgame::MsgDamage msgDamage{ 3, 8, 3, &message, &position };
                 SendMessageImm(message.ActorID, &msgDamage);
             }
         }

         void SetEnableCollision(int isEnable)
         {
             int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
             if (!gocCollider)
                 return;

             ObjUtil::SetEnableColliShape(gocCollider, 2, isEnable);
             ObjUtil::SetEnableColliShape(gocCollider, 1, isEnable);
         }

         void SetEnableDamageCollision(bool isEnable)
         {
             int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
             if (!gocCollider)
                 return;

             if (isEnable)
                 Direction |= 8;
             else
                 Direction &= ~8;

             ObjUtil::SetEnableColliShape(gocCollider, 2, isEnable);
         }

         void SetScale(float scale)
         {
             int* gocVisual = GameObject::GetGOC(this, GOCVisual);
             if (!gocVisual)
                 return;

             Scale = scale;

             csl::math::Vector3 visualScale{ scale, scale, scale };
             fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &visualScale);

             int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
             if (!gocShadow)
                 return;

             game::GOCShadowSimple::SetScale(gocShadow, &visualScale);
         }

         void SoundCallback(int a1, int a2, int a3)
         {
             int deviceTag[3]{};

             if (a2 || a3)
                 return;

             int* gocSound = GameObject::GetGOC((GameObject*)((char*)this + 1), GOCSoundString);
             if (!gocSound)
                 return;

             game::GOCSound::Play3D(gocSound, deviceTag, "enm_pakkunflower_bite", 0);
         }

         void AfterUpdatePoseCallback()
         {
             int* gocVisual = GameObject::GetGOC(this, GOCVisual);
             if (!gocVisual)
                 return;

             float multiplier = HeadRotation * 0.25f;
             if (!(Direction & 4))
                 fabs(multiplier);

             for (size_t i = 2; i <= 5; i++)
             {
                 math::Transform transform{};
                 char buffer[8]{};

                 snprintf(buffer, 8, "Body%d", i);
                 fnd::GOCVisualModel::GetNodeTransform(gocVisual, 2, buffer, &transform);

                 Eigen::Quaternion<float> q;
                 q = Eigen::AngleAxis<float>(multiplier, Eigen::Vector3f(0, 1, 0));
                 csl::math::Quaternion rotation{ q.x(), q.y(), q.z(), q.w() };
                 transform.Rotation = rotation;

                 fnd::GOCVisualModel::SetNodeTransform(gocVisual, 2, buffer, &transform);
             }

             math::Transform transform{};
             fnd::GOCVisualModel::GetNodeTransform(gocVisual, 1, "Head", &transform);
             csl::math::Matrix34 transformMatrix{};
             math::Transform::GetTransformMatrix(&transform, &transformMatrix);
             fnd::HFrame::SetLocalTransform(&Parent, &transformMatrix);
         }

         inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
         {
             return new animation::AnimCallbackBridge<EnemyPiranhaPlant>();
         }

         inline static void* Idle_Initialize(csl::fnd::IAllocator* pAllocator)
         {
             return new EnemyPiranhaPlant::State::Idle();
         }

         inline static void* Attack_Initialize(csl::fnd::IAllocator* pAllocator)
         {
             return new EnemyPiranhaPlant::State::Attack();
         }

         inline static void* ShiftAttack_Initialize(csl::fnd::IAllocator* pAllocator)
         {
             return new EnemyPiranhaPlant::State::ShiftAttack();
         }

         inline static void* ShiftIdle_Initialize(csl::fnd::IAllocator* pAllocator)
         {
             return new EnemyPiranhaPlant::State::ShiftIdle();
         }

         inline static void* Dead_Initialize(csl::fnd::IAllocator* pAllocator)
         {
             return new EnemyPiranhaPlant::State::Dead();
         }

         inline static ut::internal::StateDescImpl States[] =
         {
             { "Idle", &Idle_Initialize, -1 },
             { "ShiftAttack", &ShiftAttack_Initialize, -1 },
             { "Attack", &Attack_Initialize, -1 },
             { "ShiftIdle", &ShiftIdle_Initialize, -1 },
             { "Dead", &Dead_Initialize, -1 }
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

    inline static EnemyPiranhaPlant* create_EnemyPiranhaPlant()
    {
        return new EnemyPiranhaPlant();
    }

    inline static EnemyPiranhaPlantInfo* createObjInfo_EnemyPiranhaPlantInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) EnemyPiranhaPlantInfo();
    }
}