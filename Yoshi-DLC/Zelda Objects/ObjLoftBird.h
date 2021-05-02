#pragma once

namespace app
{
    enum class ObjLoftBirdLightType : char
    {
        Zero,
        One,
        Two
    };

    struct ObjLoftBirdData
    {
        unsigned int ObjPathId;
        float StartDist;
        float EndDist;
        float MoveSpeed;
        ObjLoftBirdLightType PointLight;
    };

    class ObjLoftBirdInfo : public CObjInfo
    {
    public:
        int Models[2];
        int Skeletons[2];
        animation::AnimationResContainer LinkAnimationContainer{};
        animation::AnimationResContainer BirdAnimationContainer{};
        int TexSrtAnimContainer[4]{};

        ObjLoftBirdInfo()
        {
            animation::AnimationResContainer::__ct(&LinkAnimationContainer, (csl::fnd::IAllocator*)pAllocator);
            animation::AnimationResContainer::__ct(&BirdAnimationContainer, (csl::fnd::IAllocator*)pAllocator);
        }

        void Destructor(size_t deletingFlags) override
        {
            ObjLoftBirdInfo::~ObjLoftBirdInfo();
            animation::AnimationResContainer::__dt(&LinkAnimationContainer);
            animation::AnimationResContainer::__dt(&BirdAnimationContainer);

            CObjInfo::Destructor(deletingFlags);
        }

        void Initialize(GameDocument& gameDocument) override
        {
            const char* modelNames[2] { "zdlc03_obj_link", "zdlc03_obj_loftbird" };
            const char* texSrtAnimNames[4] { "lnk_event01_Eye_L", "lnk_event01_Eye_R", "lnk_event02_Eye_L", "lnk_event02_Eye_R" };

            int packFile = 0;
            int animationScript[3]{};
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            for (size_t i = 0; i < 2; i++)
            {
                ObjUtil::GetModelResource(&Models[i], modelNames[i], &packFile);
                ObjUtil::GetSkeletonResource(&Skeletons[i], modelNames[i], packFile);
            }

            ObjUtil::GetAnimationScriptResource(&animationScript, "zdlc03_obj_link", packFile);
            animationScript[1] = Skeletons[0];

            animationScript[0] = 0; animationScript[1] = 0; animationScript[2] = 0;
            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&LinkAnimationContainer, animationScript, packFile);

            ObjUtil::GetAnimationScriptResource(&animationScript, "zdlc03_obj_loftbird", packFile);
            animationScript[1] = Skeletons[1];

            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&BirdAnimationContainer, animationScript, packFile);

            for (size_t i = 0; i < 4; i++)
                ObjUtil::GetTexSrtAnimationResource(&TexSrtAnimContainer[i], texSrtAnimNames[i], packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjLoftBirdInfo";
        }
    };

    class ObjLoftBird : public CSetObjectListener
    {
        class Listener : public animation::AnimationListener {};

        game::PathEvaluator PathEvaluator{};
        float EndDistance{};
        float MovementSpeed{};
        Listener AnimationListener{};
        int field_3E0{};
        int SoundHandle[3]{};
        int field_3F0{};
        EnemyUvAnimLinkController UvLinkController{};
        int field_420{};
        int field_424{};
        int field_428{};
        int field_42C{};
        int field_430{};
        int field_434{};
        int field_438{};
        int field_43C{};
        int field_440{};
        int field_444{};
        int field_448{};
        int field_44C{};

        void AddCallback(GameDocument* gameDocument) override
        {
            sizeof(ObjLoftBird);
            fnd::GOComponent::Create(this, GOCVisualContainer);
            fnd::GOComponent::Create(this, GOCSound);

            fnd::GOComponent::BeginSetup(this);

            ObjLoftBirdInfo* info = (ObjLoftBirdInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjLoftBirdInfo");
            ObjLoftBirdData* data = (ObjLoftBirdData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            EndDistance = data->EndDist;
            MovementSpeed = data->MoveSpeed;
            if (data->PointLight != ObjLoftBirdLightType::Zero)
                field_420 = 1;
            else
                field_420 = 0;

            if (data->PointLight != ObjLoftBirdLightType::One)
            {
                int* gocContainer = GameObject::GetGOC(this, GOCVisual);
                if (gocContainer)
                {
                    int modelCount = 2;
                    fnd::GOCVisualContainer::Setup(gocContainer, &modelCount);

                    for (size_t i = 0; i < 2; i++)
                    {
                        int* gocVisual = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
                        if (gocVisual)
                        {
                            csl::math::Vector3 scale { 2, 2, 2 };

                            fnd::GOCVisualModel::VisualDescription visualDescriptor{};
                            fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                            visualDescriptor.Model = info->Models[i];
                            visualDescriptor.Skeleton = info->Skeletons[i];
                            fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                            fnd::GOCVisualContainer::Add(gocContainer, gocVisual);
                            fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &scale);
                            fnd::GOCVisual::SetVisible(gocVisual, false);
                        }
                    }
                }
            }

            fnd::GOComponent::EndSetup(this);
        }
    };

    inline static ObjLoftBird* create_ObjLoftBird() { return new ObjLoftBird(); }

    inline static ObjLoftBirdInfo* createObjInfo_ObjLoftBirdInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjLoftBirdInfo();
    }
}