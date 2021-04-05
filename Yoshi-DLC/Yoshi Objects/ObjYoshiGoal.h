#pragma once

namespace app
{
    enum class ObjYoshiGoalState : int
    {
        STATE_WAIT,
        STATE_WAIT_START_ROULETTE,
        STATE_ROULETTE,
        STATE_CHECK_HIT_FLOWER,
        STATE_DISAPPEAR_MODEL,
        STATE_WAIT_YOSHI_EXTRICATION,
        STATE_RESULT
    };

    struct ObjYoshiGoalData
    {
        int Locator;
        int FixCamera;
    };

    class ObjYoshiGoalInfo : public CObjInfo
    {
    public:
        int OffModels[2]{};
        int OnModels[2]{};
        int Animation{};

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));
            
            ObjUtil::GetModelResource(&OffModels[0], "zdlc02_obj_goalB_off", &packFile);
            ObjUtil::GetModelResource(&OffModels[1], "zdlc02_obj_goalA_off", &packFile);
            
            ObjUtil::GetModelResource(&OnModels[0], "zdlc02_obj_goalB_on", &packFile);
            ObjUtil::GetModelResource(&OnModels[1], "zdlc02_obj_goalA_on", &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjYoshiGoalInfo";
        }
    };

    class ObjYoshiGoal : public CSetObjectListener
    {
        INSERT_PADDING(12);
        ObjYoshiGoalState State{};
        xgame::MsgHitEventCollision* HitMessage = new xgame::MsgHitEventCollision();
        int ModelID{};
        int RouletteCount{};
        float DefaultTime = 0.05f;
        float Time = 0.05f;
        int PlayerActorID{};
        int Camera{};
        int DisappearModelID{};
        float RouletteTime{};
        int Flags{};
        int SoundHandle[3]{};
        int field_3E4{};
        int field_3E8{};
        int field_3EC{};

    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualContainer);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);

            ObjYoshiGoalInfo* info = (ObjYoshiGoalInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjYoshiGoalInfo");

            fnd::GOComponent::BeginSetup(this);

            int* gocContainer = GameObject::GetGOC(this, GOCVisual);
            if (gocContainer)
            {
                int modelCount = 20;
                fnd::GOCVisualContainer::Setup(gocContainer, &modelCount);

                Eigen::Quaternion<float> q;
                q = Eigen::AngleAxis<float>(0.69813168f, Eigen::Vector3f(0, 1, 0));
                csl::math::Quaternion rotation{ q.x(), q.y(), q.z(), q.w() };
                
                csl::math::Vector3 eulerRot{};
                csl::math::Vector3 zVec { 0, 0, 1 };
                math::Vector3Rotate(&eulerRot, &rotation, &zVec);
                for (size_t i = 0; i < 10; i++)
                {
                    bool modelType = GetModelType(i);

                    int* gocVisual = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
                    if (gocVisual)
                    {
                        fnd::GOCVisualModel::VisualDescription visualDescriptor{};

                        visualDescriptor.Model = info->OnModels[modelType];
                        fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                        Eigen::Quaternion<float> q2;
                        q2 = Eigen::AngleAxis<float>((((3.1415927f + 3.1415927f) / 10) * i),
                            Eigen::Vector3f(eulerRot.X, eulerRot.Y, eulerRot.Z));
                        csl::math::Quaternion rot{ q2.x(), q2.y(), q2.z(), q2.w() };

                        csl::math::Vector3 upVec { 0, 1, 0 };
                        math::Vector3Rotate(&upVec, &rot, &upVec);
                        math::Vector3Scale(&upVec, 20, &upVec);
                        fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &upVec);
                        fnd::GOCVisualContainer::Add(gocContainer, gocVisual);
                        fnd::GOCVisual::SetVisible(gocVisual, false);
                    }
                }
                for (size_t j = 0; j < 10; j++)
                {
                    bool modelType = GetModelType(j);

                    int* gocVisual = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
                    if (gocVisual)
                    {
                        fnd::GOCVisualModel::VisualDescription visualDescriptor{};

                        visualDescriptor.Model = info->OffModels[modelType];
                        fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                        Eigen::Quaternion<float> q2;
                        q2 = Eigen::AngleAxis<float>((((3.1415927f + 3.1415927f) / 10) * j),
                            Eigen::Vector3f(eulerRot.X, eulerRot.Y, eulerRot.Z));
                        csl::math::Quaternion rot{ q2.x(), q2.y(), q2.z(), q2.w() };

                        csl::math::Vector3 upVec{ 0, 1, 0 };
                        math::Vector3Rotate(&upVec, &rot, &upVec);
                        math::Vector3Scale(&upVec, 20, &upVec);
                        fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &upVec);
                        fnd::GOCVisualContainer::Add(gocContainer, gocVisual);
                    }
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                game::ColliBoxShapeCInfo collisionInfo{};

                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                collisionInfo.MotionType = 2;
                collisionInfo.Size = csl::math::Vector3(20, 20, 1);
                ObjUtil::SetupCollisionFilter(7, &collisionInfo);
                collisionInfo.field_04 |= 1;

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            if (message.Type != fnd::PROC_MSG_HIT_EVENT_COLLISION)
                return CSetObjectListener::ProcessMessage(message);

            ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
            return true;
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == ObjYoshiGoalState::STATE_WAIT)
                StateWait(updateInfo);

            if (State == ObjYoshiGoalState::STATE_WAIT_START_ROULETTE)
                StateWaitStartRoulette(updateInfo);

            if (State == ObjYoshiGoalState::STATE_ROULETTE)
                StateRoulette(updateInfo);

            if (State == ObjYoshiGoalState::STATE_CHECK_HIT_FLOWER)
                StateCheckHitFlower(updateInfo);

            if (State == ObjYoshiGoalState::STATE_DISAPPEAR_MODEL)
                StateDisappearModel(updateInfo);

            if (State == ObjYoshiGoalState::STATE_WAIT_YOSHI_EXTRICATION)
                StateWaitYoshiExtrication();

            if (State == ObjYoshiGoalState::STATE_RESULT)
                StateResult();
        }

    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            *HitMessage = message;
        }

        bool GetModelType(int flowerID)
        {
            EggManager* eggManager = EggManager::GetService((GameDocument*)field_24[1]);
            if (eggManager && !(flowerID & 1))
                return eggManager->IsYoshiSpecialFlowerTaked(flowerID >> 1);

            return false;
        }

        bool UpdateSelectModel(const fnd::SUpdateInfo& updateInfo)
        {
            Time -= updateInfo.deltaTime;
            if (Time > 0)
                return false;

            int* gocContainer = GameObject::GetGOC(this, GOCVisual) + 0x10;
            if (!gocContainer)
                return false;

            int* offModel = *(int**)(*gocContainer + 4 * ModelID);
            fnd::GOCVisual::SetVisible(offModel, false);
            int* onModel = *(int**)(*gocContainer + 4 * (ModelID + 10));
            fnd::GOCVisual::SetVisible(onModel, true);

            ModelID++;
            if (ModelID >= 10)
                ModelID = 0;

            int* nextModel = *(int**)(*gocContainer + 4 * ModelID);
            fnd::GOCVisual::SetVisible(nextModel, true);

            Time = DefaultTime;
            return true;
        }

        void StateWait(const fnd::SUpdateInfo& updateInfo)
        {
            UpdateSelectModel(updateInfo);

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], HitMessage->ActorID);
            if (playerNo < 0)
                return;
            
            ObjYoshiGoalData* data = (ObjYoshiGoalData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            PlayerActorID = HitMessage->ActorID;
            xgame::MsgPLHoldOn holdOnMessage { 1 };
            SendMessageImm(PlayerActorID, &holdOnMessage);

            csl::math::Vector3 targetPosition;
            csl::math::Quaternion targetRotation;
            if (ObjUtil::GetSetObjectTransform(*Document, &data->Locator,
                &targetPosition, &targetRotation))
            {
                xgame::MsgPLJumpToTargetPosition jumpTargetMessage{ 100, 200, targetPosition, targetRotation, 1 };
                SendMessageImm(PlayerActorID, &jumpTargetMessage);
            }

            xgame::MsgCameraOn cameraMessage { 0, 3001, 1, playerNo, 0, 0.5f };
            if (ObjUtil::SendMessageImmToSetObject(this, &data->FixCamera, &cameraMessage, 0))
                Camera = data->FixCamera;

            EggManager* eggManager = EggManager::GetService((GameDocument*)field_24[1]);
            if (eggManager)
                eggManager->StartExtrication(playerNo);

            xgame::MsgStopGameTimer stopTimerMessage{};
            ObjUtil::SendMessageImmToGameActor(this, &stopTimerMessage);

            xgame::MsgDisableItemBag disableBagMessage{};
            ObjUtil::SendMessageImmToGameActor(this, &disableBagMessage);

            for (size_t i = 0; i < 3; i++)
            {
                xgame::MsgStopBgm stopBgmMessage{ 0, i };
                ObjUtil::SendMessageImmToGameActor(this, &stopBgmMessage);
            }

            State = ObjYoshiGoalState::STATE_WAIT_START_ROULETTE;
        }

        void StateWaitStartRoulette(const fnd::SUpdateInfo& updateInfo)
        {
            if (RouletteTime <= 0.000001f)
            {
                int deviceTag[3]{};
                void* soundPlayer = *(void**)ASLR(0x00FD3CB8);
                if (!soundPlayer)
                    return;
            
                fnd::SoundParam soundParam { 1, 0, 0, 0x80000000, 0, 0, 0 };
                fnd::SoundPlayerCri::Play(soundPlayer, deviceTag, 0, "jingle_stg_clear_zdlc02", soundParam);
                fnd::HandleBase::__as(SoundHandle, deviceTag);
                SoundHandle[2] = deviceTag[2];
            }

            if (UpdateSelectModel(updateInfo))
            {
                int deviceTag[3]{};
                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (gocSound)
                    game::GOCSound::Play(gocSound, deviceTag, "obj_yossygoal_roulette", 0);
            }

            RouletteTime += updateInfo.deltaTime;
            if (RouletteTime > 1.5)
                State = ObjYoshiGoalState::STATE_ROULETTE;
        }

        void StateRoulette(const fnd::SUpdateInfo& updateInfo)
        {
            if (!UpdateSelectModel(updateInfo))
                return;

            int deviceTag[3]{};
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (gocSound)
                game::GOCSound::Play(gocSound, deviceTag, "obj_yossygoal_roulette", 0);

            RouletteCount++;
            DefaultTime += 0.05f;
            if (RouletteCount >= 12)
                State = ObjYoshiGoalState::STATE_CHECK_HIT_FLOWER;
        }

        void StateCheckHitFlower(const fnd::SUpdateInfo& updateInfo)
        {
            RouletteTime += updateInfo.deltaTime;
            if (RouletteTime <= 1.5f)
                return;

            int deviceTag[3]{};
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            bool modelType = GetModelType(ModelID);
            Flags |= (modelType != 0);
            if (modelType & 1)
            {
                for (size_t i = 0; i < 10; i++)
                {
                    xgame::MsgTakeObject takeMessage { xgame::MsgTakeObject::EType::ONE_UP };
                    SendMessageImm(PlayerActorID, &takeMessage);
                }

                Flags |= 2;
                game::GOCSound::Play(gocSound, deviceTag, "obj_yossygoal_roulette_success", 0);
                fnd::HandleBase::__as(SoundHandle, deviceTag);
                SoundHandle[2] = deviceTag[2];
                State = ObjYoshiGoalState::STATE_DISAPPEAR_MODEL;
            }
            else
            {
                game::GOCSound::Play(gocSound, deviceTag, "obj_yossygoal_roulette_miss", 0);
                State = ObjYoshiGoalState::STATE_DISAPPEAR_MODEL;
            }
            DisappearModelID = ModelID;
        }

        void StateDisappearModel(const fnd::SUpdateInfo& updateInfo)
        {
            RouletteTime -= updateInfo.deltaTime;
            if (RouletteTime > 0)
                return;

            DisappearModelID += 1;
            RouletteTime = 0.1f;
            if (DisappearModelID >= 10)
                DisappearModelID = 0;
            
            int* gocContainer = GameObject::GetGOC(this, GOCVisual) + 0x10;
            if (!gocContainer)
                return;

            int* offModel = *(int**)(*gocContainer + 4 * DisappearModelID);
            fnd::GOCVisual::SetVisible(offModel, false);
            int* onModel = *(int**)(*gocContainer + 4 * (DisappearModelID + 10));
            fnd::GOCVisual::SetVisible(onModel, false);

            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (gocEffect)
            {
                game::EffectCreateInfo effectInfo;
                game::EffectCreateInfo::__ct(&effectInfo);
                effectInfo.Name = "ef_dl2_goal_vanish";
                effectInfo.field_04 = 1;
                effectInfo.field_08 = 3;
                effectInfo.field_30 = true;
                effectInfo.field_40 = *(int**)(*gocContainer + 4 * DisappearModelID);
                if ((DisappearModelID == ModelID) && (Flags & 1))
                    effectInfo.field_04 = 2;

                game::GOCEffect::CreateEffectEx(gocEffect, &effectInfo);
            
                if (Flags & 1)
                {
                    if (DisappearModelID == ModelID)
                        State = ObjYoshiGoalState::STATE_WAIT_YOSHI_EXTRICATION;
                }
                else
                {
                    int deviceTag[3]{};
                    int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                    if (!gocSound)
                        return;

                    game::GOCSound::Play(gocSound, deviceTag, "obj_yossygoal_roulette_disappear", 0);
                    fnd::HandleBase::__as(SoundHandle, deviceTag);
                    SoundHandle[2] = deviceTag[2];

                    if (DisappearModelID == ModelID)
                    {
                        Flags |= 2;
                        State = ObjYoshiGoalState::STATE_WAIT_YOSHI_EXTRICATION;
                    }
                }
            }
        }

        void StateWaitYoshiExtrication()
        {
            EggManager* eggManager = EggManager::GetService((GameDocument*)field_24[1]);
            if (!eggManager)
                return;

            int winnerNo = ObjUtil::GetPlayerNo(field_24[1], HitMessage->ActorID);
            if (!eggManager->IsEndExtrication(winnerNo))
                return;

            /* Send Battle Goal Message if a second player exists */
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], 1);
            if (playerInfo)
            {
                csl::math::Matrix34 matrixP1{};
                *(csl::math::Vector3*)&matrixP1.data[0][0] = Vector3(1, 0, 0);
                *(csl::math::Vector3*)&matrixP1.data[1][0] = Vector3(0, 1, 0);
                *(csl::math::Vector3*)&matrixP1.data[2][0] = Vector3(0, 0, 1);
                *(csl::math::Vector3*)&matrixP1.data[3][0] = Vector3(502.8f, 103, 0);

                csl::math::Matrix34 matrixP2{};
                *(csl::math::Vector3*)&matrixP2.data[0][0] = Vector3(1, 0, 0);
                *(csl::math::Vector3*)&matrixP2.data[1][0] = Vector3(0, 1, 0);
                *(csl::math::Vector3*)&matrixP2.data[2][0] = Vector3(0, 0, 1);
                *(csl::math::Vector3*)&matrixP2.data[3][0] = Vector3(520.2f, 103, 0);

                int playerNo = ObjUtil::GetPlayerNo(field_24[1], HitMessage->ActorID);
                xgame::MsgCameraOff cameraMessage{ 0, 0, 1, playerNo, 1 };
                ObjUtil::SendMessageImmToSetObject(this, &Camera, &cameraMessage, 0);

                xgame::MsgGoalForBattle goalMessage { playerNo, &matrixP1, &matrixP2 };
                SendMessageImm(*(int*)(field_24[1] + 0x10), &goalMessage);
            }
            else
            {
                xgame::MsgPlayerReachGoal goalMessage{};
                SendMessageImm(PlayerActorID, &goalMessage);
            }
            State = ObjYoshiGoalState::STATE_RESULT;
        }

        void StateResult()
        {
            EggManager* eggManager = EggManager::GetService((GameDocument*)field_24[1]);
            if (!eggManager)
                return;

            for (app::ObjEgg* egg : eggManager->EggsP1)
            {
                int* gocEffect = GameObject::GetGOC(egg, GOCEffectString);
                if (gocEffect)
                    game::GOCEffect::CreateEffect(gocEffect, "ef_dl2_yossi_birth");

                int deviceTag[3];
                int* gocSound = GameObject::GetGOC(egg, GOCSoundString);
                if (gocSound)
                    game::GOCSound::Play(gocSound, deviceTag, "obj_yossyeggitem_break", 0);

                GameObject::Kill(egg);
            }

            eggManager->EggsP1.clear();

            for (app::ObjEgg* egg : eggManager->EggsP2)
            {
                int* gocEffect = GameObject::GetGOC(egg, GOCEffectString);
                if (gocEffect)
                    game::GOCEffect::CreateEffect(gocEffect, "ef_dl2_yossi_birth");

                int deviceTag[3];
                int* gocSound = GameObject::GetGOC(egg, GOCSoundString);
                if (gocSound)
                    game::GOCSound::Play(gocSound, deviceTag, "obj_yossyeggitem_break", 0);

                GameObject::Kill(egg);
            }

            eggManager->EggsP2.clear();
        }
    };

    inline static ObjYoshiGoal* create_ObjYoshiGoal() { return new ObjYoshiGoal(); }

    inline static ObjYoshiGoalInfo* createObjInfo_ObjYoshiGoalInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjYoshiGoalInfo();
    }
}