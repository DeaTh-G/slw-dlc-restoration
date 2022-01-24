#pragma once

namespace app
{
    struct ObjEggRotationChangeCollisionData
    {
        float Width;
        float Height;
        float Depth;
    };

    class ObjEggRotationChangeCollision : public CSetObjectListener
    {
    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            xgame::MsgDlcChangeEggRotation msg{};

            SendMessageImm(message.ActorID, &msg);
        }

        void ProcMsgLeaveEventCollision(xgame::MsgLeaveEventCollision& message)
        {
            xgame::MsgDlcChangeEggRotation msg{};

            SendMessageImm(message.ActorID, &msg);
        }

    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::BeginSetup(this);
            ObjEggRotationChangeCollisionData* data = (ObjEggRotationChangeCollisionData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            int shapeCount = 1;
            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                game::ColliBoxShapeCInfo collisionInfo;
                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                collisionInfo.MotionType = 2;
                collisionInfo.field_44 = 0;
                collisionInfo.field_48 = 0;
                collisionInfo.Size = Vector3(data->Width, data->Height, data->Depth);
                collisionInfo.field_04 |= 3;
                ObjUtil::SetupCollisionFilter(2, &collisionInfo);
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
            case fnd::PROC_MSG_LEAVE_EVENT_COLLISION:
                ProcMsgLeaveEventCollision((xgame::MsgLeaveEventCollision&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }
    };

    inline static ObjEggRotationChangeCollision* create_ObjEggRotationChangeCollision()
    {
        return new ObjEggRotationChangeCollision();
    }
}