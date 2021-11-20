#pragma once

namespace app
{
    inline static const char* RUPEE_MODEL_NAMES[] = 
    {
        "zdlc03_obj_rupee_green01",
        "zdlc03_obj_rupee_blue01",
        "zdlc03_obj_rupee_red01",
        "zdlc03_obj_rupee_purple01",
        "zdlc03_obj_rupee_gold01"
    };

    enum class ObjZeldaRupeeType : char
    {
        GREEN,
        BLUE,
        RED,
        PURPLE,
        GOLD
    };

    enum class ObjZeldaRupeeState : int
    {
        STATE_ACTIVE,
        STATE_INACTIVE
    };

    struct ObjZeldaRupeeData
    {
        ObjZeldaRupeeType CreateItem;
        bool IsEventOn;
        bool IsShadowOff;
    };

    class ObjZeldaRupeeInfo : public CObjInfo
    {
    public:
        int Models[5]{};

        ObjZeldaRupeeInfo() {}

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            for (size_t i = 0; i < 5; i++)
                ObjUtil::GetModelResource(&Models[i], RUPEE_MODEL_NAMES[i], &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjZeldaRupeeInfo";
        }

        void RegistCallback(int& container) override
        {
            ObjZeldaPopupItemInfo* popupObject = new(pAllocator) ObjZeldaPopupItemInfo();
            if (popupObject)
                CObjInfoContainer::Register(&container, popupObject->GetInfoName(), popupObject);
        }
    };

    class ObjZeldaRupee : public CSetObjectListener
    {
    public:
        INSERT_PADDING(8);
        xgame::MsgNotifyObjectEvent* NotifyMessage = new xgame::MsgNotifyObjectEvent();
        xgame::MsgHitEventCollision* HitMessage = new xgame::MsgHitEventCollision();
        ObjZeldaRupeeState State{};
        ObjZeldaRupeeType Type{};
        float Time{};
        INSERT_PADDING(4);

        void Destructor(size_t deletingFlags) override
        {
            delete NotifyMessage;
            delete HitMessage;

            CSetObjectListener::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            ObjZeldaRupeeInfo* info = (ObjZeldaRupeeInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjZeldaRupeeInfo");
            ObjZeldaRupeeData* data = (ObjZeldaRupeeData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            Type = data->CreateItem;

            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            if (!data->IsShadowOff)
                fnd::GOComponent::Create(this, GOCShadowSimple);

            if (data->IsEventOn)
                State = ObjZeldaRupeeState::STATE_INACTIVE;

            fnd::GOComponent::BeginSetup(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                csl::math::Vector3 position{ 0, 3.0f, 0 };
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Models[(int)Type];
                visualDescriptor.Animation |= 0x400000;

                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &position);
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                csl::math::Vector3 position{ 0, 3.0f, 0 };
                game::ColliSphereShapeCInfo collisionInfo;

                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 0;
                collisionInfo.Radius = 4;
                collisionInfo.field_04 = 1;
                ObjUtil::SetupCollisionFilter(6, &collisionInfo);
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &position);

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                game::ShadowSphereShapeCInfo shadowInfo;

                game::ShadowSphereShapeCInfo::__ct(&shadowInfo, 2.3);
                shadowInfo.field_04 = 6;
                shadowInfo.ShadowQualityType = 0;

                game::ShadowSphereShapeCInfo* ppShadowInfo = &shadowInfo;
                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
                game::GOCShadowSimple::SetMaxDistDown(gocShadow, 300);
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

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
            case fnd::PROC_MSG_NOTIFY_OBJECT_EVENT:
                ProcMsgNotifyObjectEvent((xgame::MsgNotifyObjectEvent&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == ObjZeldaRupeeState::STATE_ACTIVE)
                StateActive(updateInfo);

            if (State == ObjZeldaRupeeState::STATE_INACTIVE)
                StateInactive(updateInfo);
        }

    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            *HitMessage = message;
        }

        void ProcMsgNotifyObjectEvent(xgame::MsgNotifyObjectEvent& message)
        {
            *NotifyMessage = message;
        }

        void StateActive(const fnd::SUpdateInfo& updateInfo)
        {
            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (!gocVisual)
                return;

            Time = Time + updateInfo.deltaTime;
            float radianRotation = fmodf(2.2689281f * Time, 3.1415927f + 3.1415927f);

            Eigen::Quaternion<float> q;
            q = Eigen::AngleAxis<float>(radianRotation, Eigen::Vector3f(0, 1, 0));
            csl::math::Quaternion visualRotation{ q.x(), q.y(), q.z(), q.w() };
            fnd::GOCVisualTransformed::SetLocalRotation(gocVisual, &visualRotation);

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], HitMessage->ActorID);
            if (playerNo < 0)
                return;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            zelda_popupitem::ZeldaPopupItemCinfo popupInfo { (csl::math::Matrix34*)(gocTransform + 0x44), (ObjZeldaPopupItemType)((int)Type), playerNo };
            zelda_popupitem::CreateZeldaPopupItem((GameDocument&)field_24[1], &popupInfo);
            GameObject::Kill();
            SetStatusRetire();
        }

        void StateInactive(const fnd::SUpdateInfo& updateInfo)
        {
            if (!NotifyMessage->field_18)
            {
                int* gocVisual = GameObject::GetGOC(this, GOCVisual);
                if (!gocVisual)
                    return;

                fnd::GOCVisual::SetVisible(gocVisual, false);

                int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
                if (!gocCollider)
                    return;

                game::GOCCollider::SetEnable(gocCollider, false);

                int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
                if (!gocShadow)
                    return;

                game::GOCShadowSimple::SetVisible(gocShadow, false);
            }

            if (NotifyMessage->field_18)
            {
                State = ObjZeldaRupeeState::STATE_ACTIVE;
                Time = 0;

                int* gocVisual = GameObject::GetGOC(this, GOCVisual);
                if (!gocVisual)
                    return;

                fnd::GOCVisual::SetVisible(gocVisual, true);

                int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
                if (!gocCollider)
                    return;

                game::GOCCollider::SetEnable(gocCollider, true);

                int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
                if (!gocShadow)
                    return;

                game::GOCShadowSimple::SetVisible(gocShadow, true);
            }
        }
    };

    inline static ObjZeldaRupee* create_ObjZeldaRupee() { return new ObjZeldaRupee(); }

    inline static ObjZeldaRupeeInfo* createObjInfo_ObjZeldaRupeeInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjZeldaRupeeInfo();
    }
}