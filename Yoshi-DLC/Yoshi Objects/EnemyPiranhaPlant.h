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
            animation::AnimationResContainer::__ct(&AnimationContainer, (int)pAllocator);
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
    private:
        inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new animation::AnimCallbackBridge<EnemyPiranhaPlant>();
        }

    public:
        int AnimationListener;
        int field_514;
        int field_518;
        int field_51C;
        int field_520;
        int field_524;
        int field_528;
        int field_52C;
        int field_530;
        int field_534;
        int field_538;
        int field_53C;
        fnd::HFrame Parent;
        float field_670;
        float field_674;
        int field_678;
        int Direction;

        EnemyPiranhaPlant()
        {
            fnd::HFrame::__ct(&Parent);
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
            fnd::GOComponent::BeginSetup(this);
            
            EnemyPiranhaPlantData* data = (EnemyPiranhaPlantData*)CSetAdapter::GetData(*(int**)&(field_031C[0xC]));
            EnemyPiranhaPlantInfo* info = (EnemyPiranhaPlantInfo*)ObjUtil::GetObjectInfo(gameDocument, "EnemyPiranhaPlantInfo");
            Direction = !data->Direction;

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

                    game::GOCAnimationScript::Setup(gocAnimation, (int*)&animation);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);

                    if (Direction)
                        game::GOCAnimationScript::SetAnimation(gocAnimation, "IDLE_L");
                    else
                        game::GOCAnimationScript::SetAnimation(gocAnimation, "IDLE_R");

                    csl::fnd::IAllocator* allocator{};
                    auto funcPtr = &EnemyPiranhaPlant::SoundCallback;
                    animation::AnimCallbackBridge<EnemyPiranhaPlant>* callback = (animation::AnimCallbackBridge<EnemyPiranhaPlant>*)AnimCallbackBridge_Initialize(allocator);
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
                app::game::ColliSphereShapeCInfo collisionInfo;
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                // Search Collider
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.field_48 = 0;
                collisionInfo.field_44 = 0;
                collisionInfo.field_54 = 0;
                collisionInfo.Radius = data->SearchRadius;
                ObjUtil::SetupCollisionFilter(6, &collisionInfo);
                collisionInfo.field_0C = 0;
                collisionInfo.field_04 |= 3;
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

                /* Unknown Collision */
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.field_48 = 0;
                collisionInfo.field_44 = 0;
                collisionInfo.field_54 = 0;
                collisionInfo.Radius = 6;
                ObjUtil::SetupCollisionFilter(9, &collisionInfo);
                collisionInfo.field_0C = 2;
                collisionInfo.field_04 |= 1;
                collisionInfo.Parent = EnemyBase::GetCenterPositionFrame(this);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

                /* Unknown Collision */
                csl::math::Vector3 positionOffset { 7, 0, 0 };
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.field_48 = 0;
                collisionInfo.field_44 = 0;
                collisionInfo.field_54 = 0;
                collisionInfo.Radius = 3;
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

            int* gocEnemyHsm = GameObject::GetGOC((GameObject*)this, GOCEnemyHsmString);
            if (gocEnemyHsm)
            {
                GOCEnemyHsm::Description hsmDescription{ StateDescriptors, 5, 0 };
                GOCEnemyHsm::Setup(gocEnemyHsm, &hsmDescription);
            }

            game::GOCEffect::SimpleSetup((GameObject*)this);
            game::GOCSound::SimpleSetup((GameObject*)this, 0, 0);

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
            public:
                virtual ~Idle() {};
                virtual int Trigger(EnemyPiranhaPlant* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyPiranhaPlant* obj) { return 0; };
                virtual int Enter(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyPiranhaPlant* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int Step(EnemyPiranhaPlant* obj, float a2) { return 0;  };
            };

            class ShiftAttack
            {
                char field_00[20];
            public:
                virtual ~ShiftAttack() {};
                virtual int Trigger(EnemyPiranhaPlant* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyPiranhaPlant* obj) { return 0; };
                virtual int Enter(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyPiranhaPlant* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int Step(EnemyPiranhaPlant* obj, float a2) { return 0; };
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
                virtual int ProcessMessage(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int Step(EnemyPiranhaPlant* obj, float a2) { return 0; };
            };

            class ShiftIdle
            {
                char field_00[20];
            public:
                virtual ~ShiftIdle() {};
                virtual int Trigger(EnemyPiranhaPlant* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyPiranhaPlant* obj) { return 0; };
                virtual int Enter(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyPiranhaPlant* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int Step(EnemyPiranhaPlant* obj, float a2) { return 0; };
            };

            class Dead
            {
                char field_00[20];
            public:
                virtual ~Dead() {};
                virtual int Trigger(EnemyPiranhaPlant* obj, int a2, int* a3) { return ut::StateBase::Trigger(this, (int*)obj, a2, a3); };
                virtual int Init(EnemyPiranhaPlant* obj) { return 0; };
                virtual int Enter(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Enter(this, obj, a2); };
                virtual int Leave(EnemyPiranhaPlant* obj, int a2) { return EnemyState::Leave(this, obj, a2); };
                virtual int Update(EnemyPiranhaPlant* obj, float a2) { return EnemyState::Update(this, obj, a2); };
                virtual int ProcessMessage(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int OnEnter(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int OnLeave(EnemyPiranhaPlant* obj, int a2) { return 0; };
                virtual int Step(EnemyPiranhaPlant* obj, float a2) { return 0; };
            };
        };

        private:
            void ProcMsgDamage(xgame::MsgDamage& message)
            {
                int* gocEnemyHsm = GameObject::GetGOC(this, GOCEnemyHsmString);
                if (!gocEnemyHsm)
                    return;

                if (message.field_28 == 3)
                    GOCEnemyHsm::Dispatch(gocEnemyHsm, (fnd::MessageNew*)&message);
                else if (message.field_54 = 3)
                    if (ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], 0) &&
                        Transform.Data[0x24] == 7)
                        GOCEnemyHsm::Dispatch(gocEnemyHsm, (fnd::MessageNew*)&message);
            }

            void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
            {
                if (Direction & 8)
                    EnemyBase::SendTouchDamage(this, (xgame::MsgDamage&)message);
            }

            void SoundCallback(int a1, int a2, int a3)
            {
                int deviceTag[3]{};

                if (a2 || a3)
                    return;

                int* gocSound = GameObject::GetGOC((GameObject*)(this + 1), GOCSoundString);
                if (!gocSound)
                    return;

                game::GOCSound::Play3D(gocSound, deviceTag, "enm_pakkunflower_bite", 0);
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

            inline static app::GOCEnemyHsm::StateDesc StateDescriptors[] =
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

    inline static EnemyPiranhaPlantInfo* create_EnemyPiranhaPlantInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) EnemyPiranhaPlantInfo();
    }
}