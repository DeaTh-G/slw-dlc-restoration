#pragma once
#include <random>

inline static const char* Y_ANIM_STATE[] = { "RESULT1", "RESULT2", "RESULT3", "RESULT4", "RESULT5", "RESULT6" };
inline static const char* Y_ANIM_NAME[] = { "RESULT1_START", "RESULT1_LOOP", "RESULT2_START", "RESULT2_LOOP",
                                            "RESULT3_START", "RESULT3_LOOP", "RESULT4_START", "RESULT4_LOOP",
                                            "RESULT5_START", "RESULT5_LOOP", "RESULT6_START", "RESULT6_LOOP" };

namespace app
{
    enum class ObjYoshiState : int
    {
        STATE_RELEASE,
        STATE_POSE,
        STATE_END
    };

    class ObjYoshiInfo : public CObjInfo
    {
    public:
        int Models[4]{};
        int Skeleton{};
        animation::AnimationResContainer AnimationContainer{};
        std::vector<int> AnimationID{};
        int TexSrtAnimContainer[24]{};

        ObjYoshiInfo()
        {
            animation::AnimationResContainer::__ct(&AnimationContainer, (csl::fnd::IAllocator*)pAllocator);
        }

        void Destructor(size_t deletingFlags) override
        {
            ObjYoshiInfo::~ObjYoshiInfo();
            animation::AnimationResContainer::__dt(&AnimationContainer);

            CObjInfo::Destructor(deletingFlags);
        }

        void Initialize(GameDocument& gameDocument) override
        {
            const char* modelNames[4] { "zdlc02_obj_yoshi_green", "zdlc02_obj_yoshi_blue", "zdlc02_obj_yoshi_red", "zdlc02_obj_yoshi_yellow" };
            const char* texSrtAnimNames[24]
            {
                "yos_result01_start_Eye_L",
                "yos_result01_start_Eye_R",
                "yos_result01_loop_Eye_L",
                "yos_result01_loop_Eye_R",
                "yos_result02_start_Eye_L",
                "yos_result02_start_Eye_R",
                "yos_result02_loop_Eye_L",
                "yos_result02_loop_Eye_R",
                "yos_result03_start_Eye_L",
                "yos_result03_start_Eye_R",
                "yos_result03_loop_Eye_L",
                "yos_result03_loop_Eye_R",
                "yos_result04_start_Eye_L",
                "yos_result04_start_Eye_R",
                "yos_result04_loop_Eye_L",
                "yos_result04_loop_Eye_R",
                "yos_result05_start_Eye_L",
                "yos_result05_start_Eye_R",
                "yos_result05_loop_Eye_L",
                "yos_result05_loop_Eye_R",
                "yos_result06_start_Eye_L",
                "yos_result06_start_Eye_R",
                "yos_result06_loop_Eye_L",
                "yos_result06_loop_Eye_R",
            };

            int packFile = 0;
            int animationScript[3]{};
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            for (size_t i = 0; i < 4; i++)
                ObjUtil::GetModelResource(&Models[i], modelNames[i], &packFile);
            ObjUtil::GetSkeletonResource(&Skeleton, "zdlc02_obj_yoshi", packFile);

            ObjUtil::GetAnimationScriptResource(&animationScript, "yoshi", packFile);
            animationScript[1] = Skeleton;

            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&AnimationContainer, animationScript, packFile);

            for (size_t i = 0; i < 12; i++)
                AnimationID.push_back(i % 6);

            for (size_t i = 0; i < 24; i++)
                ObjUtil::GetTexSrtAnimationResource(&TexSrtAnimContainer[i], texSrtAnimNames[i], packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjYoshiInfo";
        }
    };

    class ObjYoshi : public GameObject3D
    {
    public:
        class CInfo
        {
        public:
            int ModelType{};
            int PlayerNo{};
            int field_08{};
            int field_0C{};
            csl::math::Vector3 Position{ 0, 0, 0 };
            csl::math::Quaternion Rotation{ 0, 0, 0, 1 };
            int Index{};

            CInfo(int type, int playerNo, csl::math::Vector3* position, csl::math::Quaternion* rotation, int index)
            {
                ModelType = type;
                PlayerNo = playerNo;
                Position = *position;
                Rotation = *rotation;
                Index = index;
            }
        };

        class BoundListener : public game::MoveBound::Listener
        {
        public:
            int field_00;
            void* object;

            void OnBound(csl::math::Plane& const a1) override
            {
                if (!object)
                    return;

                ((ObjYoshi*)object)->OnBoundCallback();
            }
        };

        INSERT_PADDING(16);
        ObjYoshiState State{};
        int field_32C{};
        int field_330{};
        int field_334{};
        int field_338{};
        int field_33C{};
        int Type{};
        int PlayerNo{};
        int field_348{};
        int field_34C{};
        csl::math::Vector3 Position;
        csl::math::Quaternion Rotation;
        int Index{};
        int field_374{};
        int field_378{};
        int field_37C{};
        BoundListener BoundListener{};
        EnemyUvAnimLinkController UvLinkController{};
        float Time{};
        float field_3B8{};
        char field_3BC{};
        char IsGrounded{};
        INSERT_PADDING(2);

    public:
        ObjYoshi(const CInfo& info)
        {
            Type = info.ModelType;
            PlayerNo = info.PlayerNo;
            Position = info.Position;
            Rotation = info.Rotation;
            Index = info.Index;
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCGravity);
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationScript);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCMovementComplex);

            ObjYoshiInfo* info = (ObjYoshiInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjYoshiInfo");

            fnd::GOComponent::BeginSetup(this);

            if (!(Index & 1))
            {
                Eigen::Quaternion<float> q;
                q = Eigen::AngleAxis<float>(3.1415927f, Eigen::Vector3f(0, 1, 0));
                Eigen::Quaternion<float> q2(Rotation.X, Rotation.Y, Rotation.Z, Rotation.W);
                q *= q2;
                q.normalize();
                Rotation = csl::math::Quaternion(q.w(), q.x(), q.y(), q.z());
            }

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
            {
                fnd::GOCTransform::SetLocalTranslation(gocTransform, &Position);
                fnd::GOCTransform::SetLocalRotation(gocTransform, &Rotation);
            }

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor{};

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Models[Type];
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.Animation |= 0x400000;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                csl::math::Vector3 scale{ 0.55f, 0.55f, 0.55f };
                csl::math::Vector3 localPos { 0, -3, 0 };
                fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
                fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &localPos);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
                if (gocAnimation)
                {
                    animation::AnimationResContainer* animation = &(info->AnimationContainer);

                    game::GOCAnimationScript::Setup(gocAnimation, &animation);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    game::GOCAnimationScript::SetAnimation(gocAnimation, "JUMP");
                }
            }

            csl::fnd::IAllocator* pAllocator = GetAllocator();
            EnemyUvAnimLinkController::Description description { 12, 2 };
            GameObjectHandleBase::__ct(description.field_0C, this);
            UvLinkController.Setup(description, pAllocator);
            for (size_t i = 0; i < 12; i++)
                for (size_t j = 0; j < 2; j++)
                    UvLinkController.Add((int*)info->TexSrtAnimContainer[2 * i + j], Y_ANIM_NAME[i], 0);

            int* gocMovement = GameObject::GetGOC (this, GOCMovementString);
            if (gocMovement)
            {
                unsigned int random = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));
                random = (((random * 2.328306436538696e-10f) * 0.6f) - 0.3f);
                csl::math::Vector3 depthVec{ 0, 0, 1 };
                csl::math::Vector3 rotVec{};
                if ((Index & 1) && !(Index & 2))
                    math::Vector3RotateX(&rotVec, -((((((Index >> 1) * -2) + 87.5f) + random)) * 0.017453292f), &depthVec);
                else
                    math::Vector3RotateX(&rotVec, -((((((Index >> 1) * -2) + 87.5f) + random) - 0.7f) * 0.017453292f), &depthVec);

                csl::math::Vector3 xVec{ 1, 0, 0 };
                if (!(Index & 2))
                    xVec = csl::math::Vector3(-1, 0, 0);

                csl::math::Vector3 xRot{};
                csl::math::Vector3 negXRot{};

                math::Vector3Rotate(&xRot, &Rotation, &xVec);
                math::Vector3Rotate(&negXRot, &Rotation, &rotVec);
                math::Vector3Scale(&negXRot, 130, &negXRot);
                math::Vector3Scale(&xRot, 0.3f, &xRot);
                math::Vector3Scale(&xRot, 10, &xRot);
                math::Vector3Add(&negXRot, &xRot, &negXRot);

                void* movementMem = ((app::fnd::ReferencedObject*)gocMovement)->pAllocator->Alloc
            	(sizeof(game::MoveBound), 16);
                game::MoveBound* movement = new(movementMem) game::MoveBound();
            	
                game::GOCMovement::SetupController(gocMovement, movement);

                game::MoveBound::Description description{};
                description.field_00 = negXRot;
                description.field_10 = 3;
                description.field_14 = 300;
                description.field_18 = 0.8f;
                description.field_1C = 0.75f;
                description.field_20 = 150;
                description.field_28 = 1;
                description.field_2C = 0.1f;
                game::PathEvaluator::__ct(&description.field_38);

                description.field_14 = 200;
                description.field_18 = 1.2f;
                description.field_1C = 0;
                description.field_48 |= 1;

                game::MoveBound::Setup(movement, &description);
                BoundListener.object = this;
                movement->ResetListener(&BoundListener);

                int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
                *(csl::math::Vector3*)(contextParam + 8) = negXRot;
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);
            game::GOCGravity::SimpleSetup(this, 1);

            fnd::GOComponent::EndSetup(this);

            int deviceTag[3]{};
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play(gocSound, deviceTag, "enm_yossy_voice", 0);
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == ObjYoshiState::STATE_RELEASE)
                StateRelease(updateInfo);

            if (State == ObjYoshiState::STATE_POSE)
                StatePose(updateInfo);
        }

        static ObjYoshi* Create(GameDocument& gameDocument, const CInfo& info)
        {
            ObjYoshi* result = new ObjYoshi(info);
            if (result)
                GameDocument::AddGameObject(&gameDocument, result);

            return result;
        }

        void OnBoundCallback()
        {
            field_3BC = 1;

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (!gocMovement)
                return;

            game::GOCMovement::DisableMovementFlag(gocMovement, false);
            if (IsGrounded)
                return;

            IsGrounded = 1;

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
            if (!gocAnimation)
                return;

            game::GOCAnimationScript::ExitLoopSeqInsideAnimation(gocAnimation);
        }
    private:
        void PopupOneup()
        {
            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Matrix34 matrix = *(csl::math::Matrix34*)(gocTransform + 0x44);
            csl::math::Vector3 translation{};
            csl::math::Quaternion rotation{};

            math::CalculatedTransform::GetTranslation(&matrix, &translation);
            math::CalculatedTransform::GetQuaternionRotation(&matrix, &rotation);

            ObjYoshiOneUp::CInfo* oneUpInfo = new ObjYoshiOneUp::CInfo(PlayerNo, translation, rotation);
            ObjYoshiOneUp::Create(*(GameDocument*)field_24[1], *oneUpInfo);
        }

        void UpdateModelPosture(const fnd::SUpdateInfo& updateInfo)
        {
            if (!IsGrounded)
                return;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Quaternion rotation { 0, 0, 0, 1 };
            fnd::GOCTransform::SetLocalRotation(gocTransform, &rotation);
        }

        void StateRelease(const fnd::SUpdateInfo& updateInfo)
        {
            UpdateModelPosture(updateInfo);

            if (field_3BC)
            {
                State = ObjYoshiState::STATE_POSE;
                return;
            }

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (!gocMovement)
                return;

            csl::math::Vector3 mCol2 = *(csl::math::Vector3*)(gocTransform + 0x48);
            mCol2 = csl::math::Vector3(-mCol2.X, -mCol2.Y, -mCol2.Z);

            int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
            if (math::Vector3DotProduct((csl::math::Vector3*)(contextParam + 8), &mCol2) <= 0)
                return;

            csl::math::Vector3 transVector = *(csl::math::Vector3*)(gocTransform + 0x50);
            math::Vector3Scale(&mCol2, 10, &mCol2);
            math::Vector3Add(&transVector, &mCol2, &mCol2);
            if (!*ObjUtil::RaycastHitCollision((GameDocument*)field_24[1], &transVector, &mCol2, 0xC996))
                return;
            
            IsGrounded = 1;

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
            if (!gocAnimation)
                return;

            game::GOCAnimationScript::ExitLoopSeqInsideAnimation(gocAnimation);
        }

        void StatePose(const fnd::SUpdateInfo& updateInfo)
        {
            if (!Time)
            {
                UpdateModelPosture(updateInfo);

                ObjYoshiInfo* info = (ObjYoshiInfo*)ObjUtil::GetObjectInfo((GameDocument*)field_24[1], "ObjYoshiInfo");

                std::random_device rd;
                std::mt19937 g(rd());

                std::shuffle(info->AnimationID.begin(), info->AnimationID.end(), g);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
                if (gocAnimation)
                    game::GOCAnimationScript::ChangeAnimation(gocAnimation, Y_ANIM_STATE[info->AnimationID[Index % 12]]);

                PopupOneup();
            }

            if (Time > 1 && Time <= (1 + updateInfo.deltaTime))
                PopupOneup();
            else if (Time > 2 && Time <= (2 + updateInfo.deltaTime))
                PopupOneup();
            else if (Time > (2 + updateInfo.deltaTime))
                State = ObjYoshiState::STATE_END;

            Time += updateInfo.deltaTime;
        }
    };
}