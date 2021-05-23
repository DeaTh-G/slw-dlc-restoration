#pragma once

namespace app
{
    enum class ObjTreasureBoxState : int
    {
        STATE_INITIALIZE,
        STATE_WAIT,
        STATE_HITOFF,
        STATE_OPEN_CONTROL_CAMERA,
        STATE_OPEN_WAIT_ANIM,
        STATE_OPEN_END,
        STATE_OPENED
    };

    enum class ObjTreasureBoxType : uint8_t
    {
        HEART_CONTAINER,
        RUPEE_PURPLE,
        RUPEE_GOLD,
        RUPEE_GREEN
    };

    struct ObjTreasureBoxData
    {
        ObjTreasureBoxType ItemType;
    };

    class ObjTreasureBoxInfo : public CObjInfo
    {
    public:
        int Model{};
        int Skeleton{};
        int Animation{};
        int HeartModel{};

        ObjTreasureBoxInfo() {}

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&Model, "zdlc03_obj_treasurebox", &packFile);
            ObjUtil::GetSkeletonResource(&Skeleton, "zdlc03_obj_treasurebox", packFile);
            ObjUtil::GetAnimationResource(&Animation, "treasurebox_open", &packFile);
            ObjUtil::GetModelResource(&HeartModel, "zdlc03_obj_heartcase", &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjTreasureBoxInfo";
        }

        void RegistCallback(int& container) override
        {
            ObjZeldaPopupItemInfo* popupObject = new(pAllocator) ObjZeldaPopupItemInfo();
            if (popupObject)
                CObjInfoContainer::Register(&container, popupObject->GetInfoName(), popupObject);
        }
    };

    class ObjTreasureBox : public CSetObjectListener
    {
    public:
        class Listener : public animation::AnimationListener
        {
        public:
            ObjTreasureBox* pTreasureBox;

            void OnEvent(int notifyTiming) override
            {
                if (!pTreasureBox)
                    return;


            }
        };

        INSERT_PADDING(20);
        Listener AnimationListener{};
        void* pTreasureBoxCamera{};
        INSERT_PADDING(12);
        csl::math::Vector3 field_3F0{};
        csl::math::Vector3 field_400{};
        csl::math::Matrix34 TransformMatrix{};
        int field_450{};
        ObjTreasureBoxType ItemType{};
        INSERT_PADDING(7);
        game::HudLayerController* LayerController;

        ObjTreasureBox()
        {
            sizeof(ObjTreasureBox);
            AnimationListener.field_20 = 2;
        }

        void Destructor(size_t deletingFlags)
        {
            AnimationListener.Destructor(0);

            CSetObjectListener::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualContainer);
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCHud);

            ObjTreasureBoxInfo* treasureInfo = (ObjTreasureBoxInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjTreasureBoxInfo");
            ObjZeldaPopupItemInfo* popupInfo = (ObjZeldaPopupItemInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjZeldaPopupItemInfo");
            ObjTreasureBoxData* data = (ObjTreasureBoxData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            ItemType = data->ItemType;

            fnd::GOComponent::BeginSetup(this);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
                TransformMatrix = *(csl::math::Matrix34*)(gocTransform + 0x44);

            int* gocVContainer = GameObject::GetGOC(this, GOCVisual);
            if (gocVContainer)
            {
                int modelCount = 2;
                fnd::GOCVisualContainer::Setup(gocVContainer, &modelCount);

                int* chestModel = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
                if (chestModel)
                {
                    csl::math::Vector3 scale { 0.9f, 0.9f, 0.9f };

                    fnd::GOCVisualModel::VisualDescription visualDescriptor{};
                    fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                    visualDescriptor.Model = treasureInfo->Model;
                    visualDescriptor.Skeleton = treasureInfo->Skeleton;
                    fnd::GOCVisualModel::Setup(chestModel, &visualDescriptor);
                    fnd::GOCVisualContainer::Add(gocVContainer, chestModel);
                    fnd::GOCVisualTransformed::SetLocalScale(chestModel, &scale);
                }

                int* treasureModel = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
                if (treasureModel)
                {
                    csl::math::Vector3 scale { 0.7f, 0.7f, 0.7f };
                    csl::math::Vector3 offset { 0, 11.2f, 13 };

                    fnd::GOCVisualModel::VisualDescription visualDescriptor{};
                    fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                    switch (ItemType)
                    {
                    case app::ObjTreasureBoxType::HEART_CONTAINER:
                        visualDescriptor.Model = treasureInfo->HeartModel;
                        break;
                    case app::ObjTreasureBoxType::RUPEE_PURPLE:
                        visualDescriptor.Model = popupInfo->Models[3];
                        break;
                    case app::ObjTreasureBoxType::RUPEE_GOLD:
                        visualDescriptor.Model = popupInfo->Models[4];
                        break;
                    case app::ObjTreasureBoxType::RUPEE_GREEN:
                    default:
                        visualDescriptor.Model = popupInfo->Models[0];
                        break;
                    }

                    fnd::GOCVisualModel::Setup(treasureModel, &visualDescriptor);
                    fnd::GOCVisualContainer::Add(gocVContainer, treasureModel);
                    fnd::GOCVisualTransformed::SetLocalScale(treasureModel, &scale);
                    fnd::GOCVisualTransformed::SetLocalTranslation(treasureModel, &offset);
                    fnd::GOCVisual::SetVisible(treasureModel, false);
                }

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
                if (gocAnimation)
                {
                    int animCount = 1;
                    game::GOCAnimationSimple::Setup(gocAnimation, &animCount);
                    fnd::GOCVisualModel::AttachAnimation(chestModel, gocAnimation);
                    game::GOCAnimationSimple::Add(gocAnimation, "OPEN", treasureInfo->Animation, 1);
                }

                int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
                if (gocCollider)
                {
                    int shapeCount = 2;
                    game::ColliBoxShapeCInfo collisionInfo;

                    game::GOCCollider::Setup(gocCollider, &shapeCount);

                    csl::math::Vector3 triggerOffset{ 0, 9, 0 };
                    game::CollisionObjCInfo::__ct(&collisionInfo);
                    collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                    collisionInfo.MotionType = 0;
                    collisionInfo.Size = Vector3(9, 9, 9);
                    collisionInfo.field_04 |= 1;
                    ObjUtil::SetupCollisionFilter(0, &collisionInfo);
                    game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &triggerOffset);
                    game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

                    csl::math::Vector3 colliderOffset { 0, 7.74f, 0 };
                    game::CollisionObjCInfo::__ct(&collisionInfo);
                    collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                    collisionInfo.MotionType = 0;
                    collisionInfo.Size = Vector3(10.8f, 7.74f, 6.975f);
                    collisionInfo.field_02 = 2;
                    collisionInfo.field_04 |= 0x100;
                    collisionInfo.field_08 = 3;
                    collisionInfo.field_0C = 1;
                    game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &colliderOffset);
                    game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
                }
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

            if (DisableChestLetterboxing)
            {
                fnd::GOComponent::EndSetup(this);
                return;
            }
            
            game::GOCHud* gocHud = (game::GOCHud*)GameObject::GetGOC(this, GOCHudString);
            if (gocHud)
            {
                int packFile{};
                int refCount{};

                game::GOCHud::Description hudDescription{};
                hudDescription.field_00 = "HudWipeTreasureBox";
                hudDescription.field_04 = 8;
                hudDescription.field_08 = 8;
                hudDescription.field_0C = -21;
                hudDescription.field_10 = 1;
                hudDescription.field_14 = 2048;
                gocHud->Setup(&hudDescription);

                ObjUtil::GetPackFile(&packFile, "ui_zdlc03_gamemodestage.pac");
                gocHud->SetupProject(&refCount, "ui_wipe_zdlc03", packFile);
                if (refCount)
                {
                    LayerController = gocHud->CreateLayerController(refCount, "ui_wipe_zdlc03", "wipe_mask", 0);
                    LayerController->ReserveAnimation("Outro_Anim", 0, 0);
                    LayerController->PlayReservedAnimation();
                }
            }

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            if (true)
                return CSetObjectListener::ProcessMessage(message);

            return 1;
        }

    private:
    };

    inline static ObjTreasureBox* create_ObjTreasureBox() { return new ObjTreasureBox(); }

    inline static ObjTreasureBoxInfo* createObjInfo_ObjTreasureBoxInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjTreasureBoxInfo();
    }
}