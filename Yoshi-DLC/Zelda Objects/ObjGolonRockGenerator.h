#pragma once

namespace app
{
    struct ObjGolonRockGeneratorData
    {
        float Speed;
        float CreateInterval;
        bool IsEventOn;
        bool IsCheckFall;
        INSERT_PADDING(2);
    };

    class ObjGolonRockGeneratorInfo : public CObjInfo
    {
    public:
        int Models[2]{};
        int Skeleton{};
        int Animation{};

        void Initialize(GameDocument& gameDocument) override
        {
            const char* modelNames[2] { "zdlc03_obj_goron", "zdlc03_obj_goronrock" };

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

    class ObjGolonRockGenerator : public CSetObjectListener
    {
    private:
        char Flags{};
        INSERT_PADDING(3);
        float Speed{};
        float CreateInterval{};
        float CreateIntervalCp{};

    public:

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);

            ObjGolonRockGeneratorInfo* info = (ObjGolonRockGeneratorInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjGolonRockGeneratorInfo");
            ObjGolonRockGeneratorData* data = (ObjGolonRockGeneratorData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            if (data->IsCheckFall)
                Flags |= 1;
            if (data->IsEventOn)
                Flags |= 2;

            Speed = data->Speed * 10;
            CreateInterval = data->CreateInterval;
            CreateIntervalCp = data->CreateInterval;

            fnd::GOComponent::BeginSetup(this);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                csl::math::Vector3 position { 0, 30, 0 };

                int shapeCount = 1;
                game::ColliBoxShapeCInfo collisionInfo{};
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                collisionInfo.MotionType = 2;
                collisionInfo.field_04 |= 3;
                collisionInfo.Size = Vector3(17.5f, 30, 17.5f);

                ObjUtil::SetupCollisionFilter(6, &collisionInfo);
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &position);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            game::GOCSound::SimpleSetup(this, 0, 0);
            game::GOCEffect::SimpleSetup(this);

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
        }

    private:
    };

    inline static ObjCocco* create_ObjCocco() { return new ObjCocco(); }

    inline static ObjCoccoInfo* createObjInfo_ObjCoccoInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjCoccoInfo();
    }
}