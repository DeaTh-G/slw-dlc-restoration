#pragma once

namespace app
{
    class ObjTreasureBoxInfo : public CObjInfo
    {
    public:

        void Initialize(GameDocument& gameDocument) override
        {
        }

        const char* GetInfoName() override
        {
            return "ObjTreasureBoxInfo";
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