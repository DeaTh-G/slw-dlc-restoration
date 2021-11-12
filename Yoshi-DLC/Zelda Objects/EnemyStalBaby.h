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
        float field_620{};
        float field_624{};
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
            fnd::GOComponent::Create(this, GOCGravity);
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationScript);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCShadowSimple);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, EnemyTargetGOC);
            fnd::GOComponent::Create(this, GOCEnemyHSM);
            fnd::GOComponent::Create(this, GOCMovementComplex);

            EnemyStalBabyData* data = (EnemyStalBabyData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            EnemyStalBabyInfo* info = (EnemyStalBabyInfo*)ObjUtil::GetObjectInfo(gameDocument, "EnemyStalBabyInfo");

            fnd::GOComponent::BeginSetup(this);

            fnd::GOComponent::EndSetup(this);
        }
    };
}
