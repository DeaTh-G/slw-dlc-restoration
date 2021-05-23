#pragma once

namespace app
{
    enum class ObjTreasureBoxState : int
    {
        STATE_INITIALIZE,
        STATE_WAIT,
        STATE_HITOFF,
        STATE_OPEN_CONTROL_CAMERA,
        STATE_OPEN_WAIT_ANIM,
        STATE_OPEN_END,
        STATE_OPENED
    };

    struct ObjTreasureBoxData
    {
        int ItemType;
    };

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
        class Listener : public animation::AnimationListener
        {
        public:
            ObjTreasureBox* pTreasureBox;

            void OnEvent(int notifyTiming) override
            {
                if (!pTreasureBox)
                    return;


            }
        };

        INSERT_PADDING(20);
        Listener AnimationListener{};
        void* pTreasureBoxCamera{};
        INSERT_PADDING(12);
        csl::math::Vector3 field_3F0{};
        csl::math::Vector3 field_400{};
        csl::math::Matrix34 field_410{};
        int field_450{};
        int field_454{};
        INSERT_PADDING(4);
        int field_45C{};

        ObjTreasureBox()
        {
            AnimationListener.field_20 = 2;
        }

        void Destructor(size_t deletingFlags)
        {
            AnimationListener.Destructor(0);

            CSetObjectListener::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualContainer);
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCHud);

            ObjTreasureBoxInfo* info = (ObjTreasureBoxInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjTreasureBoxInfo");
            ObjTreasureBoxData* data = (ObjTreasureBoxData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

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