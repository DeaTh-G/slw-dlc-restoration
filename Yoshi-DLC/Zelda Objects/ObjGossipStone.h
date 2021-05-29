#pragma once

namespace app
{
    enum class ObjGossipStoneState : int
    {
        STATE_IDLE,
        STATE_SHAKE,
        STATE_COUNTDOWN,
        STATE_ROCKET,
        STATE_LOST
    };

    struct ObjGossipStoneData
    {
        float CheckRadius;
    };

    class ObjGossipStoneInfo : public CObjInfo
    {
    public:
        int BaseModel{};
        int BaseSkeleton{};
        int BaseAnimation{};
        int FairyModel{};
        int FairySkeleton{};
        int FairyAnimation{};

        ObjGossipStoneInfo() {}

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&BaseModel, "zdlc03_obj_gossipstone", &packFile);
            ObjUtil::GetSkeletonResource(&BaseSkeleton, "zdlc03_obj_gossipstone", packFile);
            ObjUtil::GetAnimationResource(&BaseAnimation, "zdlc03_obj_gossipstone", &packFile);

            ObjUtil::GetModelResource(&FairyModel, "zdlc03_obj_fairy", &packFile);
            ObjUtil::GetSkeletonResource(&FairySkeleton, "zdlc03_obj_fairy", packFile);
            ObjUtil::GetAnimationResource(&FairyAnimation, "zdlc03_obj_fairy", &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjGossipStoneInfo";
        }

        void RegistCallback(int& container) override
        {
            // TODO: ItemBoxInfo
        }
    };

    class ObjGossipStone : public CSetObjectListener
    {
    public:
        INSERT_PADDING(48);

        void AddCallback(GameDocument* gameDocument) override
        {
            ObjGossipStoneInfo* treasureInfo = (ObjGossipStoneInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjGossipStoneInfo");
            ObjGossipStoneData* data = (ObjGossipStoneData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            fnd::GOComponent::BeginSetup(this);

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
                ProcMsgDamage((xgame::MsgDamage&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {

        }

    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {

        }

        void ProcMsgDamage(xgame::MsgDamage& message)
        {

        }
    };

    inline static ObjGossipStone* create_ObjGossipStone() { return new ObjGossipStone(); }

    inline static ObjGossipStoneInfo* createObjInfo_ObjGossipStoneInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjGossipStoneInfo();
    }
}