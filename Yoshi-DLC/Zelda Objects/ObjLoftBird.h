#pragma once

namespace app
{
    inline static const char* LINK_ANIM_NAME[] = { "EVENT_ONE", "EVENT_TWO" };
    inline static Vector3 LOFTBIRD_LIGHT_SIZE[] = { Vector3(10, 10, 35), Vector3(-5, 0, 30), Vector3(-15, 6, 15), Vector3(-15, 15, 0) };
    inline static float LOFTBIRD_LIGHT_VALUE1[] = { 35, 35, 30, 30 };
    inline static float LOFTBIRD_LIGHT_VALUE2[] = { 1, 0.5f, 1, 0.4f };
    inline static float LOFTBIRD_LIGHT_VALUE3[] = { 1, 0.5f, 0.7f, 0.3f };
    inline static float LOFTBIRD_LIGHT_VALUE4[] = { 1, 0.5f, 0.3f, 0.3f };

    enum class ObjLoftBirdLightType : char
    {
        Zero,
        One,
        Two
    };

    struct ObjLoftBirdData
    {
        unsigned int ObjPathId;
        float StartDist;
        float EndDist;
        float MoveSpeed;
        ObjLoftBirdLightType PointLight;
    };

    class ObjLoftBirdInfo : public CObjInfo
    {
    public:
        int Models[2];
        int Skeletons[2];
        animation::AnimationResContainer LinkAnimationContainer{};
        animation::AnimationResContainer BirdAnimationContainer{};
        int TexSrtAnimContainer[4]{};

        ObjLoftBirdInfo()
        {
            animation::AnimationResContainer::__ct(&LinkAnimationContainer, (csl::fnd::IAllocator*)pAllocator);
            animation::AnimationResContainer::__ct(&BirdAnimationContainer, (csl::fnd::IAllocator*)pAllocator);
        }

        void Destructor(size_t deletingFlags) override
        {
            ObjLoftBirdInfo::~ObjLoftBirdInfo();
            animation::AnimationResContainer::__dt(&LinkAnimationContainer);
            animation::AnimationResContainer::__dt(&BirdAnimationContainer);

            CObjInfo::Destructor(deletingFlags);
        }

        void Initialize(GameDocument& gameDocument) override
        {
            const char* modelNames[2] { "zdlc03_obj_link", "zdlc03_obj_loftbird" };
            const char* texSrtAnimNames[4] { "lnk_event01_Eye_L", "lnk_event01_Eye_R", "lnk_event02_Eye_L", "lnk_event02_Eye_R" };

            int packFile = 0;
            int animationScript[3]{};
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            for (size_t i = 0; i < 2; i++)
            {
                ObjUtil::GetModelResource(&Models[i], modelNames[i], &packFile);
                ObjUtil::GetSkeletonResource(&Skeletons[i], modelNames[i], packFile);
            }

            ObjUtil::GetAnimationScriptResource(&animationScript, "zdlc03_obj_link", packFile);
            animationScript[1] = Skeletons[0];

            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&LinkAnimationContainer, animationScript, packFile);

            animationScript[0] = 0; animationScript[1] = 0; animationScript[2] = 0;
            ObjUtil::GetAnimationScriptResource(&animationScript, "zdlc03_obj_loftbird", packFile);
            animationScript[1] = Skeletons[1];

            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&BirdAnimationContainer, animationScript, packFile);

            for (size_t i = 0; i < 4; i++)
                ObjUtil::GetTexSrtAnimationResource(&TexSrtAnimContainer[i], texSrtAnimNames[i], packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjLoftBirdInfo";
        }
    };

    class ObjLoftBird : public CSetObjectListener
    {
    public:
        class Listener : public animation::AnimationListener
        {
        public:
            ObjLoftBird* pLoftBird;

            void OnEvent(int notifyTiming) override
            {
                if (!pLoftBird)
                    return;
                    
                pLoftBird->UpdateNodeTransform();
            }
        };

        game::PathEvaluator PathEvaluator{};
        float EndDistance{};
        float MovementSpeed{};
        Listener AnimationListener{};
        int IsActive{};
        int SoundHandle[3]{};
        int field_3F0{};
        EnemyUvAnimLinkController UvLinkController{};
        int field_420{};
        int field_424{};
        int field_428{};
        int field_42C{};
        int field_430{};
        int field_434{};
        int field_438{};
        int field_43C{};
        int field_440{};
        int field_444{};
        int field_448{};
        int field_44C{};

        ObjLoftBird()
        {
            AnimationListener.field_20 = 2;
        }

        void Destructor(size_t deletingFlags)
        {
            AnimationListener.Destructor(0);

            CSetObjectListener::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualContainer);
            fnd::GOComponent::Create(this, GOCAnimationContainer);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCSound);

            fnd::GOComponent::BeginSetup(this);

            ObjLoftBirdInfo* info = (ObjLoftBirdInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjLoftBirdInfo");
            ObjLoftBirdData* data = (ObjLoftBirdData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            EndDistance = data->EndDist;
            MovementSpeed = data->MoveSpeed;
            if (data->PointLight != ObjLoftBirdLightType::One)
                field_420 = 1;
            else
                field_420 = 0;

            int* gocVContainer = GameObject::GetGOC(this, GOCVisual);
            if (gocVContainer)
            {
                int modelCount = 2;
                fnd::GOCVisualContainer::Setup(gocVContainer, &modelCount);

                for (size_t i = 0; i < 2; i++)
                {
                    int* gocVisual = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
                    if (gocVisual)
                    {
                        csl::math::Vector3 scale{ 2, 2, 2 };

                        fnd::GOCVisualModel::VisualDescription visualDescriptor{};
                        fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                        visualDescriptor.Model = info->Models[i];
                        visualDescriptor.Skeleton = info->Skeletons[i];
                        fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                        fnd::GOCVisualContainer::Add(gocVContainer, gocVisual);
                        fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
                        fnd::GOCVisual::SetVisible(gocVisual, false);
                    }
                }

                int* gocAContainer = GameObject::GetGOC(this, GOCAnimation);
                if (gocAContainer)
                {
                    int animationCount = 2;
                    game::GOCAnimationContainer::Setup(gocAContainer, &animationCount);

                    int* gocAnimation = fnd::GOComponent::CreateSingle(this, GOCAnimationScript);
                    if (gocAnimation)
                    {
                        animation::AnimationResContainer* animation = &(info->LinkAnimationContainer);

                        game::GOCAnimationScript::Setup(gocAnimation, &animation);
                        game::GOCAnimationContainer::Add(gocAContainer, gocAnimation);
                        int* linkModel = *(int**)(*(gocVContainer + 0x10));
                        fnd::GOCVisualModel::AttachAnimation(linkModel, gocAnimation);
                        game::GOCAnimationScript::SetAnimation(gocAnimation, "IDLE_LOOP");
                    }

                    gocAnimation = fnd::GOComponent::CreateSingle(this, GOCAnimationScript);
                    if (gocAnimation)
                    {
                        animation::AnimationResContainer* animation = &(info->BirdAnimationContainer);

                        game::GOCAnimationScript::Setup(gocAnimation, &animation);
                        game::GOCAnimationContainer::Add(gocAContainer, gocAnimation);
                        int* birdModel = *(int**)(*(gocVContainer + 0x10) + 4);
                        fnd::GOCVisualModel::AttachAnimation(birdModel, gocAnimation);
                        game::GOCAnimationScript::SetAnimation(gocAnimation, "FLY_LOOP");

                        csl::fnd::IAllocator* allocator{};
                        auto funcPtr = &ObjLoftBird::SoundCallback;
                        animation::AnimCallbackBridge<ObjLoftBird>* callback =
                            (animation::AnimCallbackBridge<ObjLoftBird>*)AnimCallbackBridge_Initialize(allocator);
                        callback->GameObject = this;
                        callback->field_10 = reinterpret_cast<void*&>(funcPtr);
                        callback->field_14 = -1;

                        game::GOCAnimationScript::RegisterCallback(gocAnimation, 0, callback);

                        AnimationListener.pLoftBird = this;
                        game::GOCAnimationSimple::AddListener(gocAnimation, &AnimationListener);
                    }
                }
            }

            csl::fnd::IAllocator* pAllocator = GetAllocator();
            EnemyUvAnimLinkController::Description description{ 2, 2 };
            GameObjectHandleBase::__ct(description.field_0C, this);
            UvLinkController.Setup(description, pAllocator);
            for (size_t i = 0; i < 2; i++)
                for (size_t j = 0; j < 2; j++)
                    UvLinkController.Add((int*)info->TexSrtAnimContainer[2 * i + j], LINK_ANIM_NAME[i], 0);

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                game::ColliSphereShapeCInfo collisionInfo{};
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                // Search Collider
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 20;
                ObjUtil::SetupCollisionFilter(2, &collisionInfo);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            game::GOCSound::SimpleSetup(this, 1, 1);

            game::PathManager* pathManager = game::PathManager::GetService(gameDocument);
            if (!pathManager)
            {
                fnd::GOComponent::EndSetup(this);
                return;
            }

            int* pathObject = pathManager->GetPathObject(data->ObjPathId);
            if (!pathObject)
            {
                fnd::GOComponent::EndSetup(this);
                return;
            }

            game::PathEvaluator::SetPathObject(&PathEvaluator, pathObject);
            if (!fnd::HandleBase::IsValid(&PathEvaluator))
            {
                fnd::GOComponent::EndSetup(this);
                return;
            }

            game::PathEvaluator::SetDistance(&PathEvaluator, data->StartDist);
            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
            {
                fnd::GOComponent::EndSetup(this);
                return;
            }

            csl::math::Matrix34 matrix{};

            csl::math::Vector3 splinePoint{};
            csl::math::Vector3 someVector{};
            csl::math::Vector3 someVector2{};
            csl::math::Vector3 objectPosition{};
            game::PathEvaluator::GetPNT(&PathEvaluator, PathEvaluator.field_08, &splinePoint, &someVector, &someVector2);
            fnd::GOCTransform::SetLocalTranslation(gocTransform, &splinePoint);
            math::Vector3CrossProduct(&someVector, &someVector2, &objectPosition);
            *(csl::math::Vector3*)&matrix.data[0] = objectPosition;
            *(csl::math::Vector3*)&matrix.data[1] = someVector;
            *(csl::math::Vector3*)&matrix.data[2] = someVector2;
            *(csl::math::Vector3*)&matrix.data[3] = Vector3(0, 0, 0);
            csl::math::Quaternion rotation = GetRotationFromMatrix(&matrix);
            fnd::GOCTransform::SetLocalRotation(gocTransform, &rotation);

            fnd::GOComponent::EndSetup(this);
            GameObject::Sleep(this);
            CreatePointLight();
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            switch (message.Type)
            {
            case fnd::PROC_MSG_NOTIFY_OBJECT_EVENT:
                ProcMsgNotifyObjectEvent((xgame::MsgNotifyObjectEvent&)message);
                return true;
            case fnd::PROC_MSG_LOFT_BIRD_COLLISION:
                ProcMsgLoftBirdCollision((xgame::MsgLoftBirdCollision&)message);
                return true;
            case fnd::PROC_MSG_DLC_ZELDA_NOTICE_STOP_ENEMY:
                ProcMsgDlcZeldaNoticeStopEnemy();
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (!IsActive)
                return;

            float pathLength = game::PathEvaluator::GetLength(&PathEvaluator);
            float nextPos = (MovementSpeed * updateInfo.deltaTime) + PathEvaluator.field_08;
            if (nextPos >= pathLength || nextPos >= EndDistance)
            {
                CSetObjectListener::SetStatusRetire(this);
                GameObject::Kill(this);
            }
            else
            {
                int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
                if (!gocTransform)
                    return;

                csl::math::Matrix34 matrix{};
                nextPos = csl::math::Clamp(nextPos, 0, pathLength);

                csl::math::Vector3 splinePoint{};
                csl::math::Vector3 someVector{};
                csl::math::Vector3 someVector2{};
                csl::math::Vector3 objectPosition{};
                game::PathEvaluator::SetDistance(&PathEvaluator, nextPos);
                game::PathEvaluator::GetPNT(&PathEvaluator, PathEvaluator.field_08, &splinePoint, &someVector, &someVector2);
                math::Vector3CrossProduct(&someVector, &someVector2, &objectPosition);
                *(csl::math::Vector3*)&matrix.data[0] = objectPosition;
                *(csl::math::Vector3*)&matrix.data[1] = someVector;
                *(csl::math::Vector3*)&matrix.data[2] = someVector2;
                *(csl::math::Vector3*)&matrix.data[3] = Vector3(0, 0, 0);
                csl::math::Quaternion rotation = GetRotationFromMatrix(&matrix);
                csl::math::Quaternion objectRotation = *(csl::math::Quaternion*)(gocTransform + 0x1C);

                Eigen::Quaternionf qOR(objectRotation.X, objectRotation.Y, objectRotation.Z, objectRotation.W);
                Eigen::Quaternionf q(rotation.X, rotation.Y, rotation.Z, rotation.W);
                qOR = qOR.slerp(10 * updateInfo.deltaTime, q);
                rotation = csl::math::Quaternion(qOR.w(), qOR.x(), qOR.y(), qOR.z());

                fnd::GOCTransform::SetLocalTranslation(gocTransform, &splinePoint);
                fnd::GOCTransform::SetLocalRotation(gocTransform, &rotation);
            }
        }

    private:
        void ProcMsgLoftBirdCollision(xgame::MsgLoftBirdCollision& message)
        {
            switch (message.EventType)
            {
            case app::ObjLoftBirdCollisionType::FLAP_WING:
            {
                int* gocContainer = GameObject::GetGOC(this, GOCAnimation) + 0xF;
                if (!gocContainer)
                    break;

                int* animation = *(int**)(*gocContainer + 4);
                game::GOCAnimationScript::ChangeAnimation(animation, "FLAP_WING");
                break;
            }
            case app::ObjLoftBirdCollisionType::LOFTBIRD_CRY:
            {
                int deviceTag[3];
                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (gocSound)
                    game::GOCSound::Play(gocSound, deviceTag, "obj_loftbird_cry", 0);

                break;
            }
            case app::ObjLoftBirdCollisionType::EVENT_ONE:
            {
                int* gocContainer = GameObject::GetGOC(this, GOCAnimation) + 0xF;
                if (!gocContainer)
                    break;

                int* animation = *(int**)(*gocContainer);
                game::GOCAnimationScript::ChangeAnimation(animation, "EVENT_ONE");
                break;
            }
            case app::ObjLoftBirdCollisionType::EVENT_TWO:
            {
                int* gocContainer = GameObject::GetGOC(this, GOCAnimation) + 0xF;
                if (!gocContainer)
                    break;

                int* animation = *(int**)(*gocContainer);
                game::GOCAnimationScript::ChangeAnimation(animation, "EVENT_TWO");
                break;
            }
            }
        }

        void ProcMsgNotifyObjectEvent(xgame::MsgNotifyObjectEvent& message)
        {
            if (!message.field_18 || IsActive)
                return;

            int* gocContainer = GameObject::GetGOC(this, GOCVisual) + 0x10;
            if (!gocContainer)
                return;

            for (size_t i = 0; i < 2; i++)
            {
                int* model = *(int**)(*gocContainer + 4 * i);
                fnd::GOCVisual::SetVisible(model, true);
            }

            IsActive = 1;

            GameObject::Resume(this);
        }

        void ProcMsgDlcZeldaNoticeStopEnemy()
        {
            if (IsActive)
            {
                CSetObjectListener::SetStatusRetire(this);
                GameObject::Kill(this);
            }
        }

        void SoundCallback(int a1, int a2, int a3)
        {
            if (a2 != 1)
                return;

            int deviceTag[3]{};
            int* gocSound = GameObject::GetGOC((GameObject*)((char*)this + 1), GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play3D(gocSound, deviceTag, "obj_loftbird_flap", 0);
        }

        void UpdateNodeTransform()
        {
            int* gocContainer = GameObject::GetGOC(this, GOCVisual) + 0x10;
            if (!gocContainer)
                return;

            int* birdModel = *(int**)(*gocContainer + 4);
            if (!birdModel)
                return;

            math::Transform transform{};
            fnd::GOCVisualModel::GetNodeTransform(birdModel, 1, "Const_link", &transform);

            int* linkModel = *(int**)(*gocContainer);
            if (!linkModel)
                return;

            fnd::GOCVisualModel::SetNodeTransform(linkModel, 1, "Reference", &transform);
        }

        void CreatePointLight()
        {
            int* gocContainer = GameObject::GetGOC(this, GOCVisual) + 0x10;
            if (!gocContainer)
                return;

            int* birdModel = *(int**)(*gocContainer + 4);
            if (!birdModel)
                return;

            math::Transform transform{};
            fnd::GOCVisualModel::GetNodeTransform(birdModel, 1, "Const_link", &transform);

            int* linkModel = *(int**)(*gocContainer);
            if (!linkModel)
                return;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            for (size_t i = 0; i < 2; i++)
            {
                int lightCount = 2 * field_420 + i;

                ObjectPartPointLight::CInfo lightInfo
                {
                    LOFTBIRD_LIGHT_SIZE[lightCount],
                    LOFTBIRD_LIGHT_VALUE1[lightCount],
                    1,
                    LOFTBIRD_LIGHT_VALUE2[lightCount],
                    LOFTBIRD_LIGHT_VALUE3[lightCount],
                    LOFTBIRD_LIGHT_VALUE4[lightCount],
                    -1,
                    gocTransform,
                    1
                };

                ObjectPartPointLight::Create((GameDocument*)field_24[1], &lightInfo);
                break;
            }
        }

        inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
        {
            return new animation::AnimCallbackBridge<ObjLoftBird>();
        }
    };

    inline static ObjLoftBird* create_ObjLoftBird() { return new ObjLoftBird(); }

    inline static ObjLoftBirdInfo* createObjInfo_ObjLoftBirdInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjLoftBirdInfo();
    }
}