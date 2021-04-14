#pragma once

namespace app
{
    enum class ObjEggState : int
    {
        STATE_TO_FIRST_LOCUS,
        STATE_TO_INDEX_LOCUS,
        STATE_MOVE_INDEX_LOCUS,
        STATE_DROP,
        STATE_MOVE_TO_EXTRICATION,
        STATE_AFTER_EXTRICATION
    };

    class ObjEgg;
    struct EggCInfo;
    namespace egg
    {
        ObjEgg* CreateEgg(GameDocument& gameDocument, EggCInfo* cInfo);
    }

    struct EggCInfo
    {
        csl::math::Matrix34* Transform;
        int ModelType;
        int PlayerNo;
    };

    class ObjEggInfo : public CObjInfo
    {
    public:
        int Models[4]{};

        void Initialize(GameDocument& gameDocument) override
        {
            const char* names[4] { "zdlc02_obj_yoshiegg_green", "zdlc02_obj_yoshiegg_blue", "zdlc02_obj_yoshiegg_red", "zdlc02_obj_yoshiegg_yellow" };

            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            for (size_t i = 0; i < 4; i++)
                ObjUtil::GetModelResource(&Models[i], names[i], &packFile);
        }
        const char* GetInfoName() override { return "ObjEggInfo"; }
    };

    class ObjEgg : public GameObject3D
    {
    public:
        ObjEggState State{};
        float Time{};
        INSERT_PADDING(0x14); // TinyFsm
        EggCInfo* CInfo = new EggCInfo();
        int ModelType{};
        int Index = -1;
        int SpaceCount = 10;
        int Frame{};
        float RotationTime{};
        bool DoRotate{};
        char PlayerNo{};
        INSERT_PADDING(2);
        csl::math::Vector3 DropPosition{ 0, 34.732917f, 35.966991f };
        float ScaleTime{};
        float ScaleX = 1;
        float ScaleY = 1;
        float ScaleZ = 1;
        int SlipperyType = 1;
        int field_374{};
        INSERT_PADDING(8);

        ObjEgg(GameDocument& gameDocument, EggCInfo* cInfo)
        {
            CInfo = cInfo;
            ModelType = cInfo->ModelType;
            if (cInfo->PlayerNo > 1)
                PlayerNo = 0;
            else
                PlayerNo = cInfo->PlayerNo;
        }

        void Destructor(size_t deletingFlags) override
        {
            delete CInfo;

            GameObject3D::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCGravity);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);

            EggManager* eggManager = EggManager::GetService(gameDocument);
            if (!eggManager)
                return;

            if (!PlayerNo)
                Index = eggManager->EggsP1.size();
            else
                Index = eggManager->EggsP2.size();
            bool isEggStored = eggManager->AddEgg(this);

            fnd::GOComponent::BeginSetup(this);

            csl::math::Vector3 position = *(csl::math::Vector3*) & CInfo->Transform->data[3][0];
            csl::math::Quaternion rotation = GetRotationFromMatrix(CInfo->Transform);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
            {
                fnd::GOCTransform::SetLocalTranslation(gocTransform, &position);
                fnd::GOCTransform::SetLocalRotation(gocTransform, &rotation);
            }

            ObjEggInfo* info = (ObjEggInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjEggInfo");

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor{};

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Models[ModelType];
                visualDescriptor.Animation |= 0x400000;
                visualDescriptor.ZIndex = (-0.2f * Index) - 2;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                Eigen::Quaternion<float> q;
                q = Eigen::AngleAxis<float>((3.1415927f / 2), Eigen::Vector3f(0, 1, 0));
                q.normalize();
                csl::math::Quaternion visualRotation{ q.x(), q.y(), q.z(), q.w() };
                fnd::GOCVisualTransformed::SetLocalRotation(gocVisual, &visualRotation);
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;

                game::ColliSphereShapeCInfo collisionInfo{};

                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 3;
                collisionInfo.field_44 = 0;
                collisionInfo.field_48 = 0;
                collisionInfo.field_54 = 0;
                ObjUtil::SetupCollisionFilter(2, &collisionInfo);
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            game::GOCGravity::SimpleSetup(this, 1);
            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

            fnd::GOComponent::EndSetup(this);

            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (gocSound)
            {
                int deviceTag[3];

                if (isEggStored)
                    game::GOCSound::Play(gocSound, deviceTag, "obj_yossyegg_get", 0);
                else
                {
                    xgame::MsgTakeObject msg { xgame::MsgTakeObject::EType::ONE_UP };
                    ObjUtil::SendMessageImmToPlayer(this, CInfo->PlayerNo, &msg);
                    int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
                    if (msg.field_20 && gocEffect)
                    {
                        game::GOCEffect::CreateEffect(gocEffect, "ef_dl2_yossi_birth");
                        game::GOCSound::Play(gocSound, deviceTag, "obj_yossy_1up", 0);
                    }
                    GameObject::Kill(this);
                }
            }
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            if (message.Type != fnd::PROC_MSG_DLC_CHANGE_EGG_ROTATION)
                return GameObject::ProcessMessage(message);

            ProcMsgDlcChangeEggRotation((xgame::MsgDlcChangeEggRotation&)message);
            return true;
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (State == ObjEggState::STATE_TO_FIRST_LOCUS)
                StateToFirstLocus(updateInfo);

            if (State == ObjEggState::STATE_TO_INDEX_LOCUS)
                StateToIndexLocus(updateInfo);

            if (State == ObjEggState::STATE_MOVE_INDEX_LOCUS)
                StateMoveIndexLocus(updateInfo);

            if (State == ObjEggState::STATE_DROP)
                StateDrop();

            if (State == ObjEggState::STATE_MOVE_TO_EXTRICATION)
                StateMoveToExtrication(updateInfo);
        }

        void StartExtrication() { State = ObjEggState::STATE_MOVE_TO_EXTRICATION; }

        bool IsEndExtrication() { return State == ObjEggState::STATE_AFTER_EXTRICATION; }

        void StartDrop() { State = ObjEggState::STATE_DROP; }

        bool SubSpaceOffset()
        {
            if (!SpaceCount)
                return false;

            SpaceCount -= 1;
            return true;
        }

        bool AddSpaceOffset()
        {
            if (SpaceCount >= 0xA)
                return false;

            SpaceCount += 1;
            return true;
        }

    private:
        void ProcMsgDlcChangeEggRotation(xgame::MsgDlcChangeEggRotation& message)
        {
            if (message.field_18)
            {
                if (message.field_18 && !DoRotate)
                {
                    DoRotate = true;
                    RotationTime = 0.3f;
                }
            }
            else if (DoRotate)
            {
                DoRotate = false;
                RotationTime = 0.3f;
            }
        }

        void SlipperyInterpolate(float a1, float scalarX, float scalarY, float scalarZ)
        {
            if ((ScaleTime / (a1 * 0.5)) > 1)
                field_374 &= ~2;

            ScaleX = csl::math::Lerp(ScaleX, scalarX, ScaleTime / (a1 * 0.5f));

            ScaleY = csl::math::Lerp(ScaleY, scalarY, ScaleTime / (a1 * 0.5f));

            ScaleZ = csl::math::Lerp(ScaleZ, scalarZ, ScaleTime / (a1 * 0.5f));
        }

        void SetSlipperyHeightScale(float scalarX, float scalarY, float scalarZ)
        {
            ScaleX = scalarX;
            ScaleY = scalarY;
            ScaleZ = scalarZ;
        }

        void UpdateRotation(csl::math::Quaternion* rotation, const fnd::SUpdateInfo updateInfo, int playerNo)
        {
            csl::math::Quaternion normalizedRotation{};
            csl::math::Quaternion normalizedPlayerRotation{};

            csl::math::QuaternionNormalize(&normalizedRotation, rotation);
            if (!DoRotate)
            {
                int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
                if (playerInfo)
                    csl::math::QuaternionNormalize(&normalizedPlayerRotation, (csl::math::Quaternion*)(playerInfo + 8));
            }

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Quaternion objectRotation = GetRotationFromMatrix((csl::math::Matrix34*)(gocTransform + 0x44));

            if (RotationTime > 0)
            {
                float interpolationAmount = 1 - (RotationTime / 0.3f);
                csl::math::Clamp(interpolationAmount, 0, 1);
                Eigen::Quaternionf oR(objectRotation.X, objectRotation.Y, objectRotation.Z, objectRotation.W);
                Eigen::Quaternionf nR(normalizedRotation.X, normalizedRotation.Y, normalizedRotation.Z, normalizedRotation.W);
                oR.slerp(interpolationAmount, nR);
                normalizedRotation = csl::math::Quaternion(oR.x(), oR.y(), oR.z(), oR.w());
                RotationTime -= updateInfo.deltaTime;
            }

            fnd::GOCTransform::SetLocalRotation(gocTransform, &normalizedRotation);
        }

        void UpdateSlippery(bool isInAir, bool a3, const fnd::SUpdateInfo updateInfo)
        {
            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (!gocVisual)
                return;

            int type = 2;
            if (!isInAir)
                type = !a3;

            if (SlipperyType != type)
            {
                field_374 |= 2;
                SlipperyType = type;
                field_374 &= ~1;
                ScaleTime = 0;
            }

            csl::math::Vector3 position{};
            csl::math::Vector3 scale { 1, 1, 1 };
            csl::math::Vector3 upVec { 0, 1, 0 };
            float multiplier = 0.18f;
            float time{};

            if (!SlipperyType)
            {
                float scalarX = 0;
                float scalarY = 0.6f;
                float scalarZ = 0.1f;

                if (field_374 & 1)
                {
                    scalarY = 0.4f;
                    multiplier = 0.1f;
                    scalarZ = 0.1f;
                }

                ScaleTime += updateInfo.deltaTime;
                time = ScaleTime + (multiplier * 0.25f * Index);
                float ratio = egg::CalcSlipperyRatio(csl::math::Max(time, 0), multiplier);

                if (0 <= ratio)
                {
                    scalarY = (scalarZ * ratio) + 1;
                    scalarZ = -((0.13f * ratio) - 1);
                }
                else
                {
                    scalarX = scalarY * abs(ratio);
                    scalarY = 1;
                    scalarZ = 1;
                }

                if (field_374 & 2)
                {
                    SlipperyInterpolate(multiplier, scalarX, scalarY, scalarZ);
                    scale = Vector3(ScaleY, ScaleZ, 1);
                    fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
                    math::Vector3Scale(&upVec, ScaleX, &position);
                    fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &position);
                    return;
                }

                SetSlipperyHeightScale(scalarX, scalarY, scalarZ);
                int flag = (((60 * updateInfo.deltaTime < 0) << 2) << 0x1C) >> 0x1E;
                if (flag != (field_374 & 1))
                {
                    if (flag == 0)
                        multiplier = 0.18f;

                    ScaleTime = -(multiplier * 0.25f * Index);
                    scale = Vector3(ScaleY, ScaleZ, 1);
                    fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
                    math::Vector3Scale(&upVec, ScaleX, &position);
                    fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &position);
                    return;
                }
            }
            else
            {
                float scalarX = 0;
                float scalarY{};
                float scalarZ = 0.1f;

                if (SlipperyType == 1)
                {
                    ScaleTime += updateInfo.deltaTime;
                    float ratio = egg::CalcSlipperyRatio((Index * 1.6f * 0.25f) + ScaleTime, 1.6f);

                    scalarY = scalarZ * abs(ratio) + 1;
                    scalarZ = 1 - 0.1f * abs(ratio);

                    if (!(field_374 & 2))
                    {
                        SetSlipperyHeightScale(scalarX, scalarY, scalarZ);
                        scale = Vector3(ScaleY, ScaleZ, 1);
                        fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
                        math::Vector3Scale(&upVec, ScaleX, &position);
                        fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &position);
                        return;
                    }

                    SlipperyInterpolate(1.6f, scalarX, scalarY, scalarZ);
                    scale = Vector3(ScaleY, ScaleZ, 1);
                    fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
                    math::Vector3Scale(&upVec, ScaleX, &position);
                    fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &position);
                    return;
                }

                if (SlipperyType != 2)
                {
                    scale = Vector3(ScaleY, ScaleZ, 1);
                    fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
                    math::Vector3Scale(&upVec, ScaleX, &position);
                    fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &position);
                    return;
                }

                ScaleTime += updateInfo.deltaTime;
                if (field_374 & 2)
                    SlipperyInterpolate(0.2f, scalarX, 1, 1);
            }

            scale = Vector3(ScaleY, ScaleZ, 1);
            fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
            math::Vector3Scale(&upVec, ScaleX, &position);
            fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &position);
            return;
        }

        void Extrication()
        {
            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (gocEffect)
                game::GOCEffect::CreateEffect(gocEffect, "ef_dl2_yossi_birth");

            int deviceTag[3];
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (gocSound) 
                game::GOCSound::Play(gocSound, deviceTag, "obj_yossyeggitem_break", 0);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
                fnd::GOCVisual::SetVisible(gocVisual, false);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Matrix34 matrix = *(csl::math::Matrix34*)(gocTransform + 0x44);
            Eigen::Vector3f upVector(0, 10, 0);
            Eigen::Vector3f tVec(matrix.data[3][0], matrix.data[3][1], matrix.data[3][2]);
            Eigen::Vector3f rVec(matrix.data[1][0], matrix.data[1][1], matrix.data[1][2]);
            csl::math::Vector3 transVector { tVec.x() * upVector.x(), tVec.y() * upVector.y(), tVec.z() * upVector.z() };
            csl::math::Vector3 rotationVector { -rVec.x(), -rVec.y(), -rVec.z() };
            
            csl::math::Quaternion rotation = GetRotationFromMatrix(&matrix);
            int* path = ObjUtil::GetSVPath((GameDocument*)field_24[1], &transVector, &rotationVector);
            if (path)
            {
                game::PathEvaluator pathEvaluator{};
                game::PathEvaluator::__ct(&pathEvaluator);
                game::PathEvaluator::SetPathObject(&pathEvaluator, path);
                if (fnd::HandleBase::IsValid(&pathEvaluator))
                {
                    csl::math::Vector3 splinePoint{};
                    csl::math::Vector3 someVector{};
                    csl::math::Vector3 someVector2{};

                    csl::math::Vector3 objectPosition = *(csl::math::Vector3*)(gocTransform + 0x50);
                    float length = game::PathEvaluator::GetLength(&pathEvaluator);
                    game::PathEvaluator::GetClosestPositionAlongSpline(&pathEvaluator, &objectPosition, &splinePoint, 0, &length);
                    game::PathEvaluator::SetDistance(&pathEvaluator, splinePoint.X);
                    game::PathEvaluator::GetPNT(&pathEvaluator, &pathEvaluator.field_08, &splinePoint, &someVector, &someVector2);
                    math::Vector3CrossProduct(&someVector, &someVector2, &objectPosition);
                    csl::math::Matrix34 pointMatrix{};
                    *(csl::math::Vector3*)&pointMatrix.data[0][0] = objectPosition;
                    *(csl::math::Vector3*)&pointMatrix.data[1][0] = someVector;
                    *(csl::math::Vector3*)&pointMatrix.data[2][0] = someVector2;
                    rotation = GetRotationFromMatrix(&pointMatrix);
                }
            }

            csl::math::Vector3 objectPosition = *(csl::math::Vector3*)(gocTransform + 0x50);
            ObjYoshi::CInfo* yoshiInfo = new ObjYoshi::CInfo(ModelType, PlayerNo, &objectPosition, &rotation, Index);
            ObjYoshi::Create(*(GameDocument*)field_24[1], *yoshiInfo);
        }

        void StateToFirstLocus(const fnd::SUpdateInfo& updateInfo)
        {
            EggManager::LocusData locusData{};
            csl::math::Vector3 translation{};
            csl::math::Vector3 posDifference{};

            EggManager* eggManager = EggManager::GetService((GameDocument*)field_24[1]);
            if (!eggManager)
                return;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            bool isMoving = false;
            eggManager->GetTargetDataFromLocusIndex(&locusData, 0, &isMoving, nullptr, PlayerNo);
            Time = csl::math::Clamp(Time, 0, 1);
                
            math::CalculatedTransform::GetTranslation((csl::math::Matrix34*)(gocTransform + 0x44), &translation);
            math::Vector3Subtract(&locusData.Position, &translation, &posDifference);
            math::Vector3Scale(&posDifference, Time, &posDifference);
            math::Vector3Add(&posDifference, &translation, &posDifference);
            fnd::GOCTransform::SetLocalTranslation(gocTransform, &posDifference);
            
            UpdateRotation(&locusData.Rotation, updateInfo, PlayerNo);
            UpdateSlippery(locusData.IsInAir, 1, updateInfo);

            Time += updateInfo.deltaTime;
            Frame++;

            if (Frame == 30)
            {
                Frame = 0;
                State = ObjEggState::STATE_TO_INDEX_LOCUS;
            }
        }

        void StateToIndexLocus(const fnd::SUpdateInfo& updateInfo)
        {
            EggManager::LocusData locusData{};

            EggManager* eggManager = EggManager::GetService((GameDocument*)field_24[1]);
            if (!eggManager)
                return;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            int locusIndex = eggManager->GetTargetLocusIndex(Index, PlayerNo);
            if (locusIndex < Frame)
                Frame = locusIndex;

            bool isMoving = false;
            eggManager->GetTargetDataFromLocusIndex(&locusData, Frame, &isMoving, nullptr, PlayerNo);
            fnd::GOCTransform::SetLocalTranslation(gocTransform, &locusData.Position);

            UpdateRotation(&locusData.Rotation, updateInfo, PlayerNo);
            UpdateSlippery(locusData.IsInAir, 1, updateInfo);

            Frame++;
            if (locusIndex < Frame)
                State = ObjEggState::STATE_MOVE_INDEX_LOCUS;
        }

        void StateMoveIndexLocus(const fnd::SUpdateInfo& updateInfo)
        {
            EggManager::LocusData locusData{};

            EggManager* eggManager = EggManager::GetService((GameDocument*)field_24[1]);
            if (!eggManager)
                return;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            bool isMoving = false;
            float magnitude = 0;
            eggManager->GetTargetData(&locusData, Index, &isMoving, &magnitude, PlayerNo);
            fnd::GOCTransform::SetLocalTranslation(gocTransform, &locusData.Position);

            UpdateRotation(&locusData.Rotation, updateInfo, PlayerNo);
            UpdateSlippery(locusData.IsInAir, isMoving, updateInfo);
        }

        void StateDrop()
        {
            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            int* gocGravity = GameObject::GetGOC(this, GOCGravityString);
            if (!gocTransform || !gocGravity)
            {
                Kill(this);
                return;
            }

            csl::math::Vector3 gravityDirection { *game::GOCGravity::GetGravityDirection(gocGravity) };
            math::Vector3Scale(&gravityDirection, 200, &gravityDirection);
            math::Vector3Scale(&gravityDirection, Time, &gravityDirection);
            math::Vector3Add(&DropPosition, &gravityDirection, &DropPosition);
            csl::math::Vector3 dropDistance{};
            math::Vector3Scale(&DropPosition, Time, &dropDistance);
        
            csl::math::Vector3 eggPosition{};
            math::CalculatedTransform::GetTranslation((csl::math::Matrix34*)(gocTransform + 0x44), &eggPosition);
            math::Vector3Add(&eggPosition, &dropDistance, &eggPosition);
            fnd::GOCTransform::SetLocalTranslation(gocTransform, &eggPosition);
            if (!(Frame % 5))
            {
                int* gocVisual = GameObject::GetGOC(this, GOCVisual);
                if (gocVisual)
                {
                    bool isVisible = fnd::GOCVisual::IsVisible(gocVisual);
                    fnd::GOCVisual::SetVisible(gocVisual, isVisible ^ 1);
                }
            }
            Frame++;
            if (Frame > 300)
                Kill(this);
        }

        void StateMoveToExtrication(const fnd::SUpdateInfo& updateInfo)
        {
            EggManager* eggManager = EggManager::GetService((GameDocument*)field_24[1]);
            if (!eggManager)
                return;

            EggManager::LocusData locusData{};
            EggManager::LocusData nextLocusData{};
            EggManager::LocusData currentLocus{};
            int locusIndex = eggManager->GetTargetLocusIndex(Index, PlayerNo);
            if (locusIndex)
            {
                for (size_t i = 0; i < locusIndex; i++)
                {
                    bool isMoving = false;
                    eggManager->GetTargetDataFromLocusIndex(&locusData, i, &isMoving, nullptr, PlayerNo);
                }
            }

            bool isMoving = false;
            eggManager->GetTargetDataFromLocusIndex(&currentLocus, 0, &isMoving, nullptr, PlayerNo);
            eggManager->GetTargetDataFromLocusIndex(&nextLocusData, locusIndex, &isMoving, nullptr, PlayerNo);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            fnd::GOCTransform::SetLocalTranslation(gocTransform, &nextLocusData.Position);
            UpdateRotation(&nextLocusData.Rotation, updateInfo, PlayerNo);
            UpdateSlippery(nextLocusData.IsInAir, 1, updateInfo);
            if (!nextLocusData.IsInAir)
            {
                math::Vector3Subtract(&nextLocusData.Position, &currentLocus.Position, &locusData.Position);
                if (abs(locusData.Position.X) > 0.35f || abs(locusData.Position.Y) > 0.35f || abs(locusData.Position.Z) > 0.35f || isMoving)
                    return;

                Extrication();
                State = ObjEggState::STATE_AFTER_EXTRICATION;
            }
        }
    };
}

inline static app::ObjEgg* app::egg::CreateEgg(GameDocument& gameDocument, EggCInfo* cInfo)
{
    ObjEgg* object = new ObjEgg(gameDocument, cInfo);
    if (object)
        GameDocument::AddGameObject(*(GameDocument**)&gameDocument, object);
    
    return object;
}