#pragma once

namespace app
{
    inline static size_t ZELDA_GOAL_BGM_IDS[3] = { 0, 1, 2 };
    inline static float ZELDA_GOAL_BGM_VALUES[3] = { 0.2f, 0, 0 };
    inline static const char* ZELDA_GOAL_NODE_NAMES[3] = { "power", "courage", "wisdom" };

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
        res::ResShadowModel ShadowModel{0};

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

            ObjUtil::GetShadowModel(&ShadowModel, "zdlc03_obj_triforce", packFile);
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
                if (!pZeldaGoal)
                    return;
            
                if (field_2C)
                {
                    pZeldaGoal->CreatePointLights();
                    field_2C = 0;
                }
            }
        };

        INSERT_PADDING(16); // TinyFsm
        ObjZeldaGoalState State{};
        int CameraID{};
        int field_3B8{};
        int field_3BC{};
        csl::math::Matrix34 field_3C0{};
        int PlayerNumber{};
        float Time{};
        bool DoCreateModel{};
        char field_409{};
        char field_40A{};
        char field_40B{};
        Listener AnimationListener{};
        fnd::HandleBase PointLights[3]{};
        int field_454{};
        int field_458{};
        int field_45C{};
        int IsSoundPlaying{};
        int SoundHandle[3]{};

        ObjZeldaGoal()
        {
            AnimationListener.field_20 = 2;
            AnimationListener.field_2C = 0;
        }

        void Destructor(size_t deletingFlags)
        {
            AnimationListener.Destructor(0);

            CSetObjectListener::Destructor(deletingFlags);
        }

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

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                game::ShadowModelShapeCInfo shadowInfo;

                game::ShadowModelShapeCInfo::__ct2(&shadowInfo, gocVisual, &info->ShadowModel, 50);
                shadowInfo.ShadowQualityType = 4;

                game::ShadowModelShapeCInfo* ppShadowInfo = &shadowInfo;
                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
                game::GOCShadowSimple::SetVisible(gocShadow, 0);
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
            if (State == ObjZeldaGoalState::STATE_WAIT_END_EVENT)
                StateWaitEndEvent(updateInfo);

            if (State == ObjZeldaGoalState::STATE_WHITE_OUT)
                StateWhiteOut();

            if (State == ObjZeldaGoalState::STATE_FADE_IN)
                StateFadeIn(updateInfo);
        }

    private:
        void ProcMsgGetExternalMovePosition(xgame::MsgGetExternalMovePosition& message)
        {
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    message.Transform->data[i][j] = field_3C0.data[i][j];
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
            {*/
                xgame::MsgStopBgm stopBgmMessage{ ZELDA_GOAL_BGM_VALUES[0], ZELDA_GOAL_BGM_IDS[0] };
                ObjUtil::SendMessageImmToGameActor(this, &stopBgmMessage);
            /*}*/

            State = ObjZeldaGoalState::STATE_WAIT_END_EVENT;
            DoCreateModel = true;
        }

        void StateWaitEndEvent(const fnd::SUpdateInfo& updateInfo)
        {
            if (DoCreateModel)
            {
                int* gocVisual = GameObject::GetGOC(this, GOCVisual);
                if (!gocVisual)
                    return;

                int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
                if (!gocShadow)
                    return;

                fnd::GOCVisual::SetVisible(gocVisual, true);
                game::GOCShadowSimple::SetVisible(gocShadow, true);
                DoCreateModel = false;
            }

            int deviceTag[3]{};
            void* soundPlayer = *(void**)ASLR(0x00FD3CB8);
            if (!soundPlayer)
                return;

            if (!IsSoundPlaying)
            {
                fnd::SoundParam soundParam{ 1, 0, 0, 0x80000000, 0, 0, 0 };
                fnd::SoundPlayerCri::Play(soundPlayer, deviceTag, 0, "bgm_result_zdlc03", soundParam);
                fnd::HandleBase::__as(SoundHandle, deviceTag);
                SoundHandle[2] = deviceTag[2];
            }
            IsSoundPlaying = true;

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            if (game::GOCAnimationScript::IsFinished(gocAnimation))
            {
                State = ObjZeldaGoalState::STATE_WHITE_OUT;
                GameMode::WhiteOut(1, 1);
            }
        }

        void StateWhiteOut()
        {
            if (!GameMode::IsFadeFinished(3))
                return;

            State = ObjZeldaGoalState::STATE_FADE_IN;
            GameMode::FadeIn(1, 1);
            Time = 0;
        }

        void StateFadeIn(const fnd::SUpdateInfo& updateInfo)
        {
            if (!GameMode::IsFadeFinished(3))
                return;

            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], 1);
            if (playerInfo)
            {
                xgame::MsgGoalForBattle goalMessage{ PlayerNumber };
                SendMessageImm(*(int*)(field_24[1] + 0x10), &goalMessage);
            }
            else
            {
                xgame::MsgPlayerReachGoal goalMessage{};
                ObjUtil::SendMessageImmToPlayer(this, PlayerNumber, &goalMessage); 
            }

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            game::GOCAnimationScript::SetAnimation(gocAnimation, "TRIFORCE_RESULT");

            if (!playerInfo)
            {
                State = ObjZeldaGoalState::STATE_IDLE;
                return;
            }

            Time += updateInfo.deltaTime;
            if (Time > 3.35f)
            {
                int* gocVisual = GameObject::GetGOC(this, GOCVisual);
                if (!gocVisual)
                    return;

                xgame::MsgCameraOn cameraMessage{ 0, 3001, 1, PlayerNumber ^ 1, 0, 0 };
                ObjUtil::SendMessageImmToSetObject(this, &CameraID, &cameraMessage, 0);

                fnd::GOCVisual::SetVisible(gocVisual, false);
                State = ObjZeldaGoalState::STATE_IDLE;
            }
        }

        void CreatePointLights()
        {
            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (!gocVisual)
                return;

            for (size_t i = 0; i < 3; i++)
            {
                math::Transform transform{};
                fnd::GOCVisualModel::GetNodeTransform(gocVisual, 1, ZELDA_GOAL_NODE_NAMES[i], &transform);

                ObjectPartPointLight::CInfo lightInfo
                {
                    transform.Position,
                    25,
                    1,
                    0.7f,
                    0.6f,
                    0.4f,
                    -1,
                    gocTransform,
                    1
                };

                ObjectPartPointLight* lightObject = ObjectPartPointLight::Create((GameDocument*)field_24[1], &lightInfo);
                fnd::HandleBase::__as(&PointLights[i], lightObject);
            }
        }

        inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new animation::AnimCallbackBridge<ObjZeldaGoal>();
        }
    };

    inline static ObjZeldaGoal* create_ObjZeldaGoal() { return new ObjZeldaGoal(); }

    inline static ObjZeldaGoalInfo* createObjInfo_ObjZeldaGoalInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjZeldaGoalInfo();
    }
}