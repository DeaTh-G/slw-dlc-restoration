#pragma once

namespace app
{
    struct ObjYoshiCoinData
    {
        float ResetTime;
        bool IsReset;
        char SetPlaceType;
        char EventType;
    };

    class ObjYoshiCoinInfo : public CObjInfo
    {
    public:
        int Model{};

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));
            ObjUtil::GetModelResource(&Model, "zdlc02_obj_yoshicoin", &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjYoshiCoinInfo";
        }
    };

    class ObjYoshiCoin : public CSetObjectListener
    {
        void* RingManager;
        float ResetTime{};
        bool IsReset;
        INSERT_PADDING(7);

    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            sizeof(ObjYoshiCoin);
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);

            if (IsConsistentShadow)
                fnd::GOComponent::Create(this, GOCShadowSimple);

            ObjYoshiCoinData* data = (ObjYoshiCoinData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            ResetTime = data->ResetTime;
            IsReset = data->IsReset != 0;

            ObjYoshiCoinInfo* info = (ObjYoshiCoinInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjYoshiCoinInfo");
            GameObject* parentObject = CSetObjectListener::GetParentObject(this);

            fnd::GOComponent::BeginSetup(this);

            // Caused allocation error randomly, now it decided to do. Check back in the future.
            RingManager = Gimmick::CRingManager::GetService(gameDocument, (void*)ASLR(0x00FECC20));
            if (RingManager)
                Gimmick::CRingManager::RegisterRotateRing(RingManager, this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                csl::math::Vector3 position{ 0, 5.0f, 0 };
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Animation |= 0x400000;
                visualDescriptor.LightQualityType = 1;
                visualDescriptor.field_30 = 0x60000000;

                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &position);
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                csl::math::Vector3 position{ 0, 5.0f, 0 };
                game::ColliSphereShapeCInfo collisionInfo;

                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 0;
                if (!parentObject)
                    collisionInfo.MotionType = 2;
                collisionInfo.Radius = 3;
                collisionInfo.field_04 = 1;
                ObjUtil::SetupCollisionFilter(12, &collisionInfo);
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &position);

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                csl::math::Vector3 position{ 0, 5.0f, 0 };
                game::ShadowHemisphereShapeCInfo shadowInfo;

                game::ShadowHemisphereShapeCInfo::__ct(&shadowInfo, 3);
                if (parentObject || data->SetPlaceType == 1)
                    shadowInfo.field_04 = 5;
                else
                    shadowInfo.field_04 = 6;

                game::ShadowHemisphereShapeCInfo* ppShadowInfo = &shadowInfo;
                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
                app::game::GOCShadowSimple::SetLocalOffsetPosition(gocShadow, &position);
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

            uint32_t extUserData = GetExtUserData(0);
            if (extUserData == 1 || data->EventType == 2)
                SetActivate(false);
            
            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::MessageNew& message) override
        {
            if (PreProcessMessage(message))
                return true;

            switch (message.Type)
            {
            case fnd::PROC_MSG_NOTIFY_OBJECT_EVENT:
                ProcMsgNotifyObjectEvent((xgame::MsgNotifyObjectEvent&)message);
                return true;
            case fnd::PROC_MSG_RING_GET_SUCKED_INTO:
                // TODO - MsgRingGetSuckedInto
                return false;
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

    private:
        void SetActivate(bool isActive)
        {
            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (!gocVisual)
                return;
            fnd::GOCVisual::SetVisible(gocVisual, isActive);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (!gocCollider)
                return;
            game::GOCCollider::SetEnable(gocCollider, isActive);

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (!gocShadow)
                return;
            game::GOCShadowSimple::SetVisible(gocShadow, isActive);
        }

        void SetStatusToKill()
        {
            if (IsReset)
                CSetObjectListener::SetStatusTemporaryRetire(this, ResetTime);
            else
                CSetObjectListener::SetStatusRetire(this);

            GameObject::Kill(this);
        }

        void ProcMsgNotifyObjectEvent(xgame::MsgNotifyObjectEvent& message)
        {
            ObjYoshiCoinData* data = (ObjYoshiCoinData*)CSetAdapter::GetData(*(int**)((char*)this + 0x31C));
            if (data->EventType)
            {
                if (message.field_18 == 1)
                {
                    SetActivate(true);
                    SetExtUserData(0, 2);
                }
                else if (message.field_18 == 2)
                {
                    SetActivate(false);
                    SetExtUserData(0, 1);
                }
            }
        }

        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            xgame::MsgTakeObject msgTakeObject { 0 };
            msgTakeObject.SetShapeUserID(message.field_1C[0x2F]);
            if (SendMessageImm(message.ActorID, &msgTakeObject))
            {
                int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
                if (gocEffect)
                {
                    game::EffectCreateInfo effectInfo;
                    game::EffectCreateInfo::__ct(&effectInfo);
                    effectInfo.Name = "ef_dl2_coin_get";
                    effectInfo.field_04 = 1;
                    effectInfo.Position.Y = 5;
                    effectInfo.field_30 = true;

                    game::GOCEffect::CreateEffectEx(gocEffect, &effectInfo);
                }

                int deviceTag[3];
                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (gocSound)
                    game::GOCSound::Play(gocSound, deviceTag, "obj_yossycoin_get", 0);

                int playerNo = ObjUtil::GetPlayerNo(field_24[1], message.ActorID);
                ObjUtil::AddScorePlayerAction(this, "GET_YOSHICOIN", playerNo);

                SetStatusToKill();
            }
        }
    };

    inline static ObjYoshiCoin* create_ObjYoshiCoin()
    {
        return new ObjYoshiCoin();
    }

    inline static ObjYoshiCoinInfo* createObjInfo_ObjYoshiCoinInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjYoshiCoinInfo();
    }
}