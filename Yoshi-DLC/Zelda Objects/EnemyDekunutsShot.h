#pragma once

namespace app
{
    namespace dekunuts_shot
    {
        struct CreateInfo
        {
            csl::math::Matrix34 Transform{};
            int Model{};
            float field_44{};
            float MaxLifeTime{};
            float field_4C{};

            CreateInfo(csl::math::Matrix34* matrix, int model, float a2, float maxLifeTime)
            {
                Transform = *matrix;
                Model = model;
                field_44 = a2;
                MaxLifeTime = maxLifeTime;
            }
        };
    }

    class EnemyDekunutsShot : public GameObject3D
    {
        INSERT_PADDING(4);
        dekunuts_shot::CreateInfo* CInfo;
        INSERT_PADDING(12);
        csl::math::Vector3 field_330;
        float MaxLifeTime;
        float LifeTime;
        float Rotation;
        int Flags;

    public:
        EnemyDekunutsShot(dekunuts_shot::CreateInfo* info)
        {
            CInfo = info;
            MaxLifeTime = info->MaxLifeTime;
            field_330 = *(csl::math::Vector3*)&info->Transform.data[3][0];
            ObjUtil::SetPropertyLockonTarget(this);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCGravity);
            fnd::GOComponent::Create(this, GOCMovementComplex);
            fnd::GOComponent::Create(this, GOCShadowSimple);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);

            fnd::GOComponent::BeginSetup(this);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
            {
                csl::math::Vector3 position = *(csl::math::Vector3*)&CInfo->Transform.data[3][0];
                csl::math::Quaternion rotation = GetRotationFromMatrix(&CInfo->Transform);

                fnd::GOCTransform::SetLocalTranslation(gocTransform, &position);
                fnd::GOCTransform::SetLocalRotation(gocTransform, &rotation);
            }

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;
                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                visualDescriptor.Model = CInfo->Model;
                visualDescriptor.Animation |= 0x400000;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
            }

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                game::ShadowSphereShapeCInfo shadowInfo;
                game::ShadowSphereShapeCInfo* ppShadowInfo = &shadowInfo;

                game::ShadowSphereShapeCInfo::__ct(&shadowInfo, 1.5f);
                shadowInfo.field_04 = 5;
                shadowInfo.ShadowQualityType = 2;

                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 2;
                game::ColliSphereShapeCInfo colliderInfo{};
                game::GOCCollider::Setup(gocCollider, &shapeCount);

                // Reticle Collider
                game::CollisionObjCInfo::__ct(&colliderInfo);
                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                colliderInfo.MotionType = 2;
                colliderInfo.Radius = 1.5f;
                ObjUtil::SetupCollisionFilter(0, &colliderInfo);
                colliderInfo.field_04 |= 1;
                colliderInfo.field_08 = 0x20000;
                colliderInfo.field_0C = 0;
                game::GOCCollider::CreateShape(gocCollider, &colliderInfo);

                // Hit Collider
                game::CollisionObjCInfo::__ct(&colliderInfo);
                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                colliderInfo.MotionType = 2;
                colliderInfo.Radius = 1.5f;
                ObjUtil::SetupCollisionFilter(3, &colliderInfo);
                colliderInfo.field_04 |= 1;
                colliderInfo.field_0C = 1;
                game::GOCCollider::CreateShape(gocCollider, &colliderInfo);
                ObjUtil::SetEnableColliShape(gocCollider, 1, false);
            }

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (gocMovement)
            {
                int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
                csl::math::Vector3 vector = *(csl::math::Vector3*)&CInfo->Transform.data[0][0];
                math::Vector3Scale(&vector, CInfo->field_44, &vector);
                vector.Y = 0;
                *(csl::math::Vector3*)(contextParam + 8) = vector;

                void* moveVelocity = ((csl::fnd::IAllocator*)(((int**)gocMovement)[2]))->Alloc(128, 16);
                int* mv = game::MoveVelocityReference::SetMoveController(moveVelocity);
                game::GOCMovement::SetupController(gocMovement, moveVelocity);
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);
            game::GOCGravity::SimpleSetup(this, 1);

            fnd::GOComponent::EndSetup(this);
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
                return GameObject3D::ProcessMessage(message);
            }
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            bool doKill = false;
            LifeTime += updateInfo.deltaTime;
            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (!gocVisual)
                return;

            int deviceTag[3]{};
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;

            if (!(Flags & 2))
            {
                Rotation += 720 * 0.017453292f * updateInfo.deltaTime;
                Rotation = SonicUSA::System::RadianMaskU(Rotation);

                fnd::GOCVisualTransformed::SetLocalRotationXYZ(gocVisual, Rotation, 0, 0);

                if (LifeTime > MaxLifeTime)
                    doKill = true;

                if ((Flags & 1) == 1)
                    doKill = true;
            }
            else
            {
                Rotation += 540 * 0.017453292f * updateInfo.deltaTime;
                Rotation = SonicUSA::System::RadianMaskU(Rotation);

                fnd::GOCVisualTransformed::SetLocalRotationXYZ(gocVisual, Rotation, 0, 0);

                if (LifeTime <= MaxLifeTime)
                    if ((Flags & 1) == 1)
                        doKill = true;
            }

            if (doKill)
            {
                game::GOCSound::Play3D(gocSound, deviceTag, "enm_cmn_bullet_landing", 0);
                game::GOCEffect::CreateEffect(gocEffect, "ef_dl3_dekunuts_hit");
                Kill();
            }
        }

        void Explosion() { Flags |= 1; }

    private:
        void ProcMsgDamage(xgame::MsgDamage& message)
        {
            csl::math::Vector3 vector{};

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            vector = *(csl::math::Vector3*)(gocTransform + 0x50);
            message.SetReply(&vector, 1);

            if (!ObjUtil::CheckShapeUserID(*(int*)&message.field_18, 0))
                return;

            Damaged(message);
        }

        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;
            
            game::GOCEffect::CreateEffect(gocEffect, "ef_dl3_dekunuts_hit");
            Flags |= 1;
            if ((Flags & 2) == 2)
            {
                if (ObjUtil::CheckShapeUserID(message.field_18, 1))
                {
                    int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
                    if (!gocMovement)
                        return;

                    int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
                    csl::math::Vector3 vector = *(csl::math::Vector3*)(contextParam + 8);

                    xgame::MsgDamage damageMessage{ 2, 8, 2, &message, &vector };
                    SendMessageImm(message.ActorID, &damageMessage);
                    return;
                }
                Flags &= ~1;
            }

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (!gocMovement)
                return;

            int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
            csl::math::Vector3 vector = *(csl::math::Vector3*)(contextParam + 8);

            xgame::MsgDamage damageMessage{ 2, 8, 1, &message, &vector };
            SendMessageImm(message.ActorID, &damageMessage);
        }

        void ProcMsgKick(xgame::MsgKick& message)
        {
            if (!ObjUtil::CheckShapeUserID(*(int*)&message.field_18, 0))
                return;
            
            xgame::MsgKick::SetReplyForSucceed(&message);
            Damaged(message);
        }

        void ProcMsgPLGetHomingTargetInfo(xgame::MsgPLGetHomingTargetInfo& message)
        {
            if (!ObjUtil::CheckShapeUserID(message.field_40, 0) || (Flags & 2) == 2)
                message.field_18 |= 1;

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (!gocMovement)
                return;

            int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
            message.field_20 = *(csl::math::Vector3*)contextParam;
            message.field_18 |= 8;
        }

        void ProcMsgPLKickTargetting(xgame::MsgPLKickTargetting& message)
        {
            message.field_34 = 0;
            if (!ObjUtil::CheckShapeUserID(message.field_18, 0))
                return;
        
            if (!(Flags && 2))
                message.field_34 = 1;
        }

        void Damaged(xgame::MsgDamageBase& message)
        {
            csl::math::Vector3 bulletDistance{};
            csl::math::Quaternion rotation{};

            if (message.field_28 != 1)
                return;

            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], message.field_50);
            if (!playerInfo)
                return;

            Flags &= ~1;
            Flags |= 2;

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (!gocCollider)
                return;

            ObjUtil::SetEnableColliShape(gocCollider, 0, false);

            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (!gocMovement)
                return;

            int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
            math::Vector3Subtract(&field_330, (csl::math::Vector3*)contextParam, &bulletDistance);
            bool isNormalized = math::Vector3NormalizeIfNotZero(&bulletDistance, &bulletDistance);
            if (!isNormalized)
                return;
            
            csl::math::Vector3 depthVec { 0, 0, 1 };
            math::Vector3Rotate(&depthVec, (csl::math::Quaternion*)(contextParam + 4), &depthVec);
            csl::math::QuaternionRotationBetweenNormals(&rotation, &depthVec, &bulletDistance);
            math::Vector3Scale(&bulletDistance, 200, &bulletDistance);
            bulletDistance.Y = 0;
            *(csl::math::Vector3*)(contextParam + 8) = bulletDistance;

            Eigen::Quaternion<float> q(rotation.X, rotation.Y, rotation.Z, rotation.W);
            csl::math::Quaternion r = *(csl::math::Quaternion*)(contextParam + 4);
            Eigen::Quaternion<float> q2(r.X, r.Y, r.Z, r.W);
            q *= q2;
            q.normalize();
            *(csl::math::Quaternion*)(contextParam + 4) = csl::math::Quaternion(q.w(), q.x(), q.y(), q.z());
            ObjUtil::SetEnableColliShape(gocCollider, 1, true);
            LifeTime = 0;
        }
    };

    namespace dekunuts_shot
    {
        static EnemyDekunutsShot* Create(CreateInfo* info, GameDocument& gameDocument)
        {
            EnemyDekunutsShot* result = new EnemyDekunutsShot(info);
            if (result)
                GameDocument::AddGameObject(&gameDocument, result);

            return result;
        }
    }
}