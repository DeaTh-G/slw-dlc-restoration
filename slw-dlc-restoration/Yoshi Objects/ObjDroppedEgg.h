#pragma once

namespace app
{
    struct DroppedEggCInfo;
    class ObjDroppedEgg;
    namespace egg
    {
        ObjDroppedEgg* CreateDroppedEgg(GameDocument& gameDocument, DroppedEggCInfo* cInfo);
    }

    struct DroppedEggCInfo
    {
        csl::math::Matrix34 Transform;
        csl::math::Vector3 field_40;
        game::PathEvaluator PathEvaluator;
        int ModelType;
        int ZIndex;
        int field_68;
        int field_6C;
    };

    class ObjDroppedEgg : public GameObject3D
    {
        typedef enum DropState
        {
            STATE_FALL,
            STATE_LANDING,
            STATE_WAIT
        };

    private:
        bool ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            EggCInfo cInfo{};
            GameDocument& document = (GameDocument&)field_24[1];

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
                cInfo.Transform = (csl::math::Matrix34*)(gocTransform + 0x44);

            cInfo.PlayerNo = ObjUtil::GetPlayerNo(field_24[1], message.ActorID);
            cInfo.ModelType = ModelType;
            egg::CreateEgg(document, &cInfo);

            return Kill();
        }

        void UpdateFollow()
        {
            if (!fnd::HandleBase::IsValid(&Handle))
                return;

            int* handle = fnd::Handle::Get(&Handle);
            if (!handle)
                return;

            csl::math::Vector3 origTranslation{};
            csl::math::Vector3 translation{};
            origTranslation = *(csl::math::Vector3*)(handle + 0xC);
            math::Vector3Subtract(&origTranslation, &field_360, &translation);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Vector3 objectPosition{};
            math::CalculatedTransform::GetTranslation((csl::math::Matrix34*)(gocTransform + 0x44), &objectPosition);
            math::Vector3Add(&objectPosition, &translation, &objectPosition);
            fnd::GOCTransform::SetLocalTranslation(gocTransform, &objectPosition);
            field_360 = origTranslation;
        }

        void StateFall()
        {
            if (Movement->field_80 & 1)
            {
                game::MoveController::ResetFlag(Movement, 1);
                State = STATE_LANDING;
            }
        }

        void StateLanding(const fnd::SUpdateInfo& updateInfo)
        {
            csl::math::Vector3 defaultScale { 1, 1, 1 };
            csl::math::Vector3 scale{};

            UpdateFollow();

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (!gocVisual)
                return;

            if (Time >= 0.35f)
            {
                fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &defaultScale);
                if (Time > 0.55f)
                {
                    Time = 0;
                    State = STATE_WAIT;
                }
            }
            else
            {
                scale = egg::CalcSlipperyScale(Time, 0.7f, 0.4f, 0.2f);
                fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
            }
            Time += updateInfo.deltaTime;
        }

        void StateWait(const fnd::SUpdateInfo& updateInfo)
        {
            csl::math::Vector3 scale;

            Time += updateInfo.deltaTime;
            UpdateFollow();

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (!gocVisual)
                return;
        
            scale = egg::CalcSlipperyScale(Time, 1.6f, 0.1f, 0.1f);
            fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
        }
    
    public:
        DropState State;
        INSERT_PADDING(0x4);
        INSERT_PADDING(0x14);	// TinyFSM
        DroppedEggCInfo* CInfo{};
        int ModelType{};
        float Time{};
        game::MoveBound* Movement = nullptr;
        INSERT_PADDING(0x4);	// BoundListener
        ObjDroppedEgg* Instance;
        fnd::HandleBase Handle{};
        INSERT_PADDING(0xC);
        csl::math::Vector3 field_360{};

        ObjDroppedEgg(GameDocument& document, DroppedEggCInfo* cInfo)
        {
            CInfo = cInfo;
            ModelType = CInfo->ModelType;
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOCVisualModel::VisualDescription visualDescriptor{};
            game::ColliSphereShapeCInfo collisionInfo{};
            int unit = 1;

            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCMovementComplex);

            fnd::GOComponent::BeginSetup(this);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
            {
                csl::math::Quaternion rotation = GetRotationFromMatrix(&CInfo->Transform);
                fnd::GOCTransform::SetLocalTranslation(gocTransform, (csl::math::Vector3*)&CInfo->Transform.data[3][0]);
                fnd::GOCTransform::SetLocalRotation(gocTransform, &rotation);
            }

            ObjEggInfo* info = (ObjEggInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjEggInfo");

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                csl::math::Vector3 visualOffset{ 0, -3, 0 };

                visualDescriptor.Model = info->Models[ModelType];
                visualDescriptor.Animation |= 0x400000;
                visualDescriptor.ZIndex = -0.2f * CInfo->ZIndex;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &visualOffset);
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                game::GOCCollider::Setup(gocCollider, &unit);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 3;
                ObjUtil::SetupCollisionFilter(8, &collisionInfo);
                collisionInfo.field_04 |= 1;
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (gocMovement)
            {
                void* movementMem = ((app::fnd::ReferencedObject*)gocMovement)->pAllocator->Alloc
            	(sizeof(game::MoveBound), 16);

            	Movement = new(movementMem) game::MoveBound();
                game::GOCMovement::SetupController(gocMovement, Movement);

                game::MoveBound::Description description{};
                description.field_00 = CInfo->field_40;
                description.field_10 = 3;
                description.field_14 = 300;
                description.field_18 = 0.8f;
                description.field_1C = 0.75f;
                description.field_20 = 150;
                description.field_28 = 1;
                description.field_2C = 0.1f;
                game::PathEvaluator::__ct(&description.field_38);
                if (fnd::HandleBase::IsValid((fnd::HandleBase*)&CInfo->PathEvaluator))
                    description.field_38 = CInfo->PathEvaluator;
                
                description.field_48 |= 3;

                game::MoveBound::Setup(Movement, &description);

                State = STATE_FALL;
            }
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            if (message.Type != fnd::PROC_MSG_HIT_EVENT_COLLISION)
                return GameObject::ProcessMessage(message);

            ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
            return true;
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == STATE_FALL)
                StateFall();

            if (State == STATE_LANDING)
                StateLanding(updateInfo);
        
            if (State == STATE_WAIT)
                StateWait(updateInfo);
        }
    };
}

inline static app::ObjDroppedEgg* app::egg::CreateDroppedEgg(GameDocument& gameDocument, DroppedEggCInfo* cInfo)
{
    ObjDroppedEgg* object = new ObjDroppedEgg(gameDocument, cInfo);
    if (!object)
        return 0;
    GameDocument::AddGameObject(*(GameDocument**)&gameDocument, object);
    return object;
}