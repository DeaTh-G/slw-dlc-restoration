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
        int field_3B8{};
        float field_3BC = 0.05f;
        float Time = 0.05f;
        int PlayerActorID{};
        int field_3C8{};
        int field_3CC{};
        float field_3D0{};
        int field_3D4{};
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
                StateWaitStartRoulette();

            if (State == ObjYoshiGoalState::STATE_ROULETTE)
                StateRoulette();

            if (State == ObjYoshiGoalState::STATE_CHECK_HIT_FLOWER)
                StateCheckHitFlower();

            if (State == ObjYoshiGoalState::STATE_DISAPPEAR_MODEL)
                StateDisappear();

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

        void UpdateSelectModel(const fnd::SUpdateInfo& updateInfo)
        {
            Time -= updateInfo.deltaTime;
            if (Time > 0)
                return;

            int* gocContainer = GameObject::GetGOC(this, GOCVisual) + 0x10;
            if (!gocContainer)
                return;

            int* offModel = *(int**)(*gocContainer + 4 * ModelID);
            fnd::GOCVisual::SetVisible(offModel, false);
            int* onModel = *(int**)(*gocContainer + 4 * (ModelID + 10));
            fnd::GOCVisual::SetVisible(onModel, true);

            ModelID++;
            if (ModelID >= 10)
                ModelID = 0;

            int* nextModel = *(int**)(*gocContainer + 4 * ModelID);
            fnd::GOCVisual::SetVisible(nextModel, true);

            Time = field_3BC;
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
            if (!ObjUtil::GetSetObjectTransform(*Document, &data->Locator,
                &targetPosition, &targetRotation))
                return;


        }

        void StateWaitStartRoulette()
        {

        }

        void StateRoulette()
        {

        }

        void StateCheckHitFlower()
        {

        }

        void StateDisappear()
        {

        }

        void StateWaitYoshiExtrication()
        {

        }

        void StateResult()
        {

        }
    };

    inline static ObjYoshiGoal* create_ObjYoshiGoal() { return new ObjYoshiGoal(); }

    inline static ObjYoshiGoalInfo* createObjInfo_ObjYoshiGoalInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjYoshiGoalInfo();
    }
}