#pragma once

namespace app
{
    struct ObjLoftBirdCollisionData
    {
        app::ObjLoftBirdCollisionType ActionType;
    };

    class ObjLoftBirdCollision : public CSetObjectListener
    {
    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCCollider);

            fnd::GOComponent::BeginSetup(this);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                game::ColliBoxShapeCInfo collisionInfo;

                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                collisionInfo.MotionType = 0;
                collisionInfo.Size = Vector3(10, 10, 10);
                collisionInfo.field_04 |= 1;
                ObjUtil::SetupCollisionFilter(2, &collisionInfo);

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
            ObjLoftBirdCollisionData* data = (ObjLoftBirdCollisionData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            xgame::MsgLoftBirdCollision loftMessage { data->ActionType };
            SendMessageImm(message.ActorID, &loftMessage);
        }
    };

    inline static ObjLoftBirdCollision* create_ObjLoftBirdCollision() { return new ObjLoftBirdCollision(); }
}