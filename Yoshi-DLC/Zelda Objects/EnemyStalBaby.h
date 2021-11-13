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
        int field_600{};
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
        int field_62C{};

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

            //fnd::GOComponent::Create(this, GOCGravity);
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationScript);
            fnd::GOComponent::Create(this, GOCCollider);
            /*fnd::GOComponent::Create(this, GOCShadowSimple);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, EnemyTargetGOC);
            fnd::GOComponent::Create(this, GOCEnemyHSM);
            fnd::GOComponent::Create(this, GOCMovementComplex);*/

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
                collisionInfo.Parent = EnemyBase::GetCenterPositionFrame(this);
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
                collisionInfo.Parent = EnemyBase::GetCenterPositionFrame(this);
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

            fnd::GOComponent::EndSetup(this);
        }

    private:
        void AttackCallback(int a1, int a2, int a3)
        {
        }

        void SoundCallback(int a1, int a2, int a3)
        {
        }

        inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new animation::AnimCallbackBridge<EnemyStalBaby>();
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
