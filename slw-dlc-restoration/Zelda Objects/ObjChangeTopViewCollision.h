#pragma once

namespace app
{
    struct ObjChangeTopViewCollisionData
    {
        float CollisionWidth;
        float CollisionHeight;
        float CollisionDepth;
    };

    class ObjChangeTopViewCollision : public CSetObjectListener
    {
    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCCollider);

            ObjChangeTopViewCollisionData* data = (ObjChangeTopViewCollisionData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            
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
                collisionInfo.field_04 |= 3;
                ObjUtil::SetupCollisionFilter(7, &collisionInfo);

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

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
                ProcMsgLeaveEventCollision((xgame::MsgLeaveEventCollision&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            int playerNo = ObjUtil::GetPlayerNo(field_24[1], message.ActorID);

            xgame::MsgPLChangeAreaTopView topViewMessage { true };
            ObjUtil::SendMessageImmToPlayer(this, playerNo, &topViewMessage);
        }

        void ProcMsgLeaveEventCollision(xgame::MsgLeaveEventCollision& message)
        {
            int playerNo = ObjUtil::GetPlayerNo(field_24[1], message.ActorID);

            xgame::MsgPLChangeAreaTopView topViewMessage{ false };
            ObjUtil::SendMessageImmToPlayer(this, playerNo, &topViewMessage);
        }
    };

    inline static ObjChangeTopViewCollision* create_ObjChangeTopViewCollision() { return new ObjChangeTopViewCollision(); }
}