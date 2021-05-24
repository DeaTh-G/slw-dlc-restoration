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

        INSERT_PADDING(8);
        xgame::MsgHitEventCollision* HitMessage = new xgame::MsgHitEventCollision();
        xgame::MsgGetExternalMovePosition* ExternalMoveMessage = new xgame::MsgGetExternalMovePosition();
        ObjTreasureBoxState State{};
        Listener AnimationListener{};
        void* pTreasureBoxCamera{};
        INSERT_PADDING(12);
        csl::math::Vector3 CameraPosition{};
        csl::math::Vector3 CameraTargetPosition{};
        csl::math::Matrix34 TransformMatrix{};
        int PlayerNumber{};
        ObjTreasureBoxType ItemType{};
        INSERT_PADDING(3);
        float Time{};
        game::HudLayerController* LayerController;

        ObjTreasureBox()
        {
            AnimationListener.field_20 = 2;
        }

        void Destructor(size_t deletingFlags)
        {
            delete HitMessage;
            delete ExternalMoveMessage;
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
                    game::GOCAnimationSimple::Add(gocAnimation, "OPEN", treasureInfo->Animation, 0);
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
                if (!packFile)
                {
                    fnd::GOComponent::EndSetup(this);
                    return;
                }

                gocHud->SetupProject(&refCount, "ui_wipe_zdlc03", packFile);
                if (refCount)
                {
                    LayerController = gocHud->CreateLayerController(refCount, "ui_wipe_zdlc03", "wipe_mask", 0);
                    
                    /* TODO: Do this but better */
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

            switch (message.Type)
            {
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return true;
            case fnd::PROC_MSG_GET_EXTERNAL_MOVE_POSITION:
                ProcMsgGetExternalMovePosition((xgame::MsgGetExternalMovePosition&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == ObjTreasureBoxState::STATE_INITIALIZE)
                StateInitialize();

            if (State == ObjTreasureBoxState::STATE_WAIT)
                StateWait();

            if (State == ObjTreasureBoxState::STATE_HITOFF)
                StateHitoff();

            if (State == ObjTreasureBoxState::STATE_OPEN_CONTROL_CAMERA)
                StateOpenControlCamera();

            if (State == ObjTreasureBoxState::STATE_OPEN_WAIT_ANIM)
                StateOpenWaitAnim(updateInfo);

            if (State == ObjTreasureBoxState::STATE_OPEN_END)
                StateOpenEnd();

            if (State == ObjTreasureBoxState::STATE_OPENED)
                StateOpened(updateInfo);
        }

    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            *HitMessage = message;
        }

        void ProcMsgGetExternalMovePosition(xgame::MsgGetExternalMovePosition& message)
        {
            *ExternalMoveMessage = message;
            *ExternalMoveMessage->Transform = TransformMatrix;
        }

        void StateInitialize()
        {
            State = ObjTreasureBoxState::STATE_WAIT;
        }

        void StateWait()
        {
            if (!HitMessage->ActorID)
                return;

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], HitMessage->ActorID);
            PlayerNumber = playerNo;

            xgame::MsgCatchPlayer catchMessage { 32, TransformMatrix, 19 };
            if (!ObjUtil::SendMessageImmToPlayer(this, playerNo, &catchMessage) || !catchMessage.field_64)
                return;

            xgame::MsgStopGameTimer stopTimerMessage{};
            ObjUtil::SendMessageImmToGameActor(this, &stopTimerMessage);

            xgame::MsgDlcZeldaNoticeStopEnemy stopEnemyMessage{};
            ObjUtil::SendMessageImmToGameActor(this, &stopEnemyMessage);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (!gocCollider)
                return;
            
            game::GOCCollider::SetEnable(gocCollider, false);

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            game::GOCAnimationSimple::SetAnimation(gocAnimation, "OPEN");

            if (LayerController)
            {
                LayerController->ReserveAnimation("Intro_Anim", 0, 0);
                LayerController->ReserveAnimation("Usual_Anim", 1, 0);
                LayerController->PlayReservedAnimation();
            }

            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;

            app::game::GOCEffect::CreateEffect(gocEffect, "ef_dl3_treasurebox_open");

            int deviceTag[3];
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play(gocSound, deviceTag, "obj_zeldatreasure_open", 0);

            xgame::MsgChangeBGMVolume changeBGMVolumeMessage { 0, 0.2f };
            ObjUtil::SendMessageImmToGameActor(this, &changeBGMVolumeMessage);

            State = ObjTreasureBoxState::STATE_OPEN_CONTROL_CAMERA;
        }

        void StateHitoff()
        {

        }

        void StateOpenControlCamera()
        {
            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            if (game::GOCAnimationSimple::GetFrame(gocAnimation) < 460)
                return;

            int* gocContainer = GameObject::GetGOC(this, GOCVisual) + 0x10;
            if (!gocContainer)
                return;

            int* treasureModel = *(int**)(*gocContainer + 4);
            fnd::GOCVisual::SetVisible(treasureModel, true);

            int deviceTag[3];
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play(gocSound, deviceTag, "obj_zeldaitem_get", 0);

            // CreateEffectEx

            State = ObjTreasureBoxState::STATE_OPEN_WAIT_ANIM;
        }

        void StateOpenWaitAnim(const fnd::SUpdateInfo& updateInfo)
        {
            Time += updateInfo.deltaTime;

            Eigen::Quaternion<float> q;
            q = Eigen::AngleAxis<float>(2.2689281 * Time, Eigen::Vector3f(0, 1, 0));
            csl::math::Quaternion rotation { q.x(), q.y(), q.z(), q.w() };


            int* gocContainer = GameObject::GetGOC(this, GOCVisual) + 0x10;
            if (!gocContainer)
                return;

            int* treasureModel = *(int**)(*gocContainer + 4);
            fnd::GOCVisualTransformed::SetLocalRotation(treasureModel, &rotation);

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            if (!game::GOCAnimationSimple::IsFinished(gocAnimation))
                return;

            State = ObjTreasureBoxState::STATE_OPEN_END;
        }

        void StateOpenEnd()
        {
            fnd::Message* treasureMessage = NULL;

            switch (ItemType)
            {
            case app::ObjTreasureBoxType::HEART_CONTAINER:
                treasureMessage = new xgame::MsgDlcZeldaTakeHeartContainer();
                break;
            case app::ObjTreasureBoxType::RUPEE_PURPLE:
                treasureMessage = new xgame::MsgGetAnimal(50);
                break;
            case app::ObjTreasureBoxType::RUPEE_GOLD:
                if (IsRupeeCountInChestFixed)
                    treasureMessage = new xgame::MsgGetAnimal(200);
                else
                    treasureMessage = new xgame::MsgGetAnimal(50);
                break;
            case app::ObjTreasureBoxType::RUPEE_GREEN:
            default:
                if (IsRupeeCountInChestFixed)
                    treasureMessage = new xgame::MsgGetAnimal(1);
                break;
            }

            ObjUtil::SendMessageImmToGameActor(this, treasureMessage);
            delete treasureMessage;

            // ObjZeldaOneUp

            int* gocContainer = GameObject::GetGOC(this, GOCVisual) + 0x10;
            if (!gocContainer)
                return;

            for (size_t i = 0; i < 2; i++)
            {
                int* gocVisual = *(int**)(*gocContainer + 4 * i);
                fnd::GOCVisual::SetVisible(gocVisual, false);
            }

            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;

            app::game::GOCEffect::CreateEffect(gocEffect, "ef_dl3_treasurebox_vanish");

            if (LayerController)
            {
                LayerController->ClearReservedAnimation();
                LayerController->PlayAnimation("Outro_Anim", 0, 0);
            }

            xgame::MsgDlcZeldaNoticeActiveEnemy activateEnemyMessage{};
            ObjUtil::SendMessageImmToGameActor(this, &activateEnemyMessage);

            xgame::MsgResumeGameTimer resumeTimerMessage{};
            ObjUtil::SendMessageImmToGameActor(this, &resumeTimerMessage);

            // PopCamera

            xgame::MsgCatchEndPlayer catchEndMessage { false };
            ObjUtil::SendMessageImmToPlayer(this, PlayerNumber, &catchEndMessage);

            State = ObjTreasureBoxState::STATE_OPENED;
            PlayerNumber = -1;
            Time = 0;
        }

        void StateOpened(const fnd::SUpdateInfo& updateInfo)
        {
            if (Time > 0)
            {
                if (LayerController)
                {
                    if (LayerController->IsEndAnimation())
                    {
                        CSetObjectListener::SetStatusRetire(this);
                        GameObject::Kill(this);
                        return;
                    }
                    return;
                }

                CSetObjectListener::SetStatusRetire(this);
                GameObject::Kill(this);
                return;
            }
            else
            {
                Time += updateInfo.deltaTime;
                xgame::MsgChangeBGMVolume changeBGMVolumeMessage{ 1, 2 };
                ObjUtil::SendMessageImmToGameActor(this, &changeBGMVolumeMessage);
            }
        }
    };

    inline static ObjTreasureBox* create_ObjTreasureBox() { return new ObjTreasureBox(); }

    inline static ObjTreasureBoxInfo* createObjInfo_ObjTreasureBoxInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjTreasureBoxInfo();
    }
}