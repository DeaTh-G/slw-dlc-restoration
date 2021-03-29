#pragma once

namespace app
{
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
        INSERT_PADDING(20);
        int field_3B4;
        int field_3B8;
        float field_3BC;
        float field_3C0;
        int field_3C4;
        int field_3C8;
        int field_3CC;
        float field_3D0;
        int field_3D4;
        int SoundHandle[3]{};
        int field_3E4;
        int field_3E8;
        int field_3EC;

    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualContainer);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);

            ObjYoshiGoalInfo* info = (ObjYoshiGoalInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjYoshiGoalInfo");
            ObjYoshiGoalData* data = (ObjYoshiGoalData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

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

    private:
        bool GetModelType(int flowerID)
        {
            EggManager* eggManager = EggManager::GetService((GameDocument*)field_24[1]);
            if (eggManager && !(flowerID & 1))
                return eggManager->IsYoshiSpecialFlowerTaked(flowerID >> 1);

            return false;
        }
    };

    inline static ObjYoshiGoal* create_ObjYoshiGoal() { return new ObjYoshiGoal(); }

    inline static ObjYoshiGoalInfo* createObjInfo_ObjYoshiGoalInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjYoshiGoalInfo();
    }
}