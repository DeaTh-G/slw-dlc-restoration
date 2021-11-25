#pragma once

namespace app
{
    enum class ObjCoccoState : int
    {
        STATE_IDLE,
        STATE_ATTACK_IN,
        STATE_ATTACK_OUT,
        STATE_END
    };

    class ObjCoccoInfo : public CObjInfo
    {
    public:
        int Model;
        int Skeleton;
        animation::AnimationResContainer AnimationContainer{};

        ObjCoccoInfo()
        {
            animation::AnimationResContainer::__ct(&AnimationContainer, (csl::fnd::IAllocator*)pAllocator);
        }

        void Destructor(size_t deletingFlags) override
        {
            ObjCoccoInfo::~ObjCoccoInfo();
            animation::AnimationResContainer::__dt(&AnimationContainer);
            CObjInfo::Destructor(deletingFlags);
        }

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            int animationScript[3]{};
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&Model, "zdlc03_obj_cocco", &packFile);
            ObjUtil::GetSkeletonResource(&Skeleton, "zdlc03_obj_cocco", packFile);

            ObjUtil::GetAnimationScriptResource(&animationScript, "cocco", packFile);
            animationScript[1] = Skeleton;

            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&AnimationContainer, animationScript, packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjCoccoInfo";
        }
    };

    class ObjCocco : public CSetObjectListener
    {
    private:
        class CInfo
        {
            csl::math::Vector3 Position{};
            csl::math::Quaternion Rotation{};
            int field_20;
            int field_24;
        };

    public:
        ObjCoccoState State{};
        INSERT_PADDING(16); // TinyFSM
        int field_3B4{};
        int field_3B8{};
        int field_3BC{};
        csl::math::Vector3 Position{};
        csl::math::Quaternion Rotation{};
        int field_3E0{};
        int field_3E4{};
        int field_3E8{};
        int field_3EC{};
        char ActionType{};
        char field_3F1{};
        char field_3F2{};
        char field_3F3{};
        int field_3F4{};
        std::vector<ObjCocco*> SubCoccos{};
        float field_408{};
        float field_40C{};
        float field_410{};
        float Time{};
        float field_418{};
        int field_41C{};
        char Flags{};
        char field_421{};
        char field_422{};
        char field_423{};
        int field_424{};
        int field_428{};
        int field_42C{};

        ObjCocco() { ObjUtil::SetPropertyLockonTarget(this); }
        ObjCocco(const CInfo&) {};

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationScript);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCShadowSimple);
            fnd::GOComponent::Create(this, GOCMovementComplex);

            ObjCoccoInfo* info = (ObjCoccoInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjCoccoInfo");

            fnd::GOComponent::BeginSetup(this);

            GetParentObject(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.Animation |= 0x400000;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
                if (gocAnimation)
                {
                    animation::AnimationResContainer* animation = &(info->AnimationContainer);
                    game::GOCAnimationScript::Setup(gocAnimation, &animation);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    game::GOCAnimationScript::SetAnimation(gocAnimation, "IDLE");

                    csl::fnd::IAllocator* allocator{};
                    auto fucnPtr = &ObjCocco::SoundCallback;
                    animation::AnimCallbackBridge<ObjCocco>* callback =
                        (animation::AnimCallbackBridge<ObjCocco>*)AnimCallbackBridge_Initialize(allocator);
                    callback->GameObject = this;
                    callback->field_10 = reinterpret_cast<void*&>(fucnPtr);
                    callback->field_14 = -1;

                    game::GOCAnimationScript::RegisterCallback(gocAnimation, 0, callback);
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                csl::math::Vector3 position{ 0, 3, 0 };
                game::ColliSphereShapeCInfo collisionInfo{};
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 2;
                collisionInfo.field_04 = 1;
                if (!field_3E0)
                    collisionInfo.field_08 = 0x20000;

                collisionInfo.field_44 = 0;
                collisionInfo.field_48 = 0;
                collisionInfo.field_54 = 0;
                ObjUtil::SetupCollisionFilter(9, &collisionInfo);
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &position);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                csl::math::Vector3 position { 0, 3, 0 };
                game::ShadowHemisphereShapeCInfo shadowInfo;

                game::ShadowHemisphereShapeCInfo::__ct(&shadowInfo, 2.5f);
                shadowInfo.field_04 = 1;
                shadowInfo.ShadowQualityType = 2;

                game::ShadowHemisphereShapeCInfo* ppShadowInfo = &shadowInfo;
                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
                game::GOCShadowSimple::SetLocalOffsetPosition(gocShadow, &position);
            }

            /*int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (gocMovement)
            {
                csl::math::Vector3 position{};

                void* movementMem = ((app::fnd::ReferencedObject*)gocMovement)->pAllocator->Alloc
                (sizeof(MoveObjCocco), 16);
                MoveObjCocco* movement = new(movementMem) MoveObjCocco();
                game::GOCMovement::SetupController(gocMovement, movement);
            }*/

            game::GOCEffect::SimpleSetupEx(this, 1, 1);
            game::GOCSound::SimpleSetup(this, 0, 0);

            if (GetExtUserData(0) == 1)
            {
                if (gocVisual)
                    fnd::GOCVisual::SetVisible(gocVisual, 0);

                if (gocShadow)
                    game::GOCShadowSimple::SetVisible(gocShadow, 0);

                if (gocCollider)
                    game::GOCCollider::SetEnable(gocCollider, 0);

                Sleep(this);
            }

            SetEnableAttack(true);
            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;
            
            return CSetObjectListener::ProcessMessage(message);
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == ObjCoccoState::STATE_IDLE)
                StateIdle();

            if (State == ObjCoccoState::STATE_ATTACK_IN)
                StateAttackIn();

            if (State == ObjCoccoState::STATE_ATTACK_OUT)
                StateAttackOut();

            if (State == ObjCoccoState::STATE_END)
                StateEnd();

            if ((Flags & 2) != 2)
                return;

            if (ActionType)
                return;
            
            float oldTime;
            Time += updateInfo.deltaTime;
            if (Time > 7)
            {
                SetStatusRetire();
                Kill();
            }
            
            for (std::vector<ObjCocco*>::iterator it = SubCoccos.begin(); it != SubCoccos.end();)
            {
                while (*it)
                {
                    it++;
                    if (it == SubCoccos.end())
                        if (oldTime / 0.69999999f < Time / 0.69999999f)
                            CreateAttackers();
                }
                SubCoccos.erase(it);
            }

            if (oldTime / 0.69999999f < Time / 0.69999999f)
                CreateAttackers();
        }

    private:
        void SoundCallback(int a1, int a2, int a3)
        {
            int* gocSound = GameObject::GetGOC((GameObject*)((char*)this + 1), GOCSoundString);
            int deviceTag[3]{};
            if (!gocSound)
                return;

            if (!a2)
                if (!a3)
                    game::GOCSound::Play3D(gocSound, deviceTag, "obj_cock_flap", 0);
        }

        void NotifyStopCallback()
        {
            Flags |= 1;
        }

        inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new animation::AnimCallbackBridge<ObjCocco>();
        }

        void SetEnableAttack(bool isEnable)
        {
            if (isEnable)
                Flags |= 4;
            else
                Flags &= ~4;
        }

        CreateAttackers() {}

        void StateIdle() {}
        void StateAttackIn() {}
        void StateAttackOut() {}
        void StateEnd() {}
    };

    inline static ObjCocco* create_ObjCocco() { return new ObjCocco(); }

    inline static ObjCoccoInfo* createObjInfo_ObjCoccoInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjCoccoInfo();
    }
}