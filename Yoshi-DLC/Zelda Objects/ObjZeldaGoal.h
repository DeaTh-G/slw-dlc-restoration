#pragma once

namespace app
{
    struct ObjZeldaGoalData
    {

    };

    class ObjZeldaGoalInfo : public CObjInfo
    {
    public:

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

        }

        const char* GetInfoName() override
        {
            return "ObjZeldaGoalInfo";
        }
    };

    class ObjZeldaGoal : public CSetObjectListener
    {

    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCShadow);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCEffect);

            ObjZeldaGoalInfo* info = (ObjZeldaGoalInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjZeldaGoalInfo");
            ObjZeldaGoalData* data = (ObjZeldaGoalData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            fnd::GOComponent::BeginSetup(this);



            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            switch (message.Type)
            {
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }
    };

    inline static ObjZeldaGoal* create_ObjZeldaGoal() { return new ObjZeldaGoal(); }

    inline static ObjZeldaGoalInfo* createObjInfo_ObjZeldaGoalInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjZeldaGoalInfo();
    }
}