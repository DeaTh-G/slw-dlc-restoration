#pragma once

namespace app
{
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
        void AddCallback(GameDocument* gameDocument) override
        {
            ObjTreasureBoxInfo* info = (ObjTreasureBoxInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjTreasureBoxInfo");

            fnd::GOComponent::BeginSetup(this);


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