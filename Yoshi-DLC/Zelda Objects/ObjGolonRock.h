#pragma once

namespace app
{
    class ObjGolonRockGeneratorInfo : public CObjInfo
    {
    public:
        int Models[2]{};
        int Skeleton{};
        int Animation{};

        void Initialize(GameDocument& gameDocument) override
        {
            const char* modelNames[2]{ "zdlc03_obj_goron", "zdlc03_obj_goronrock" };

            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            for (size_t i = 0; i < 2; i++)
                ObjUtil::GetModelResource(&Models[i], modelNames[i], &packFile);

            ObjUtil::GetSkeletonResource(&Skeleton, "zdlc03_obj_goron", packFile);
            ObjUtil::GetAnimationResource(&Animation, "grn_appear", &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjGolonRockGeneratorInfo";
        }
    };

    class ObjGolonRock;

    namespace golon_rock
    {
        struct GolonRockCreateInfo
        {
            csl::math::Matrix34 TransformMatrix{};
            float NegativeSpeed{};
            bool IsCheckFall{};
            INSERT_PADDING(3);

            GolonRockCreateInfo() {}
            GolonRockCreateInfo(const csl::math::Matrix34& matrix, const float speed, const bool isCheckFall)
            {
                TransformMatrix = matrix;
                NegativeSpeed = speed;
                IsCheckFall = isCheckFall;
            }
        };

        ObjGolonRock* CreateGolonRock(GameDocument& gameDocument, const GolonRockCreateInfo& cInfo);
    }

    enum class ObjGolonRockState : int
    {
        STATE_NONE = -1,
        STATE_SHOOT,
        STATE_FALL,
        STATE_APPEAR,
        STATE_MOVE,
        STATE_DISAPPEAR
    };

    class ObjGolonRock : public GameObject3D
    {
    public:
        ObjGolonRockState State;
        INSERT_PADDING(16); // TinyFSM
        golon_rock::GolonRockCreateInfo* CInfo = new golon_rock::GolonRockCreateInfo();
        float Time{};
        float AppearTime{};
        int SoundHandle[3]{};
        Effect::CEffectHandle EffectHandle{};
        int* RockVisual{};
        game::MoveObjGolonRock* MovementController{};

        ObjGolonRock(GameDocument& gameDocument, const golon_rock::GolonRockCreateInfo& cInfo)
        {
            *CInfo = cInfo;
        }

        void Destructor(size_t deletingFlags) override
        {
            GameObject3D::Destructor(deletingFlags);

            delete CInfo;
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualContainer);
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            fnd::GOComponent::Create(this, GOCGravity);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCMovementComplex);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCShadowSimple);

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Vector3 downVector { 0, -1, 0 };
            csl::math::Quaternion rotation = GetRotationFromMatrix(&CInfo->TransformMatrix);
            csl::math::Vector3 position = *(csl::math::Vector3*)&CInfo->TransformMatrix.data[3];
            math::Vector3Rotate(&downVector, &rotation, &downVector);
            math::Vector3Scale(&downVector, 20, &downVector);
            math::Vector3Add(&position, &downVector, &position);
            fnd::GOCTransform::SetLocalTranslation((int*)gocTransform, &position);
            fnd::GOCTransform::SetLocalRotation((int*)gocTransform, &rotation);
            downVector = Vector3(0, -1, 0);

            ObjGolonRockGeneratorInfo* info = (ObjGolonRockGeneratorInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjGolonRockGeneratorInfo");

            fnd::GOComponent::BeginSetup(this);

            int* gocVContainer = GameObject::GetGOC(this, GOCVisual);
            if (gocVContainer)
            {
                int modelCount = 2;
                fnd::GOCVisualContainer::Setup(gocVContainer, &modelCount);
                
                int* gocVisual = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
                if (gocVisual)
                {
                    fnd::GOCVisualModel::VisualDescription visualDescriptor{};
                    fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                    visualDescriptor.Model = info->Models[0];
                    visualDescriptor.Skeleton = info->Skeleton;
                    visualDescriptor.Animation |= 0x400000;
                    fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                    fnd::GOCVisualContainer::Add(gocVContainer, gocVisual);
                    csl::math::Quaternion rotation{ 0, 1, 0, 0 };

                    fnd::GOCVisualTransformed::SetLocalRotation(gocVisual, &rotation);
                }

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
                if (gocAnimation)
                {
                    int animCount = 1;
                    game::GOCAnimationSimple::Setup(gocAnimation, &animCount);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    game::GOCAnimationSimple::Add(gocAnimation, "APPEARE", info->Animation, 1);
                }

                RockVisual = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
                if (RockVisual)
                {
                    fnd::GOCVisualModel::VisualDescription visualDescriptor{};
                    fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                    visualDescriptor.Model = info->Models[1];
                    visualDescriptor.Animation |= 0x400000;
                    fnd::GOCVisualModel::Setup(RockVisual, &visualDescriptor);
                    fnd::GOCVisualContainer::Add(gocVContainer, RockVisual);
                    fnd::GOCVisual::SetVisible(RockVisual, false);
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 2;
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                game::ColliSphereShapeCInfo collisionInfo{};
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 20;
                collisionInfo.field_04 |= 1;
                ObjUtil::SetupCollisionFilter(0, &collisionInfo);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

                collisionInfo = game::ColliSphereShapeCInfo();
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 20;
                collisionInfo.field_02 = 2;
                collisionInfo.field_04 |= 0x100;
                collisionInfo.field_08 = 3;
                collisionInfo.field_0C = 1;
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            game::GOCGravity::SimpleSetup(this, 1);

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (gocMovement)
            {
                csl::math::Vector3 position{};

                void* movementMem = ((app::fnd::ReferencedObject*)gocMovement)->pAllocator->
                    Alloc(sizeof(game::MoveObjGolonRock), 16);
                MovementController = new(movementMem) game::MoveObjGolonRock();
                game::GOCMovement::SetupController(gocMovement, MovementController);

                game::MoveObjGolonRock::SetupParam setupParam = game::MoveObjGolonRock::SetupParam(
                    (csl::math::Vector3*)gocTransform + 6,
                    CInfo->NegativeSpeed,
                    CInfo->IsCheckFall,
                    this
                );

                MovementController->Setup(setupParam);
            }

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                game::ShadowSphereShapeCInfo shadowInfo;
                game::ShadowSphereShapeCInfo* ppShadowInfo = &shadowInfo;
                
                game::ShadowSphereShapeCInfo::__ct(&shadowInfo, 20);
                shadowInfo.field_04 = 5;

                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
                
                math::Vector3Scale(&downVector, 20, &downVector);
                game::GOCShadowSimple::SetLocalOffsetPosition(gocShadow, &downVector);
                game::GOCShadowSimple::SetVisible(gocShadow, false);
            }

            game::GOCSound::SimpleSetup(this, 1, 1);
            game::GOCEffect::SimpleSetupEx(this, 1, 0);

            fnd::GOComponent::EndSetup(this);
            
            CInfo = NULL;
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            switch (message.Type)
            {
            /*case fnd::PROC_MSG_DLC_ZELDA_NOTICE_ACTIVE_ENEMY:
                ProcMsgDlcZeldaNoticeActiveEnemy((xgame::MsgDlcZeldaNoticeActiveEnemy&)message);
                return true;
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return true;
            case fnd::PROC_MSG_BREAK_ROLLING_STONE:
                ProcBreakRollingStone((xgame::MsgBreakRollingStone&)message);
                return true;*/
            default:
                return GameObject::ProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == ObjGolonRockState::STATE_SHOOT)
                StateShoot();

            if (State == ObjGolonRockState::STATE_APPEAR)
                StateAppear(updateInfo);

            if (State == ObjGolonRockState::STATE_MOVE)
                StateMove();

            /*if (State == ObjGolonRockState::STATE_FALL)
                StateFall();

            if (State == ObjGolonRockState::STATE_DISAPPEAR)
                StateDisappear();*/
        }

    private:
        void StateShoot()
        {
            MovementController->StartMode(0);

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            game::GOCAnimationSimple::SetAnimation(gocAnimation, "APPEARE");

            State = ObjGolonRockState::STATE_APPEAR;
        }

        void StateAppear(const fnd::SUpdateInfo& updateInfo)
        {
            AppearTime += updateInfo.deltaTime;
            if (AppearTime <= 0.7f)
                return;

            int* gocContainer = GameObject::GetGOC(this, GOCVisual) + 0x10;
            if (!gocContainer)
                return;

            int* goronModel = *(int**)(*gocContainer);
            fnd::GOCVisual::SetVisible(goronModel, false);
            fnd::GOCVisual::SetVisible(RockVisual, true);

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (!gocShadow)
                return;
            
            game::GOCShadowSimple::SetVisible(gocShadow, true);

            State = ObjGolonRockState::STATE_MOVE;
        }

        void StateMove()
        {
            RotateStoneModel();

            /*int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (gocSound)
                game::GOCSound::Play3D(gocSound, SoundHandle, "obj_goron_rolling", 0);
        
            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;

            game::EffectCreateInfo effectInfo;
            game::EffectCreateInfo::__ct(&effectInfo);
            effectInfo.Name = "ef_dl3_goron_roll";
            effectInfo.field_04 = 1;
            effectInfo.Position = Vector3(0, -22, 0);
            effectInfo.field_30 = 1;
            effectInfo.field_48 = -1;

            game::GOCEffect::CreateEffectLoopEx(gocEffect, &EffectHandle, &effectInfo);*/
            MovementController->SetFlag(1);
            MovementController->StartMode(1);
        }

        void CheckOnPhysics(const fnd::SUpdateInfo& updateInfo)
        {
            if (Time < 0)
                return;

            Time -= updateInfo.deltaTime;
            if (Time < 0)
            {
                int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
                if (!gocCollider)
                    return;

                ObjUtil::SetEnableColliShape(gocCollider, 1, 1);
            }
        }

        void RotateStoneModel()
        {
            csl::math::Quaternion rotation{};

            MovementController->GetLocalRotate(&rotation);
            fnd::GOCVisualTransformed::SetLocalRotation(RockVisual, &rotation);
        }
    };
}

inline static app::ObjGolonRock* app::golon_rock::CreateGolonRock(GameDocument& gameDocument, const GolonRockCreateInfo& cInfo)
{
    ObjGolonRock* object = new ObjGolonRock(gameDocument, cInfo);
    if (object)
        GameDocument::AddGameObject(*(GameDocument**)&gameDocument, object);

    return object;
}