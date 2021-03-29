#pragma once

namespace app
{
    struct ObjRotateLiftData
    {
        float Omega;
        float Offset;
        bool UseGlobalTime;
        bool IsEventDriven;
    };

    class ObjRotateLiftInfo : public CObjInfo
    {
    public:
        int StepModel{};
        int ChainModel{};

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&StepModel, "zdlc02_obj_rollinglift_step", &packFile);
            ObjUtil::GetModelResource(&ChainModel, "zdlc02_obj_rollinglift", &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjRotateLiftInfo";
        }
    };

    class ObjRotateLift : public CSetObjectListener
    {
    public:
        fnd::HFrame Children[4];
        INSERT_PADDING(16);
        char StepCount;
        bool UseGlobalTime;
        INSERT_PADDING(12);

        ObjRotateLift()
        {
            for (fnd::HFrame& pointer : Children)
                fnd::HFrame::__ct(&pointer);

            StepCount = 4;
            UseGlobalTime = false;

            GameObject::SetObjectCategory(this, 4);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualContainer);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCMotorRotate);

            ObjRotateLiftInfo* info = (ObjRotateLiftInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjRotateLiftInfo");
            ObjRotateLiftData* data = (ObjRotateLiftData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            UseGlobalTime = data->IsEventDriven != 0;

            fnd::GOComponent::BeginSetup(this);

            fnd::HFrame* transformFrame = (fnd::HFrame*)(GameObject::GetGOC(this, GOCTransformString) + 0x28);

            csl::math::Quaternion rotation;
            CSetAdapter::GetRotation(*(int**)((char*)this + 0x324), &rotation);

            float distanceFromOrigin = 25.0f;
            csl::math::Vector3 positionArray[4] = {
                csl::math::Vector3(0, distanceFromOrigin, 0),
                csl::math::Vector3(0, -distanceFromOrigin, 0),
                csl::math::Vector3(0, 0, distanceFromOrigin),
                csl::math::Vector3(0, 0, -distanceFromOrigin)
            };
            for (size_t i = 0; i < 4; i++)
            {
                fnd::HFrame::SetLocalTranslation(&Children[i], &positionArray[i]);
                fnd::HFrame::SetLocalRotation(&Children[i], &rotation);
                fnd::HFrame::ResetFlag(&Children[i], 0x20);
                fnd::HFrame::AddChild(transformFrame, &Children[i]);
            }

            int modelCount = StepCount + 1;

            int* gocContainer = GameObject::GetGOC(this, GOCVisual);
            if (gocContainer)
            {
                fnd::GOCVisualContainer::Setup(gocContainer, &modelCount);

                for (size_t i = 0; i < modelCount; i++)
                {
                    int* gocVisual = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
                    if (gocVisual)
                    {
                        fnd::GOCVisualModel::VisualDescription visualDescriptor{};

                        fnd::GOCVisualContainer::Add(gocContainer, gocVisual);
                        fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                        if (i < 4)
                        {
                            visualDescriptor.Model = info->StepModel;
                            visualDescriptor.Parent = &Children[i];
                        }

                        if (i == 4)
                        {
                            visualDescriptor.Model = info->ChainModel;
                            csl::math::Quaternion chainRot{ 0, 0.707f, 0, 0.707f };
                            fnd::GOCVisualTransformed::SetLocalRotation(gocVisual, &chainRot);
                        }

                        fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                    }
                }
            }

            game::GOCSound::SimpleSetup(this, 1, 0);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = StepCount;
                game::GOCCollider::Setup(gocCollider, &shapeCount);
                for (size_t i = 0; i < StepCount; i++)
                {
                    game::ColliBoxShapeCInfo collisionInfo{};

                    game::CollisionObjCInfo::__ct(&collisionInfo);
                    collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                    collisionInfo.MotionType = 2;
                    collisionInfo.Size = csl::math::Vector3(1, 2.5f, 8);
                    collisionInfo.field_08 = 0x4003;
                    collisionInfo.field_04 |= 0x100;
                    collisionInfo.field_02 = 14;
                    collisionInfo.Parent = &Children[i];
                    game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
                }
            }

            int* gocMotor = GameObject::GetGOC(this, GOCMotorString);
            if (gocMotor)
            {
                game::MotorInfo motorInfo{};
                motorInfo.someVector.X = 1;
                motorInfo.Speed = data->Omega;
                game::GOCMotorRotate::Setup(gocMotor, &motorInfo);

                if (UseGlobalTime)
                    game::GOCMotor::RequestEnable(gocMotor, 0);
            }

            fnd::GOComponent::EndSetup(this);
            
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (gocSound)
            {
                int deviceTag[3]{};
                game::GOCSound::Play3D(gocSound, deviceTag, "obj_yossyrollinglift_rotate", 0);
            }
        }

        bool ProcessMessage(fnd::MessageNew& message) override
        {
            if (PreProcessMessage(message))
                return true;

            if (message.Type != fnd::PROC_MSG_HIT_EVENT_COLLISION)
                return CSetObjectListener::ProcessMessage(message);

            ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
            return true;
        }
    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            if (!UseGlobalTime)
                return;

            int* gocMotor = GameObject::GetGOC(this, GOCMotorString);
            if (!gocMotor)
                return;

            if (message.field_18 == 1)
                game::GOCMotor::RequestEnable(gocMotor, true);
            else if (message.field_18 == 2)
                game::GOCMotor::RequestEnable(gocMotor, false);
        }
    };

    inline static ObjRotateLift* create_ObjRotateLift() { return new ObjRotateLift();  }

    inline static ObjRotateLiftInfo* createObjInfo_ObjRotateLiftInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjRotateLiftInfo();
    }
}