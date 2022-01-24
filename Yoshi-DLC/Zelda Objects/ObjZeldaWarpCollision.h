#pragma once

namespace app
{
    struct ObjZeldaWarpCollisionData
    {
        int TargetLocate;
        bool IsSideView;
        INSERT_PADDING(3);
        float CollisionWidth;
        float CollisionHeight;
        float CollisionDepth;
    };

    class ObjZeldaWarpCollision : public CSetObjectListener
    {
    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCCollider);

            ObjZeldaWarpCollisionData* data = (ObjZeldaWarpCollisionData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            fnd::GOComponent::BeginSetup(this);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                game::ColliBoxShapeCInfo collisionInfo;

                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                collisionInfo.MotionType = 2;
                collisionInfo.Size = Vector3(data->CollisionWidth, data->CollisionHeight, data->CollisionDepth);
                collisionInfo.field_04 = 1;
                ObjUtil::SetupCollisionFilter(7, &collisionInfo);

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            if (message.Type != fnd::PROC_MSG_HIT_EVENT_COLLISION)
                return CSetObjectListener::ProcessMessage(message);

            ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
            return 1;
        }

    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            ObjZeldaWarpCollisionData* data = (ObjZeldaWarpCollisionData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            csl::math::Vector3 targetPosition;
            csl::math::Quaternion targetRotation;
            if (!ObjUtil::GetSetObjectTransform(*Document, &data->TargetLocate,
                &targetPosition, &targetRotation))
                return;

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], message.ActorID);
            xgame::MsgWarpNewArea warpMessage{ playerNo, data->IsSideView, targetPosition, targetRotation, 0, 1 };
            ObjUtil::SendMessageImmToGameActor(this, &warpMessage);
        }
    };

    inline static ObjZeldaWarpCollision* create_ObjZeldaWarpCollision()
    {
        return new ObjZeldaWarpCollision();
    }
}