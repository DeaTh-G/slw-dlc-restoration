#pragma once

namespace app
{
    inline static size_t ZELDA_GOAL_BGM_IDS[3] = { 0, 1, 2 };
    inline static float ZELDA_GOAL_BGM_VALUES[3] = { 0.2f, 0, 0 };

    enum class ObjZeldaGoalState : int
    {
        STATE_IDLE,
        STATE_WAIT_END_EVENT,
        STATE_WHITE_OUT,
        STATE_FADE_IN,
        STATE_END_UP
    };

    struct ObjZeldaGoalData
    {
        float CollisionWidth;
        float CollisionHeight;
        float CollisionDepth;
        int FixCamera;
        float OffsetWidth;
        float OffsetHeight;
        float OffsetDepth;
    };

    class ObjZeldaGoalInfo : public CObjInfo
    {
    public:
        int Model;
        int Skeleton;
        animation::AnimationResContainer AnimationContainer{};
        int ShadowModel[3];

        ObjZeldaGoalInfo()
        {
            animation::AnimationResContainer::__ct(&AnimationContainer, (csl::fnd::IAllocator*)pAllocator);
        }

        void Destructor(size_t deletingFlags) override
        {
            ObjZeldaGoalInfo::~ObjZeldaGoalInfo();
            animation::AnimationResContainer::__dt(&AnimationContainer);
            CObjInfo::Destructor(deletingFlags);
        }

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            int animationScript[3]{};

            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));
            ObjUtil::GetModelResource(&Model, "zdlc03_obj_triforce", &packFile);
            ObjUtil::GetSkeletonResource(&Skeleton, "zdlc03_obj_triforce", packFile);

            animationScript[1] = Skeleton;
            ObjUtil::GetAnimationScriptResource(animationScript, "zdlc03_obj_triforce", packFile);
            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&AnimationContainer, animationScript, packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjZeldaGoalInfo";
        }
    };

    class ObjZeldaGoal : public CSetObjectListener
    {
    public:
        class Listener : public animation::AnimationListener
        {
        public:
            ObjZeldaGoal* pZeldaGoal;
            char field_2C;
            char field_2D;
            char field_2E;
            INSERT_PADDING(1);

            void OnEvent(int notifyTiming) override
            {
            }
        };

        INSERT_PADDING(16); // TinyFsm
        ObjZeldaGoalState State;
        int CameraID;
        int field_3B8;
        int field_3BC;
        csl::math::Matrix34 field_3C0;
        int PlayerNumber;
        float Time;
        bool DoCreateModel;
        char field_409;
        char field_40A;
        char field_40B;
        Listener AnimationListener;
        fnd::HandleBase PointLights[3];
        int field_454;
        int field_458;
        int field_45C;
        int field_460;
        int SoundHandle[3];

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationScript); 
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCShadowSimple);

            ObjZeldaGoalInfo* info = (ObjZeldaGoalInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjZeldaGoalInfo");
            ObjZeldaGoalData* data = (ObjZeldaGoalData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            fnd::GOComponent::BeginSetup(this);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
                field_3C0 = *(csl::math::Matrix34*)(gocTransform + 0x44);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                csl::math::Vector3 scale { 0.9f, 0.9f, 0.9f };
                fnd::GOCVisualModel::VisualDescription visualDescriptor;
                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.Animation |= 0x400000;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
                
                fnd::GOCVisual::SetVisible(gocVisual, false);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
                if (gocAnimation)
                {
                    animation::AnimationResContainer* animation;
                    animation = &(info->AnimationContainer);

                    game::GOCAnimationScript::Setup(gocAnimation, &animation);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);

                    AnimationListener.pZeldaGoal = this;
                    game::GOCAnimationSimple::AddListener(gocAnimation, &AnimationListener);
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                game::ColliBoxShapeCInfo colliderInfo{};
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                csl::math::Vector3 offset { data->OffsetWidth, data->OffsetHeight, data->OffsetDepth };
                csl::math::Vector3 heightOffset { 0, data->CollisionHeight / 2, 0};
                math::Vector3Add(&offset, &heightOffset, &offset);

                game::CollisionObjCInfo::__ct(&colliderInfo);
                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                colliderInfo.MotionType = 2;
                colliderInfo.Size = Vector3(data->CollisionWidth / 2, data->CollisionHeight / 2, data->CollisionDepth / 2);
                ObjUtil::SetupCollisionFilter(6, &colliderInfo);
                game::CollisionObjCInfo::SetLocalPosition(&colliderInfo, &offset);
                colliderInfo.field_04 |= 1;
                game::GOCCollider::CreateShape(gocCollider, &colliderInfo);
            }

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            switch (message.Type)
            {
            case fnd::PROC_MSG_GET_EXTERNAL_MOVE_POSITION:
                ProcMsgGetExternalMovePosition((xgame::MsgGetExternalMovePosition&)message);
                return true;
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == ObjZeldaGoalState::STATE_IDLE)
                StateIdle();

            if (State == ObjZeldaGoalState::STATE_WAIT_END_EVENT)
                StateWaitEndEvent();

            if (State == ObjZeldaGoalState::STATE_WHITE_OUT)
                StateIdle();

            if (State == ObjZeldaGoalState::STATE_FADE_IN)
                StateIdle();

            if (State == ObjZeldaGoalState::STATE_END_UP)
                StateIdle();
        }

    private:
        void ProcMsgGetExternalMovePosition(xgame::MsgGetExternalMovePosition& message)
        {
            message.Transform = &field_3C0;
        }

        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            ObjZeldaGoalData* data = (ObjZeldaGoalData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

            xgame::MsgCatchPlayer catchMessage{ field_3C0, 20 };
            if (!SendMessageImm(message.ActorID, &catchMessage) || !catchMessage.field_64)
                return;

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (!gocCollider)
                return;

            game::GOCCollider::SetEnable(gocCollider, false);
            PlayerNumber = ObjUtil::GetPlayerNo(field_24[1], message.ActorID);
            xgame::MsgCameraOn cameraMessage{ 0, 3001, 1, PlayerNumber, 0, 0 };
            if (ObjUtil::SendMessageImmToSetObject(this, &data->FixCamera, &cameraMessage, 0))
                CameraID = data->FixCamera;

            xgame::MsgStopGameTimer stopTimerMessage{};
            ObjUtil::SendMessageImmToGameActor(this, &stopTimerMessage);

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            game::GOCAnimationScript::SetAnimation(gocAnimation, "TRIFORCE");
            AnimationListener.field_2C = 1;
            /*for (size_t i = 0; i < 4; i++)
            {
                xgame::MsgStopBgm stopBgmMessage{ ZELDA_GOAL_BGM_VALUES[i], ZELDA_GOAL_BGM_IDS[i] };
                ObjUtil::SendMessageImmToGameActor(this, &stopBgmMessage);
            }*/

            State = ObjZeldaGoalState::STATE_WAIT_END_EVENT;
            DoCreateModel = true;
        }

        void StateIdle() { return; }

        void StateWaitEndEvent()
        {
            if (DoCreateModel)
            {
                int* gocVisual = GameObject::GetGOC(this, GOCVisual);
                if (!gocVisual)
                    return;

                fnd::GOCVisual::SetVisible(gocVisual, true);
                DoCreateModel = false;
            }
        }

        void StateWhiteOut()
        {

        }

        void StateFadeIn()
        {

        }

        void StateEndUp()
        {

        }
    };

    inline static ObjZeldaGoal* create_ObjZeldaGoal() { return new ObjZeldaGoal(); }

    inline static ObjZeldaGoalInfo* createObjInfo_ObjZeldaGoalInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjZeldaGoalInfo();
    }
}