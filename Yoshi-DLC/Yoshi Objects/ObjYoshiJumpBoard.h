#pragma once

inline static float YJB_SIZE[] = { 1.25f, 0.75f };
inline static float YJB_WIDTH[] = { 5, 4 };
inline static float YJB_HEIGHT[] = { 4.75f, 4.25f };
inline static float YJB_FIRSTSPEED[] = { 100, 75 };
inline static float YJB_KEEPVELOCITYDISTANCE[] = { 10, 7.5f };
inline static float YJB_OUTOFCONTROL[] = { 0.125f, 0.1f };

namespace app
{
    enum class ObjYoshiJumpBoardState : int
    {
        STATE_WAIT,
        STATE_EXPANSION
    };

    struct ObjYoshiJumpBoardData
    {
        float FirstSpeed;
        float OutOfControl;
        float KeepVelocityDistance;
        bool IsPathChange;
        bool IsChangeCameraWhenPathChange;
        bool IsUpdateYaw;
        float OutOfParkour;
    };

    class ObjYoshiJumpBoardInfo : public CObjInfo
    {
    public:
        int Model;
        int Skeleton;
        animation::AnimationResContainer AnimationContainer{};

        ObjYoshiJumpBoardInfo()
        {
            animation::AnimationResContainer::__ct(&AnimationContainer, pAllocator);
        }

        void Initialize(GameDocument& gameDocument) override
        {
            int animationScript = 0;
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&Model, "zdlc02_obj_jumpboard", &packFile);
            ObjUtil::GetSkeletonResource(&Skeleton, "zdlc02_obj_jumpboard", packFile);
            ObjUtil::GetAnimationScriptResource(&animationScript, "zdlc02_obj_jumpboard", packFile);

            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&AnimationContainer, &animationScript, packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjYoshiJumpBoardInfo";
        }
    };

    class ObjYoshiJumpBoard : public CSetObjectListener
    {
        INSERT_PADDING(16);
        ObjYoshiJumpBoardState State{};
        xgame::MsgHitEventCollision* HitMessage = new xgame::MsgHitEventCollision();
        xgame::MsgGetExternalMovePosition* ExternalMoveMessage = new xgame::MsgGetExternalMovePosition();
        int Type{};
        int Flags{};
        csl::math::Vector3 Position{};
        csl::math::Quaternion Rotation{};
        float OutOfParkour{};
        game::GOCLauncher::ShotInfo* DefaultShot = new game::GOCLauncher::ShotInfo();
        game::GOCLauncher::ShotInfo* JumpShot = new game::GOCLauncher::ShotInfo();

    public:
        ObjYoshiJumpBoard(int type)
        {
            State = ObjYoshiJumpBoardState::STATE_WAIT;
            Type = type;
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationScript);
            fnd::GOComponent::Create(this, GOCCollider);

            if (!IsConsistentShadow)
                fnd::GOComponent::Create(this, GOCShadowSimple);

            fnd::GOComponent::Create(this, GOCSound);

            ObjYoshiJumpBoardInfo* info = (ObjYoshiJumpBoardInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjYoshiJumpBoardInfo");
            ObjYoshiJumpBoardData* data = (ObjYoshiJumpBoardData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            if (data->IsPathChange)
                Flags |= 1;
            if (data->IsChangeCameraWhenPathChange)
                Flags |= 2;
            if (data->IsUpdateYaw)
                Flags |= 4;
            OutOfParkour = data->OutOfParkour;

            fnd::GOComponent::BeginSetup(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;
                csl::math::Vector3 objectScale
                    { YJB_SIZE[Type], YJB_SIZE[Type], YJB_SIZE[Type] };

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.Animation |= 0x400000;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &objectScale);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
                if (gocAnimation)
                {
                    animation::AnimationResContainer* animation = &(info->AnimationContainer);

                    game::GOCAnimationScript::Setup(gocAnimation, &animation);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                csl::math::Vector3 position{};
                csl::math::Vector3 upVec { 0, 1, 0 };
                math::Vector3Scale(&upVec, (YJB_HEIGHT[Type] * 0.5f) + YJB_WIDTH[Type], &position);

                int shapeCount = 1;
                game::ColliCapsuleShapeCInfo collisionInfo;

                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_CAPSULE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = YJB_HEIGHT[Type];
                collisionInfo.Height = YJB_WIDTH[Type];
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &position);
                ObjUtil::SetupCollisionFilter(0, &collisionInfo);
                collisionInfo.field_04 |= 1;
                collisionInfo.field_0C = 0;

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                game::ShadowSphereShapeCInfo shadowInfo;

                game::ShadowSphereShapeCInfo::__ct(&shadowInfo, 5 * YJB_SIZE[Type]);
                shadowInfo.field_04 = 6;
                shadowInfo.ShadowQualityType = 0;

                game::ShadowSphereShapeCInfo* ppShadowInfo = &shadowInfo;
                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
            }

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
            {
                game::GOCLauncher::ShotInfo shotInfo;

                SetShotInfo(data->FirstSpeed, data->KeepVelocityDistance, data->OutOfControl,
                    (csl::math::Matrix34*)(gocTransform + 0xC), Type, &shotInfo);
                *DefaultShot = shotInfo;

                SetShotInfo(YJB_FIRSTSPEED[Type], YJB_KEEPVELOCITYDISTANCE[Type], YJB_OUTOFCONTROL[Type],
                    (csl::math::Matrix34*)(gocTransform + 0xC), Type, &shotInfo);
                *JumpShot = shotInfo;
            }

            game::GOCSound::SimpleSetup(this, 0, 0);

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
            case fnd::PROC_MSG_GET_EXTERNAL_MOVE_POSITION:
                ProcMsgGetExternalMovePosition((xgame::MsgGetExternalMovePosition&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            StateWait();
            StateExpansion();
        }

        inline static ObjYoshiJumpBoard* Create_Small()
        {
            return new ObjYoshiJumpBoard(1);
        }

        inline static ObjYoshiJumpBoard* Create_Big()
        {
            return new ObjYoshiJumpBoard(0);
        }

    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            *HitMessage = message;
        }

        void ProcMsgGetExternalMovePosition(xgame::MsgGetExternalMovePosition& message)
        {
            *ExternalMoveMessage = message;
        }

        void StateWait()
        {
            if (State != ObjYoshiJumpBoardState::STATE_WAIT)
                return;

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], HitMessage->ActorID);
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
            if (!playerInfo)
                return;

            DefaultShot->StartingPosition = *(csl::math::Vector3*)(playerInfo + 4);
            JumpShot->StartingPosition = *(csl::math::Vector3*)(playerInfo + 4);

            xgame::MsgCatchPlayer catchMessage { 18 };
            if (ObjUtil::SendMessageImmToPlayer(this, playerNo, &catchMessage))
            {
                State = ObjYoshiJumpBoardState::STATE_EXPANSION;
                Flags |= 0x1000;
            }
        }

        void StateExpansion()
        {
            if (State != ObjYoshiJumpBoardState::STATE_EXPANSION)
                return;

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], ExternalMoveMessage->field_08);
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
            if (!playerInfo)
                return;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
            if (!gocAnimation)
                return;

            if (Flags & 0x1000)
            {
                game::GOCAnimationScript::SetAnimation(gocAnimation, "JUMP_SMALL");

                int deviceTag[3];
                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (!gocSound)
                    return;
                game::GOCSound::Play(gocSound, deviceTag, "obj_yossyjumpboard", 0);

                Flags &= ~0x1000;
                Flags |= 0x2000;
                return;
            }

            int currentFrame = game::GOCAnimationScript::GetFrame(gocAnimation);
            xgame::MsgPLGetInputButton buttonMessage{};
            if (currentFrame < 10.0 && (Flags & 0x2000))
            {
                if (ObjUtil::SendMessageImmToPlayer(this, playerNo, &buttonMessage))
                    if (buttonMessage.IsPressed && !(Flags & 0x100))
                    {
                        Flags |= 0x100;
                        game::GOCAnimationScript::ChangeAnimation(gocAnimation, "JUMP_BIG");
                    }

                csl::math::Vector3 playerPosition = *(csl::math::Vector3*)(playerInfo + 4);
                csl::math::Vector3 targetPosition = *(csl::math::Vector3*)(gocTransform + 0x50);

                if ((std::abs(playerPosition.Y - targetPosition.Y)) > 0.55f)
                    if (game::GOCAnimationScript::GetFrame(gocAnimation) < 8)
                        playerPosition.Y -= 1.25f;

                if (game::GOCAnimationScript::GetFrame(gocAnimation) >= 9)
                    playerPosition.Y += 5.0f;

                ExternalMoveMessage->Transform->data[3][0] = playerPosition.X;
                ExternalMoveMessage->Transform->data[3][1] = playerPosition.Y;
                ExternalMoveMessage->Transform->data[3][2] = playerPosition.Z;
            }
            else
            {
                Flags &= ~0x3000;
                Flags |= 0x4000;
            }

            if (Flags & 0x4000)
            {
                game::GOCLauncher::ShotInfo* shotInfo =
                    (Flags & 0x100) ? DefaultShot : JumpShot;

                ObjYoshiJumpBoardData* data = (ObjYoshiJumpBoardData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
                xgame::MsgSpringImpulse impulseMessage { shotInfo, data->OutOfParkour };
                if (ObjUtil::SendMessageImmToPlayer(this, playerNo, &impulseMessage))
                {
                    State = ObjYoshiJumpBoardState::STATE_WAIT;
                    Flags &= ~0x7100;
                    HitMessage = new xgame::MsgHitEventCollision();
                    ExternalMoveMessage = new xgame::MsgGetExternalMovePosition();
                    return;
                }
            }
        }
    };

    inline static ObjYoshiJumpBoardInfo* createObjInfo_ObjYoshiJumpBoard(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjYoshiJumpBoardInfo();
    }
}