#pragma once

namespace app
{
    class ObjZeldaItemTreeInfo : public CObjInfo
    {
    public:
        int Model;
        int Skeleton;
        int Animation;

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&Model, "zdlc03_obj_itemtree", &packFile);
            ObjUtil::GetSkeletonResource(&Skeleton, "zdlc03_obj_itemtree", packFile);
            ObjUtil::GetAnimationResource(&Animation, "zdlc03_obj_itemtree", &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjZeldaItemTreeInfo";
        }
    };

    class ObjZeldaItemTree : public CSetObjectListener
    {
        csl::math::Vector3 field_390[4];

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCSound);

            ObjZeldaItemTreeInfo* info = (ObjZeldaItemTreeInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjZeldaItemTreeInfo");

            fnd::GOComponent::BeginSetup(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.Animation |= 0x400000;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
                if (gocAnimation)
                {
                    int animCount = 1;
                    game::GOCAnimationSimple::Setup(gocAnimation, &animCount);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    game::GOCAnimationSimple::Add(gocAnimation, "ACT", info->Animation, 0);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 3;
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                game::ColliSphereShapeCInfo topCollisionInfo;
                csl::math::Vector3 position{ 0, 100, 0 };
                
                game::CollisionObjCInfo::__ct(&topCollisionInfo);
                topCollisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                topCollisionInfo.MotionType = 0;
                topCollisionInfo.Radius = 10;
                topCollisionInfo.field_04 |= 1;
                topCollisionInfo.field_0C = 1;
                ObjUtil::SetupCollisionFilter(6, &topCollisionInfo);
                game::CollisionObjCInfo::SetLocalPosition(&topCollisionInfo, &position);
                game::GOCCollider::CreateShape(gocCollider, &topCollisionInfo);

                game::ColliCylinderShapeCInfo wobbleCollisionInfo;
                position = Vector3(0, 65, 0);

                game::CollisionObjCInfo::__ct(&wobbleCollisionInfo);
                wobbleCollisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_CYLINDER;
                wobbleCollisionInfo.MotionType = 2;
                wobbleCollisionInfo.Radius = 40;
                wobbleCollisionInfo.Height = 40;
                wobbleCollisionInfo.field_04 |= 3;
                wobbleCollisionInfo.field_0C = 2;
                ObjUtil::SetupCollisionFilter(6, &wobbleCollisionInfo);
                game::CollisionObjCInfo::SetLocalPosition(&wobbleCollisionInfo, &position);
                game::GOCCollider::CreateShape(gocCollider, &wobbleCollisionInfo);
                
                game::ColliCapsuleShapeCInfo trunkCollisionInfo;
                position = Vector3(0, 46, 0);

                game::CollisionObjCInfo::__ct(&trunkCollisionInfo);
                trunkCollisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_CAPSULE;
                trunkCollisionInfo.MotionType = 0;
                trunkCollisionInfo.Radius = 4.5f;
                trunkCollisionInfo.Height = 92;
                trunkCollisionInfo.field_04 |= 0x100;
                trunkCollisionInfo.field_08 = 0x2010000;
                game::CollisionObjCInfo::SetLocalPosition(&trunkCollisionInfo, &position);
                game::GOCCollider::CreateShape(gocCollider, &trunkCollisionInfo);
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
            case fnd::PROC_MSG_GET_CLIMB_OBJECT_INFO:
                ProcMsgGetClimbObjectInfo((xgame::MsgGetClimbObjectInfo&)message);
                return true;
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return false;
            case fnd::PROC_MSG_LEAVE_EVENT_COLLISION:
                ProcMsgLeaveEventCollision((xgame::MsgLeaveEventCollision&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

    private:
        void ProcMsgGetClimbObjectInfo(xgame::MsgGetClimbObjectInfo& message)
        {
        }

        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            int someType = ((int*)message.field_18)[0x2F];

            if (someType == 1)
            {
                int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
                if (!gocTransform)
                    return;

                csl::math::Vector3 someVector { 0, 96.5f, 0 };
                math::Vector3Multiply((csl::math::Vector3*)(gocTransform + 0x50), &someVector, &someVector);

                xgame::MsgItemTreeWobblePoint wobbleMessage { &someVector };
                SendMessageImm(message.ActorID, &wobbleMessage);
            }
            else if (someType == 2)
            {
                int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
                if (!gocAnimation)
                    return;

                game::GOCAnimationSimple::SetAnimation(gocAnimation, "ACT");
            }
        }

        void ProcMsgLeaveEventCollision(xgame::MsgLeaveEventCollision& message)
        {
            /*csl::math::Vector3 playerPos{};
            xgame::MsgGetPosition positionMessage { playerPos };
            if (SendMessageImm(message.field_28, &positionMessage))
            {
                int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
                if (!gocTransform)
                    return;
            }*/
        }
    };

    inline static ObjZeldaItemTree* create_ObjZeldaItemTree()
    {
        return new ObjZeldaItemTree();
    }

    inline static ObjZeldaItemTreeInfo* createObjInfo_ObjZeldaItemTreeInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjZeldaItemTreeInfo();
    }
}