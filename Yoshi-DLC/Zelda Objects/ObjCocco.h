#pragma once

namespace app
{
    enum class ObjCoccoState : int
    {
        STATE_IDLE,
        STATE_ATTACK_IN,
        STATE_ATTACK_OUT,
        STATE_END
    };

    class ObjCoccoInfo : public CObjInfo
    {
    public:
        int Model;
        int Skeleton;
        animation::AnimationResContainer AnimationContainer{};

        ObjCoccoInfo()
        {
            animation::AnimationResContainer::__ct(&AnimationContainer, (csl::fnd::IAllocator*)pAllocator);
        }

        void Destructor(size_t deletingFlags) override
        {
            ObjCoccoInfo::~ObjCoccoInfo();
            animation::AnimationResContainer::__dt(&AnimationContainer);
            CObjInfo::Destructor(deletingFlags);
        }

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            int animationScript[3]{};
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&Model, "zdlc03_obj_cocco", &packFile);
            ObjUtil::GetSkeletonResource(&Skeleton, "zdlc03_obj_cocco", packFile);

            ObjUtil::GetAnimationScriptResource(&animationScript, "cocco", packFile);
            animationScript[1] = Skeleton;

            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&AnimationContainer, animationScript, packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjCoccoInfo";
        }
    };

    class ObjCocco : public CSetObjectListener
    {
    private:
        class CInfo
        {
        public:
            csl::math::Vector3 Position{};
            csl::math::Quaternion Rotation{};
            int field_20;
            int field_24;
        };

    public:
        enum class ActionType : char
        {

        };

        ObjCoccoState State{};
        xgame::MsgHitEventCollision* HitMessage = new xgame::MsgHitEventCollision();
        xgame::MsgDamage* DamageMessage = new xgame::MsgDamage();
        xgame::MsgKick* KickMessage = new xgame::MsgKick();
        INSERT_PADDING(4); // TinyFSM
        int field_3B4{};
        int field_3B8{};
        int field_3BC{};
        csl::math::Vector3 Position{};
        csl::math::Quaternion Rotation{};
        int Spawner{};
        int field_3E4{};
        int field_3E8{};
        int field_3EC{};
        ActionType ActionType{};
        INSERT_PADDING(3);
        MoveObjCocco* MovementController{};
        std::vector<ObjCocco*> SubCoccos{};
        float field_408{};
        float field_40C{};
        float field_410{};
        float Time{};
        float CryTime{};
        int field_41C{};
        char Flags{};
        INSERT_PADDING(3);
        int field_424{};
        int field_428{};
        int field_42C{};

        ObjCocco()
        {
            field_41C = 3;
            ObjUtil::SetPropertyLockonTarget(this);
        }

        ObjCocco(const CInfo& createInfo)
        {
            State = ObjCoccoState::STATE_ATTACK_IN;

            Position = createInfo.Position;
            Rotation = createInfo.Rotation;
            field_3E4 = createInfo.field_24;
            Spawner = createInfo.field_20;

            field_41C = 3;
        };

        void Destructor(size_t deletingFlags) override
        {
            delete HitMessage;
            delete DamageMessage;
            delete KickMessage;

            CSetObjectListener::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationScript);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCShadowSimple);
            fnd::GOComponent::Create(this, GOCMovementComplex);

            ObjCoccoInfo* info = (ObjCoccoInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjCoccoInfo");

            fnd::GOComponent::BeginSetup(this);

            GetParentObject(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.Animation |= 0x400000;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
                if (gocAnimation)
                {
                    animation::AnimationResContainer* animation = &(info->AnimationContainer);
                    game::GOCAnimationScript::Setup(gocAnimation, &animation);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    game::GOCAnimationScript::SetAnimation(gocAnimation, "IDLE");

                    csl::fnd::IAllocator* allocator{};
                    auto fucnPtr = &ObjCocco::SoundCallback;
                    animation::AnimCallbackBridge<ObjCocco>* callback =
                        (animation::AnimCallbackBridge<ObjCocco>*)AnimCallbackBridge_Initialize(allocator);
                    callback->GameObject = this;
                    callback->field_10 = reinterpret_cast<void*&>(fucnPtr);
                    callback->field_14 = -1;

                    game::GOCAnimationScript::RegisterCallback(gocAnimation, 0, callback);
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                csl::math::Vector3 position{ 0, 3, 0 };
                game::ColliSphereShapeCInfo collisionInfo{};
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 2;
                collisionInfo.field_04 = 1;
                if (!Spawner)
                    collisionInfo.field_08 = 0x20000;

                collisionInfo.field_44 = 0;
                collisionInfo.field_48 = 0;
                collisionInfo.field_54 = 0;
                ObjUtil::SetupCollisionFilter(9, &collisionInfo);
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &position);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                csl::math::Vector3 position { 0, 3, 0 };
                game::ShadowHemisphereShapeCInfo shadowInfo;

                game::ShadowHemisphereShapeCInfo::__ct(&shadowInfo, 2.5f);
                shadowInfo.field_04 = 1;
                shadowInfo.ShadowQualityType = 2;

                game::ShadowHemisphereShapeCInfo* ppShadowInfo = &shadowInfo;
                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
                game::GOCShadowSimple::SetLocalOffsetPosition(gocShadow, &position);
            }

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (gocMovement)
            {
                csl::math::Vector3 position{};

                void* movementMem = ((app::fnd::ReferencedObject*)gocMovement)->pAllocator->
                    Alloc(sizeof(MoveObjCocco), 16);
                MovementController = new(movementMem) MoveObjCocco();
                game::GOCMovement::SetupController(gocMovement, MovementController);

                auto notifyStopCallback = &ObjCocco::NotifyStopCallback;
                MovementController->SetNotifyStopCallback(-1, notifyStopCallback, this);
            }

            game::GOCEffect::SimpleSetupEx(this, 1, 1);
            game::GOCSound::SimpleSetup(this, 0, 0);

            if (GetExtUserData(0) == 1)
            {
                if (gocVisual)
                    fnd::GOCVisual::SetVisible(gocVisual, 0);

                if (gocShadow)
                    game::GOCShadowSimple::SetVisible(gocShadow, 0);

                if (gocCollider)
                    game::GOCCollider::SetEnable(gocCollider, 0);

                Sleep(this);
            }

            SetEnableAttack(true);
            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            switch (message.Type)
            {
            case fnd::PROC_MSG_DAMAGE:
                ProcMsgDamage((xgame::MsgDamage&)message);
                return true;
            case fnd::PROC_MSG_DLC_ZELDA_NOTICE_ACTIVE_ENEMY:
                ProcMsgDlcZeldaNoticeActiveEnemy((xgame::MsgDlcZeldaNoticeActiveEnemy&)message);
                return true;
            case fnd::PROC_MSG_DLC_ZELDA_NOTICE_STOP_ENEMY:
                ProcMsgDlcZeldaNoticeStopEnemy((xgame::MsgDlcZeldaNoticeStopEnemy&)message);
                return true;
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return true;
            case fnd::PROC_MSG_KICK:
                ProcMsgKick((xgame::MsgKick&)message);
                return true;
            case fnd::PROC_MSG_PL_GET_HOMING_TARGET_INFO:
                ProcMsgPLGetHomingTargetInfo((xgame::MsgPLGetHomingTargetInfo&)message);
                return true;
            case fnd::PROC_MSG_PL_KICK_TARGETTING:
                ProcMsgPLKickTargetting((xgame::MsgPLKickTargetting&)message);
                return true;
            default:
                return CSetObjectListener::PreProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == ObjCoccoState::STATE_IDLE)
                StateIdle();

            if (State == ObjCoccoState::STATE_ATTACK_IN)
                StateAttackIn();

            if (State == ObjCoccoState::STATE_ATTACK_OUT)
                StateAttackOut();

            if (State == ObjCoccoState::STATE_END)
                StateEnd();
        }

    private:
        inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new animation::AnimCallbackBridge<ObjCocco>();
        }

        void SoundCallback(int a1, int a2, int a3)
        {
            int* gocSound = GameObject::GetGOC((GameObject*)((char*)this + 1), GOCSoundString);
            int deviceTag[3]{};
            if (!gocSound)
                return;

            if (!a2)
                if (!a3)
                    game::GOCSound::Play3D(gocSound, deviceTag, "obj_cock_flap", 0);
        }

        void ProcMsgDamage(xgame::MsgDamage& message)
        {
            *DamageMessage = message;

            message.SetReply(&message.field_30, (field_41C | field_41C - 1) >> 31);
            message.field_90 |= 0x20;
        }

        void ProcMsgDlcZeldaNoticeActiveEnemy(xgame::MsgDlcZeldaNoticeActiveEnemy& message)
        {
            SetExtUserData(0, 0);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
                fnd::GOCVisual::SetVisible(gocVisual, 1);

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
                game::GOCShadowSimple::SetVisible(gocShadow, 1);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
                game::GOCCollider::SetEnable(gocCollider, 1);

            Resume();
        }

        void ProcMsgDlcZeldaNoticeStopEnemy(xgame::MsgDlcZeldaNoticeStopEnemy& message)
        {
            SetExtUserData(0, 1);

            if (State == ObjCoccoState::STATE_IDLE)
            {

                int* gocVisual = GameObject::GetGOC(this, GOCVisual);
                if (gocVisual)
                    fnd::GOCVisual::SetVisible(gocVisual, 0);

                int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
                if (gocShadow)
                    game::GOCShadowSimple::SetVisible(gocShadow, 0);

                int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
                if (gocCollider)
                    game::GOCCollider::SetEnable(gocCollider, 0);

                Sleep(this);
            }
            else
            {
                xgame::MsgKill msg{};
                for (ObjCocco* cocco : SubCoccos)
                {
                    cocco->SendMessageImm(*(int*)(field_24[1] + 0x10), &msg);
                }

                Kill();
            }
        }

        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            *HitMessage = message;
        }

        void ProcMsgKick(xgame::MsgKick& message)
        {
            *KickMessage = message;
            xgame::MsgKick::SetReplyForSucceed(&message);
        }

        void ProcMsgPLGetHomingTargetInfo(xgame::MsgPLGetHomingTargetInfo& message)
        {
            if (field_41C <= 0)
            {
                message.field_18 |= 1;
                return;
            }

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Vector3 vector { 0, 3, 0 };
            csl::math::Matrix34 matrix = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);
            message.field_38 = field_41C;
            message.field_20 = MultiplyMatrixByVector(&matrix, &vector);
        }

        void ProcMsgPLKickTargetting(xgame::MsgPLKickTargetting& message)
        {
            message.field_34 = 1;
        }

        void StateIdle() {}
        void StateAttackIn() {}
        void StateAttackOut() {}
        void StateEnd() {}

        void SetEnableAttack(bool isEnable)
        {
            if (isEnable)
                Flags |= 4;
            else
                Flags &= ~4;
        }

        void NotifyStopCallback()
        {
            Flags |= 1;
        }
    };

    inline static ObjCocco* create_ObjCocco() { return new ObjCocco(); }

    inline static ObjCoccoInfo* createObjInfo_ObjCoccoInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjCoccoInfo();
    }
}