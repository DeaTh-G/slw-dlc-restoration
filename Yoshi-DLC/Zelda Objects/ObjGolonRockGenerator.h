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

    class ObjGolonRockGenerator : public CSetObjectListener
    {
    private:
        char Flags{};
        INSERT_PADDING(3);
        float Speed{};
        float CreateInterval{};
        float Time{};

    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);

            ObjGolonRockGeneratorData* data = (ObjGolonRockGeneratorData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            if (data->IsCheckFall)
                Flags |= 1;
            if (data->IsEventOn)
                Flags |= 2;

            Speed = data->Speed * 10;
            CreateInterval = data->CreateInterval;
            Time = data->CreateInterval;

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
            if (PreProcessMessage(message))
                return true;

            switch (message.Type)
            {
            case fnd::PROC_MSG_DLC_ZELDA_NOTICE_ACTIVE_ENEMY:
                ProcMsgDlcZeldaNoticeActiveEnemy((xgame::MsgDlcZeldaNoticeActiveEnemy&)message);
                return true;
            case fnd::PROC_MSG_DLC_ZELDA_NOTICE_STOP_ENEMY:
                ProcMsgDlcZeldaNoticeStopEnemy((xgame::MsgDlcZeldaNoticeStopEnemy&)message);
                return true;
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return true;
            case fnd::PROC_MSG_LEAVE_EVENT_COLLISION:
                ProcMsgLeaveEventCollision((xgame::MsgLeaveEventCollision&)message);
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
            if (!(Flags & 2))
                return;
        
            Time += updateInfo.deltaTime;
            if (Flags & 4 || Time < CreateInterval)
                return;
        
            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            Time = 0;

            golon_rock::GolonRockCreateInfo cInfo { *(csl::math::Matrix34*)((int*)gocTransform + 0x44), -Speed, (Flags & 1) == 1 };
            golon_rock::CreateGolonRock((GameDocument&)field_24[1], cInfo);

            int deviceTag[3]{};
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play3D(gocSound, deviceTag, "obj_goron_generate", 0);

            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;

            game::GOCEffect::CreateEffect(gocEffect, "ef_dl3_goron_appear");
        }

    private:
        void ProcMsgDlcZeldaNoticeActiveEnemy(xgame::MsgDlcZeldaNoticeActiveEnemy& message)
        {
            Flags &= ~4;
        }

        void ProcMsgDlcZeldaNoticeStopEnemy(xgame::MsgDlcZeldaNoticeStopEnemy& message)
        {
            Flags |= 4;
        }

        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            Flags |= 4;
        }

        void ProcMsgLeaveEventCollision(xgame::MsgLeaveEventCollision& message)
        {
            Flags &= ~4;
        }

        void ProcMsgNotifyObjectEvent(xgame::MsgNotifyObjectEvent& message)
        {
            if (message.field_18 == 1)
                Flags |= 2;
        }
    };

    inline static ObjGolonRockGenerator* create_ObjGolonRockGenerator() { return new ObjGolonRockGenerator(); }

    inline static ObjGolonRockGeneratorInfo* createObjInfo_ObjGolonRockGeneratorInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjGolonRockGeneratorInfo();
    }
}