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

    struct ObjCoccoData
    {
        float MoveRange;
        int* CoccoList;
        int CoccoListSize;
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
            ObjCoccoData* field_20;
            int field_24;

            CInfo(csl::math::Vector3& position, csl::math::Quaternion& rotation, ObjCoccoData* data, int a4)
            {
                Position = position;
                Rotation = rotation;
                field_20 = data;
                field_24 = a4;
            }
        };

    public:
        enum class ActionType : char
        {
            ACTIONTYPE_0,
            ACTIONTYPE_1
        };

        ObjCoccoState State{};
        INSERT_PADDING(16); // TinyFSM
        int field_3B4{};
        int field_3B8{};
        int field_3BC{};
        csl::math::Vector3 Position{};
        csl::math::Quaternion Rotation{};
        ObjCoccoData* Spawner{};
        int field_3E4{};
        int field_3E8{};
        int field_3EC{};
        ActionType ActionType{};
        INSERT_PADDING(3);
        MoveObjCocco* MovementController{};
        float field_408{};
        float MoveRange{};
        std::vector<ObjCocco*> SubCoccos{};
        float RunAwayTime{};
        float Time{};
        float CryTime{};
        int HealthPoint{};
        char Flags{};
        INSERT_PADDING(3);
        int field_424{};
        int field_428{};
        int field_42C{};

        ObjCocco()
        {
            HealthPoint = 3;
            ObjUtil::SetPropertyLockonTarget(this);
        }

        ObjCocco(const CInfo& createInfo)
        {
            State = ObjCoccoState::STATE_ATTACK_IN;

            ActionType = ActionType::ACTIONTYPE_1;
            Position = createInfo.Position;
            Rotation = createInfo.Rotation;
            field_3E4 = createInfo.field_24;
            Spawner = createInfo.field_20;

            HealthPoint = 3;
        };

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationScript);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCShadowSimple);
            fnd::GOComponent::Create(this, GOCMovementComplex);

            MoveRange = GetSpawner()->MoveRange;
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

            // StateIdle & StateAttackIn Enter
            switch (State)
            {
            case app::ObjCoccoState::STATE_IDLE:
            {
                SetTargetOnCircle();
                int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
                if (!gocAnimation)
                    return;

                game::GOCAnimationScript::ChangeAnimation(gocAnimation, "MOVE");
                break;
            }
            case app::ObjCoccoState::STATE_ATTACK_IN:
            {
                break;
            }
            }
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

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
                return CSetObjectListener::ProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == ObjCoccoState::STATE_IDLE)
                StateIdle(updateInfo);

            if (State == ObjCoccoState::STATE_ATTACK_IN)
                StateAttackIn();

            if (State == ObjCoccoState::STATE_ATTACK_OUT)
                StateAttackOut(updateInfo);

            if (!(Flags & 2))
                return;

            if (ActionType != ActionType::ACTIONTYPE_0)
                return;

            float oldTime = Time;
            Time += updateInfo.deltaTime;

            if (Time > 7)
            {
                SetStatusRetire();
                Kill();
                return;
            }

            SubCoccos.clear();

            /*if (oldTime / 0.7f < Time / 0.7f)
                CreateAttackers();*/
        }

    public:
        void NotifyStopCallback()
        {
            Flags |= 1;
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
            message.SetReply(&message.field_30, (HealthPoint | HealthPoint - 1) >> 31);
            message.field_90 |= 0x20;

            if (State == ObjCoccoState::STATE_IDLE)
            {
                DamageJump(message.field_40);
                HealthPoint -= message.HitCount;
            }
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
            switch (State)
            {
            case app::ObjCoccoState::STATE_ATTACK_IN:
            case app::ObjCoccoState::STATE_ATTACK_OUT:
            {
                csl::math::Vector3 position{};
                xgame::MsgDamage damageMsg { 3, 8, 3, &message, &position };

                SendMessageImm(message.ActorID, &damageMsg);
                message.field_0C = 1;
                break;
            }
            default:
                break;
            }
        }

        void ProcMsgKick(xgame::MsgKick& message)
        {
            xgame::MsgKick::SetReplyForSucceed(&message);

            if (State == ObjCoccoState::STATE_IDLE)
            {
                DamageJump(message.field_40);
                message.field_0C = 1;
                HealthPoint -= 1;
            }
        }

        void ProcMsgPLGetHomingTargetInfo(xgame::MsgPLGetHomingTargetInfo& message)
        {
            if (HealthPoint <= 0)
            {
                message.field_18 |= 1;
                return;
            }

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Vector3 vector { 0, 3, 0 };
            csl::math::Matrix34 matrix = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);
            message.field_38 = HealthPoint;
            message.field_20 = MultiplyMatrixByVector(&matrix, &vector);
        }

        void ProcMsgPLKickTargetting(xgame::MsgPLKickTargetting& message)
        {
            message.field_34 = 1;
        }

        void StateIdle(const fnd::SUpdateInfo& updateInfo)
        {
            if (CryTime > 0)
            {
                CryTime -= updateInfo.deltaTime;
                if (!(Flags & 1))
                    return;
            }
            else
            {
                int deviceTag[3]{};
                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (!gocSound)
                    return;

                game::GOCSound::Play3D(gocSound, deviceTag, "obj_cock_cry", 0);
                CryTime += 2;
                if (!(Flags & 1))
                    return;
            }

            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], 0);
            if (!playerInfo || *((float*)playerInfo + 13) >= 0)
                return;

            Flags &= ~1;

            if (HealthPoint <= 0)
            {
                int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
                if (!gocAnimation)
                    return;

                game::GOCAnimationScript::ChangeAnimation(gocAnimation, "ATTACK");

                // StateAttackOut Enter
                if (playerInfo && *((float*)playerInfo + 13) < 0)
                {
                    float squareMagnitude{};
                    math::Vector3SquareMagnitude((csl::math::Vector3*)playerInfo + 3, &squareMagnitude);
                    if (22500 >= squareMagnitude)
                        MovementController->SetTargetPlayer(70, 20);
                    else
                        MovementController->SetTargetPlayer(220, 20);
                }

                State = ObjCoccoState::STATE_ATTACK_OUT;

                if (!(Flags & 4))
                    return;

                Flags |= 2;

                if (ActionType != ActionType::ACTIONTYPE_0)
                    return;
                
                ObjCoccoData* data = GetSpawner();
                for (size_t i = 0; i < data->CoccoListSize; i++)
                {
                    GameObjectHandleBase handle{};
                    ObjUtil::GetGameObjectHandle(&handle, (GameDocument*)field_24[1], data->CoccoList + i);
                    if (handle.IsValid())
                        ((ObjCocco*)handle.Get())->SetEnableAttack(0);
                }

                return;
            }

            SetTargetOnCircle();

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
            if (!gocAnimation)
                return;

            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "MOVE");
        }

        void StateAttackIn() {}

        void StateAttackOut(const fnd::SUpdateInfo& updateInfo)
        {
            if (CryTime > 0)
            {
                CryTime -= updateInfo.deltaTime;
                RunAwayTime += updateInfo.deltaTime;
            }
            else
            {
                int deviceTag[3]{};
                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (!gocSound)
                    return;

                game::GOCSound::Play3D(gocSound, deviceTag, "obj_cock_cry", 0);
                CryTime += 2;
            }

            if (Flags & 1 || RunAwayTime > 0.5f && !IsInCamera())
            {
                Flags &= ~1;

                if (ActionType == ActionType::ACTIONTYPE_0)
                {
                    State = ObjCoccoState::STATE_END;
                    if (Flags & 2)
                    {
                        // StateAttackOut End
                        ObjCoccoData* data = GetSpawner();
                        for (size_t i = 0; i < data->CoccoListSize; i++)
                        {
                            GameObjectHandleBase handle{};
                            ObjUtil::GetGameObjectHandle(&handle, (GameDocument*)field_24[1], data->CoccoList + i);
                            if (handle.IsValid())
                                ((ObjCocco*)handle.Get())->SetEnableAttack(1);
                        }

                        // StateEnd Enter
                        int* gocVisual = GameObject::GetGOC(this, GOCVisual);
                        if (gocVisual)
                            fnd::GOCVisual::SetVisible(gocVisual, 0);

                        int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
                        if (gocShadow)
                            game::GOCShadowSimple::SetVisible(gocShadow, 0);

                        int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
                        if (gocMovement)
                            game::GOCMovement::DisableMovementFlag(gocMovement, 0);
                    }
                    return;
                }

                Kill();
                return;
            }
        }

        ObjCocco* CreateAttacker(const CInfo& info)
        {
            return new ObjCocco(info);
        }

        void CreateAttackers()
        {
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], 0);
            if (!playerInfo)
                return;

            csl::math::Vector3 playerPosition { *((csl::math::Vector3*)playerInfo + 1) };
            csl::math::Quaternion rotation{};

            csl::math::Matrix34 cameraMatrix{};
            Render::CameraParam* camera = CWorld::GetCamera(0);
            camera->GetInvViewMatrix(&cameraMatrix);

            csl::math::Vector3 cUpVector = cameraMatrix.GetColumn(0); -cUpVector;
            csl::math::Vector3 cLeftVector = cameraMatrix.GetColumn(1); -cLeftVector;
            csl::math::Vector3 cForwardVector = cameraMatrix.GetColumn(2);
            csl::math::Vector3 cameraPosition = cameraMatrix.GetTransVector();
            
            csl::math::Vector3 position = Vector3(cameraPosition.X, playerPosition.Y, cameraPosition.Z);
            math::Vector3Scale(&cForwardVector, 10, &cForwardVector);
            math::Vector3Add(&position, &cForwardVector, &position);

            csl::math::Vector3 positionDiff{};
            csl::math::Vector3 upVector { 0, 1, 0 };
            csl::math::Vector3 scaledUpVector{};
            math::Vector3Subtract(&playerPosition, &cameraPosition, &positionDiff);
            float dot = math::Vector3DotProduct(&positionDiff, &upVector);
            math::Vector3Scale(&upVector, dot, &scaledUpVector);
            math::Vector3Subtract(&positionDiff, &scaledUpVector, &positionDiff);
            if (math::Vector3NormalizeIfNotZero(&positionDiff, &positionDiff))
            {
                math::Vector3CrossProduct(&upVector, &positionDiff, &scaledUpVector);
                cameraMatrix.SetColumn(0, scaledUpVector);
                cameraMatrix.SetColumn(1, upVector);
                cameraMatrix.SetColumn(2, positionDiff);
                *(csl::math::Vector3*)cameraMatrix.data[3] = Vector3(0, 0, 0);

                rotation = GetRotationFromMatrix(&cameraMatrix);
            }
            else
            {
                rotation = csl::math::Quaternion(0, 0, 0, 1);
            }

            for (int i = 0; i < 2; i++)
            {
                unsigned int random = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));
                float offset{};
                if (i == 0)
                    offset = (random * 2.328306436538696e-10f * 20 + 10) * ((i + 1) / 2);
                else
                    offset = (random * 2.328306436538696e-10f * 20 + 10) * ((i + 1) / -2);
               
                csl::math::Vector3 scaledLeft{};
                math::Vector3Scale(&cUpVector, offset, &cUpVector);
                math::Vector3Add(&position, &cUpVector, &cUpVector);
                math::Vector3Scale(&cLeftVector, 20, &scaledLeft);
                math::Vector3Subtract(&cUpVector, &scaledLeft, &scaledLeft);
                math::Vector3Scale(&cLeftVector, 40, &cUpVector);
                math::Vector3Add(&scaledLeft, &cUpVector, &cUpVector);

                game::PhysicsRaycastOutput output{};
                if (!ObjUtil::RaycastNearestCollision(&output, (GameDocument*)field_24[1], &scaledLeft, &cUpVector, 51606))
                    continue;

                math::Vector3Scale(&cLeftVector, 10, &scaledLeft);
                math::Vector3Subtract(&output.field_00, &scaledLeft, &output.field_00);
                CInfo createInfo { output.field_00, output.field_10, GetSpawner(), i };
                ObjCocco* subCocco = CreateAttacker(createInfo);
                if (subCocco)
                {
                    GameDocument::AddGameObject((GameDocument*)field_24[1], subCocco);
                    SubCoccos.push_back(subCocco);
                }
            }
        }

        void DamageJump(csl::math::Vector3& vector)
        {
            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Matrix34 m = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);
            csl::math::Vector3 leftVector = Vector3(m.data[1][0], m.data[1][1], m.data[1][2]);
            csl::math::Vector3 forwardVector = Vector3(m.data[2][0], m.data[2][1], m.data[2][2]);

            float dot = math::Vector3DotProduct(&vector, &leftVector);
            math::Vector3Scale(&leftVector, dot, &leftVector);
            math::Vector3Subtract(&vector, &leftVector, &leftVector);
            if (!math::Vector3NormalizeIfNotZero(&leftVector, &leftVector))
                leftVector = forwardVector;

            MovementController->SetTargetDirectionJump(&leftVector, 10, 30);

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
            if (!gocAnimation)
                return;

            game::GOCAnimationScript::ChangeAnimation(gocAnimation, "DAMAGE");

            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;

            game::EffectCreateInfo effectInfo{};
            game::EffectCreateInfo::__ct(&effectInfo);
            effectInfo.Name = "ef_dl3_cocco_dmg";
            effectInfo.field_04 = 1;
            effectInfo.field_08 = 1;
            effectInfo.field_40 = GameObject::GetGOC(this, GOCVisual);
            effectInfo.field_44 = "waist";
            effectInfo.field_48 = -1;
            game::GOCEffect::CreateEffectEx(gocEffect, &effectInfo);

            int deviceTag[3]{};
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play3D(gocSound, deviceTag, "obj_cock_damage", 0);
        }

        ObjCoccoData* GetSpawner()
        {
            if (ActionType != ActionType::ACTIONTYPE_0)
                return Spawner;
        
            return (ObjCoccoData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
        }

        bool IsInCamera()
        {
            Render::CameraParam* camera = CWorld::GetCamera(0);

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return 0;

            csl::math::Matrix34 matrix = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);
            csl::math::Vector3 vector{};
            return camera->TransformNDC(&vector, &matrix) && abs(vector.X) < 1.1f && abs(vector.Y) < 1.1f;
        }

        void SetEnableAttack(bool isEnable)
        {
            if (isEnable)
                Flags |= 4;
            else
                Flags &= ~4;
        }

        void SetTargetOnCircle()
        {
            csl::math::Vector3 depthVector { 0, 0, 1 };
            csl::math::Vector3 position{};
            csl::math::Quaternion rotation{};

            fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Matrix34 m = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);
            unsigned int random = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));
            float distance = SonicUSA::System::RadianMaskS(random * 2.328306436538696e-10f * 3.1415927f + (field_408 + 1.5707964f));
            
            CSetAdapter::GetRotation(*(int**)((char*)this + 0x324), &rotation);
            math::Vector3Rotate(&depthVector, &rotation, &depthVector);
            Eigen::Vector3f v(m.data[1][0], m.data[1][1], m.data[1][2]); 
            Eigen::Quaternionf q(Eigen::AngleAxisf(distance, v));
            rotation = csl::math::Quaternion(q.x(), q.y(), q.z(), q.w());
            math::Vector3Rotate(&depthVector, &rotation, &depthVector);
            CSetAdapter::GetPosition(*(int**)((char*)this + 0x324), &position);
            math::Vector3Scale(&depthVector, MoveRange, &depthVector);
            math::Vector3Add(&position, &depthVector, &position);
            MovementController->SetTargetPoint(&position, 10);
            field_408 = distance;
        }
    };

    inline static ObjCocco* create_ObjCocco() { return new ObjCocco(); }

    inline static ObjCoccoInfo* createObjInfo_ObjCoccoInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjCoccoInfo();
    }
}