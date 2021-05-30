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
        int PlayerNumber{};
        fnd::MessageType MessageType{};
        xgame::MsgHitEventCollision* HitMessage = new xgame::MsgHitEventCollision();
        xgame::MsgDamage* DamageMessage = new xgame::MsgDamage();
        ObjGossipStoneState State{};
        GameObjectHandleBase ObjectHandle;
        INSERT_PADDING(8); // CInstanceHandle
        INSERT_PADDING(4);
        bool field_3C8;
        INSERT_PADDING(3);
        float field_3CC;
        INSERT_PADDING(4);
        float field_3D4;
        INSERT_PADDING(8);

        void Destructor(size_t deletingFlags)
        {
            delete HitMessage;
            delete DamageMessage;

            CSetObjectListener::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCCollider);

            ObjGossipStoneInfo* info = (ObjGossipStoneInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjGossipStoneInfo");
            ObjGossipStoneData* data = (ObjGossipStoneData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            fnd::GOComponent::BeginSetup(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->BaseModel;
                visualDescriptor.Skeleton = info->BaseSkeleton;

                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
                if (gocAnimation)
                {
                    int animCount = 1;
                    game::GOCAnimationSimple::Setup(gocAnimation, &animCount);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    game::GOCAnimationSimple::Add(gocAnimation, "ACTION", info->BaseAnimation, 0);
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 3;

                game::GOCCollider::Setup(gocCollider, &shapeCount);

                game::ColliSphereShapeCInfo checkCollisionInfo;
                game::CollisionObjCInfo::__ct(&checkCollisionInfo);
                checkCollisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                checkCollisionInfo.MotionType = 2;
                checkCollisionInfo.Radius = data->CheckRadius;
                checkCollisionInfo.field_04 |= 1;
                checkCollisionInfo.field_0C = 0;
                ObjUtil::SetupCollisionFilter(7, &checkCollisionInfo);
                game::GOCCollider::CreateShape(gocCollider, &checkCollisionInfo);

                game::ColliCapsuleShapeCInfo collisionInfo;
                csl::math::Vector3 colliderOffset { 0, 5, 0 };
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_CAPSULE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 8;
                collisionInfo.Height = 10;
                collisionInfo.field_04 |= 1;
                collisionInfo.field_0C = 1;
                ObjUtil::SetupCollisionFilter(0, &checkCollisionInfo);
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &colliderOffset);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

                colliderOffset = Vector3(0, 4, 0);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_CAPSULE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 6;
                collisionInfo.Height = 8;
                collisionInfo.field_04 |= 0x100;
                collisionInfo.field_08 = 0x40053;
                collisionInfo.field_0C = 2;
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &colliderOffset);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            MessageType = message.Type;
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
            if (State == ObjGossipStoneState::STATE_IDLE)
                StateIdle();

            if (State == ObjGossipStoneState::STATE_SHAKE)
                StateShake();

            if (State == ObjGossipStoneState::STATE_COUNTDOWN)
                StateCountdown();

            if (State == ObjGossipStoneState::STATE_ROCKET)
                StateRocket();

            if (State == ObjGossipStoneState::STATE_LOST)
                StateLost();
        }

    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            if (ObjUtil::CheckShapeUserID(message.field_18, 0))
            {
                OnSnapShot();
                return;
            }
            
            if (!ObjUtil::CheckShapeUserID(message.field_18, 1))
                return;

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], message.ActorID);
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
            if (!playerInfo)
                return;

            if (((char*)playerInfo)[0x144])
                return;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;
            
            csl::math::Vector3 yVector = *(csl::math::Vector3*)(gocTransform + 0x48);
            csl::math::Vector3 contactPoint;
            message.GetContactPointNormal(&contactPoint);
            math::Vector3Scale(&contactPoint, -1, &contactPoint);
            float dot = math::Vector3DotProduct(&yVector, &contactPoint);
            math::Vector3Scale(&yVector, dot, &yVector);
            math::Vector3Subtract(&contactPoint, &yVector, &contactPoint);
            math::Vector3NormalizeZero(&contactPoint, &contactPoint);
            math::Vector3Scale(&contactPoint, 30, &contactPoint);
            math::Vector3Scale(&contactPoint, 2, &contactPoint);
            math::Vector3Add(&yVector, &contactPoint, &contactPoint);

            xgame::MsgAppeareKnockback knockbackMessage { contactPoint };
            SendMessageImm(message.ActorID, &knockbackMessage);
            *HitMessage = message;
        }

        void ProcMsgDamage(xgame::MsgDamage& message)
        {
            *DamageMessage = message;
        }

        void StateIdle()
        {
            if (MessageType != fnd::PROC_MSG_DAMAGE)
            {
                if (MessageType != fnd::PROC_MSG_HIT_EVENT_COLLISION)
                    return;

                /*State = ObjGossipStoneState::STATE_SHAKE;
                return;*/
            }

            if (DamageMessage->field_54 == 3)
            {
                return;
            }
        }

        void StateShake()
        {

        }

        void StateCountdown()
        {

        }

        void StateRocket()
        {

        }

        void StateLost()
        {

        }

        void OnSnapShot()
        {
            if (ObjectHandle.IsValid())
                return;
        
            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            ObjGossipStoneFairy::CInfo* fairyInfo = new ObjGossipStoneFairy::CInfo(*(csl::math::Matrix34*)(gocTransform + 0x44));
            ObjGossipStoneFairy* fairy = ObjGossipStoneFairy::Create(*(GameDocument*)field_24[1], *fairyInfo);
            if (fairy)
                ObjectHandle = fairy;
            
            xgame::MsgDlcZeldaHeartAllRecovery recoverMessage{};
            SendMessageImm((int)HitMessage, &recoverMessage);

            int deviceTag[3];
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play3D(gocSound, deviceTag, "obj_gossipstone_check", 0);
        }
    };

    inline static ObjGossipStone* create_ObjGossipStone() { return new ObjGossipStone(); }

    inline static ObjGossipStoneInfo* createObjInfo_ObjGossipStoneInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjGossipStoneInfo();
    }
}