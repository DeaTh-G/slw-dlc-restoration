#pragma once

bool IsEggBlockShadowOn;

namespace app
{
    typedef enum
    {
        STATE_IDLE,
        STATE_DAMAGE
    } EggBlockState;

    struct ObjEggBlockData
    {
        int PopEggNum;
        float PopEggRandomAddSpeed;
    };

    class ObjEggBlockInfo : public CObjInfo
    {
    public:
        int Model{};

        ObjEggBlockInfo() { }

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&Model, "zdlc02_obj_eggblock", &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjEggBlockInfo";
        }

        void RegistCallback(int& container) override
        {
            ObjEggInfo* eggObject = (ObjEggInfo*)fnd::ReferencedObject::f_new(sizeof(ObjEggInfo), pAllocator);
            if (eggObject)
                new (eggObject)ObjEggInfo();
            CObjInfoContainer::Register(&container, eggObject->GetInfoName(), eggObject);

            ObjYoshiInfo* yoshiObject = (ObjYoshiInfo*)fnd::ReferencedObject::f_new(sizeof(ObjYoshiInfo), pAllocator);
            if (yoshiObject)
                new (yoshiObject)ObjYoshiInfo();
            CObjInfoContainer::Register(&container, yoshiObject->GetInfoName(), yoshiObject);
        }
    };

    class ObjEggBlock : public CSetObjectListener
    {
    private:
        struct MotorParam
        {
            float field_00;
            float field_04;
            float field_08;
            float field_0C;
            float field_10;
        };

        struct PopEggParam
        {
            csl::math::Vector3 field_00;
            csl::math::Vector3* field_10;
            float field_14;
            int field_18;
        };

        MotorParam InitMotorParam(float a1, float a2, float a3)
        {
            MotorParam result{};
            result.field_00 = a2;
            result.field_04 = 0;
            result.field_08 = a1;
            result.field_10 = a3 * 0.017453292f;
            result.field_0C = (3.1415927f + 3.1415927f) / a1;
            return result;
        }
        
        PopEggParam InitPopEggParam(csl::math::Vector3 a1, float a2)
        {
            PopEggParam result{};
            result.field_00 = a1;
            result.field_10 = &result.field_00;
            result.field_14 = a2;
            result.field_18 = 0;
            return result;
        }

        void ProcMsgDamage(xgame::MsgDamage& message)
        {
            int* handle = fnd::Handle::Get(&message.field_18);
            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (State != STATE_DAMAGE && handle && gocTransform)
            {
                csl::math::Matrix34 m = *(csl::math::Matrix34*)(gocTransform + 0x44);
                Eigen::MatrixXf transformMatrix(4, 4);
                for (size_t i = 0; i < 4; i++)
                    for (size_t j = 0; j < 4; j++)
                        transformMatrix(i, j) = m.data[i][j];
                transformMatrix = transformMatrix.inverse();

                csl::math::Vector3 inversePosition{ transformMatrix(3,0), transformMatrix(3,1), transformMatrix(3,2) };

                //inversePosition.Y *= 2.8f;

                int playerNo = ObjUtil::GetPlayerNo(field_24[1], message.field_08);
                int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
                if (playerInfo)
                {
                    csl::math::Vector3 playerPosition = *(csl::math::Vector3*)(playerInfo + 0x3C);
                    auto locPos = (transformMatrix.transpose() *
                        Eigen::Vector4f(playerPosition.X, playerPosition.Y, playerPosition.Z, 1)).head<3>();
                    csl::math::Vector3 localPosition { locPos.x(), locPos.y(), locPos.z() };

                    csl::math::Vector3 somePos = *(csl::math::Vector3*)(playerInfo + 0xC);
                    auto invPos = (transformMatrix.transpose() *
                        Eigen::Vector4f(somePos.X, somePos.Y, somePos.Z, 1)).head<3>();
                    inversePosition = Vector3(invPos.x(), invPos.y(), invPos.z());

                    if (math::Vector3NormalizeIfNotZero(&inversePosition, &inversePosition))
                    {
                        csl::math::Vector3 downVec = csl::math::Vector3(0, -1, 0);
                        if (*(handle + 0x2F) &&
                            localPosition.Y > 9 &&
                            math::Vector3DotProduct(&inversePosition, &downVec) >= 0 &&
                            AttackType::IsDamaged(message.AttackType, 0xA))
                        {
                            DamageMotor = InitMotorParam(0.80000001f, 0, 0);
                            EggParam = InitPopEggParam(downVec, -5);

                            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                            if (gocSound)
                            {
                                csl::math::Vector3 translation{};
                                int deviceTag[3];

                                app::game::GOCSound::Play(gocSound, deviceTag, "obj_yossyeggblock_hit", 0);
                                math::CalculatedTransform::GetTranslation((csl::math::Matrix34*)(gocTransform + 0x44), &translation);
                                xgame::MsgDamage::SetReply(&message, &translation, 0);
                                State = STATE_DAMAGE;
                            }
                        }
                        else if (!*(handle + 0x2F) &&
                            localPosition.Y < 0 &&
                            (message.AttackType & 0x20 || message.AttackType & 0x40))
                        {
                            if (math::Vector3NormalizeIfNotZero(&localPosition, &localPosition)
                                && acosf(math::Vector3DotProduct(&localPosition, &localPosition)) < 1.3962634f);
                            {
                                csl::math::Vector3 crossVector{};
                                csl::math::Vector3 upVec = csl::math::Vector3(0, 1, 0);
                                math::Vector3CrossProduct(&upVec, &localPosition, &crossVector);
                                if (math::Vector3NormalizeIfNotZero(&crossVector, &crossVector))
                                {
                                    csl::math::Vector3 zVector{ 0, 0, 1 };

                                    DamageMotor = InitMotorParam(1, -math::Vector3DotProduct(&zVector, &crossVector), 30);
                                    EggParam = InitPopEggParam(upVec, 10);

                                    int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                                    if (gocSound)
                                    {
                                        csl::math::Vector3 translation{};
                                        int deviceTag[3];

                                        app::game::GOCSound::Play(gocSound, deviceTag, "obj_yossyeggblock_hit", 0);
                                        math::CalculatedTransform::GetTranslation((csl::math::Matrix34*)(gocTransform + 0x44), &translation);
                                        xgame::MsgDamage::SetReply(&message, &translation, 0);
                                        State = STATE_DAMAGE;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        void DoCheckPopEgg()
        {
            DroppedEggCInfo eggInfo{};

            if (PopEggNum <= 1 || EggParam.field_18 ||
                DamageMotor.field_04 < (DamageMotor.field_08 * 0.25))
                return;

            csl::math::Vector3 upVec{ 0, 1, 0 };
            csl::math::Quaternion rotation = GetRotationFromMatrix(&Parent.Transform);
            csl::math::Vector3 someVector{};
            csl::math::Vector3 rotationVector{};
            csl::math::Vector3 scaledRotation{};
            math::Vector3Rotate(&someVector, &rotation, &upVec);
            math::Vector3Rotate(&rotationVector, &rotation, &EggParam.field_00);
            math::Vector3Scale(&someVector, 4.5f, &someVector);
            math::Vector3Add((csl::math::Vector3*) & Parent.Transform.data[3][0], &someVector, &someVector);
            math::Vector3Scale(&rotationVector, 4.5f, &scaledRotation);
            math::Vector3Add(&someVector, &scaledRotation, &someVector);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            csl::math::Quaternion objectRotation{};
            math::CalculatedTransform::GetQuaternionRotation((csl::math::Matrix34*)(gocTransform + 0x44), &objectRotation);
            unsigned int random = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));
            random = floorf((random * 2.328306436538696e-10) * 100);

            int ModelType = 0;
            for (size_t i = 0; i < 4; i++)
            {
                if (random < 40)
                {
                    random = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));
                    random = floorf((random * 2.328306436538696e-10) * 100);
                    ModelType++;
                    continue;
                }

                csl::math::Matrix34 someMatrix{};

                game::PathEvaluator::__ct(&eggInfo.PathEvaluator);
                fnd::HandleBase::__as(&eggInfo.PathEvaluator, &PathEvaluator);
                eggInfo.PathEvaluator.field_08 = PathEvaluator.field_08;
                eggInfo.PathEvaluator.field_0C = PathEvaluator.field_0C;
                math::Matrix34AffineTransformation(&someMatrix, &someVector, &objectRotation);
                eggInfo.Transform = someMatrix;
                eggInfo.ModelType = ModelType;
                eggInfo.ZIndex = PopEggNum;

                unsigned int randomEgg = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));
                randomEgg = floorf((randomEgg * 2.328306436538696e-10) * 3);
                if (random >= INT_MAX - 47)
                    continue;

                csl::math::Vector3 eggVector{};
                math::Vector3Scale(&rotationVector, ((PopEggRandomAddSpeed / 3) * randomEgg + 40), &eggVector);
                eggInfo.field_40 = eggVector;
                egg::CreateDroppedEgg((GameDocument&)field_24[1], &eggInfo);

                PopEggNum--;
                EggParam.field_18 = 1;
                CSetObjectListener::SetExtUserData(this, 0, PopEggNum);

                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (!gocSound)
                    continue;

                int deviceTag[3];
                app::game::GOCSound::Play3D(gocSound, deviceTag, "obj_yossyegg_appear", 0);
                return;
            }
        }

    public:
        fnd::HFrame Parent{};
        MotorParam IdleMotor{};
        MotorParam DamageMotor{};
        EggBlockState State{};
        INSERT_PADDING(4);
        PopEggParam EggParam{};
        int PopEggNum{};
        float PopEggRandomAddSpeed{};
        game::PathEvaluator PathEvaluator{};
        INSERT_PADDING(8);

        ObjEggBlock()
        {
            fnd::HFrame::__ct(&Parent);
            game::PathEvaluator::__ct(&PathEvaluator);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOCVisualModel::VisualDescription visualDescriptor;
            game::ColliBoxShapeCInfo collisionInfo{};
            csl::math::Vector3 visualOffset { 0, 5, 0 };
            int unit = 3;
            
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCSound);

            ObjEggBlockData* data = (ObjEggBlockData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            ObjEggBlockInfo* info = (ObjEggBlockInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjEggBlockInfo");

            uint32_t extUserData = CSetObjectListener::GetExtUserData(this, 0);
            if (!extUserData)
                extUserData = data->PopEggNum + 1;
            PopEggNum = extUserData;
            PopEggRandomAddSpeed = data->PopEggRandomAddSpeed;

            fnd::GOComponent::BeginSetup(this);

            fnd::HFrame* transformFrame = (fnd::HFrame*)(GameObject::GetGOC(this, GOCTransformString) + 0x28);
            fnd::HFrame::AddChild(transformFrame, &Parent);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Parent = &Parent;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &visualOffset);
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                csl::math::Vector3 collisionOffset{};
                csl::math::Vector3 collisionSize{};

                /* Damage Collision Layer */
                game::GOCCollider::Setup(gocCollider, &unit);

                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                collisionInfo.MotionType = 2;
                collisionInfo.Size = csl::math::Vector3(5, 0.65f, 5);
                collisionOffset = csl::math::Vector3(0, 0.65f, 0);
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &collisionOffset);
                ObjUtil::SetupCollisionFilter(0, &collisionInfo);
                collisionInfo.field_0C = 0;
                collisionInfo.field_04 |= 1;
                
                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

                /* Unknown Collision Layer */
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                collisionInfo.MotionType = 2;
                collisionInfo.Size = csl::math::Vector3(3, 8, 3);
                collisionOffset = csl::math::Vector3(0, 13, 0);
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &collisionOffset);
                ObjUtil::SetupCollisionFilter(0, &collisionInfo);
                collisionInfo.field_0C = 1;
                collisionInfo.field_04 |= 1;

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

                /* Solid Collision Layer */
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                collisionInfo.MotionType = 2;
                collisionInfo.field_44 = 0;
                collisionInfo.field_48 = 0;
                collisionInfo.Size = csl::math::Vector3(4.5f, 4.5f, 4.5f);
                collisionOffset = csl::math::Vector3(0, 4.5f, 0);
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &collisionOffset);
                collisionInfo.field_02 = 4;
                collisionInfo.field_04 |= 0x100;
                collisionInfo.field_08 = 0x4003;
                collisionInfo.field_0C = 2;

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            game::GOCSound::SimpleSetup(this, 1, 0);
            IdleMotor = InitMotorParam(1.8f, 0, 8);

            void* pathManager = game::PathManager::GetService(gameDocument);
            if (pathManager)
            {
                game::PathRaycastInput inputRay{};
                game::PathRaycastOutput outputRay{};

                CSetAdapter::GetPosition(*(int**)((char*)this + 0x324), &inputRay.StartPostition);
                inputRay.EndPosition = inputRay.StartPostition;
                inputRay.EndPosition.Y -= 400;
                inputRay.field_20 = 1;
                inputRay.field_24 = 1;

                outputRay.field_0C = 1;

                if (game::PathManager::CastRay(pathManager, &inputRay, &outputRay))
                {
                    game::PathEvaluator::SetPathObject(&PathEvaluator, outputRay.PathObject);
                    game::PathEvaluator::SetDistance(&PathEvaluator, outputRay.Distance);
                }
            }

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            if (message.field_04 != fnd::PROC_MSG_DAMAGE)
                return CSetObjectListener::ProcessMessage(message);
            
            ProcMsgDamage((xgame::MsgDamage&)message);
            return true;
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            float radianRotation = IdleMotor.field_10 * sinf(IdleMotor.field_0C * (IdleMotor.field_04 + IdleMotor.field_00));

            if (State == STATE_IDLE)
            {
                Eigen::Quaternion<float> q;
                q = Eigen::AngleAxis<float>(radianRotation, Eigen::Vector3f(0, 0, 1));
                csl::math::Quaternion rotation { q.x(), q.y(), q.z(), q.w() };

                IdleMotor.field_04 += updateInfo.deltaTime;
                fnd::HFrame::SetLocalRotation(&Parent, &rotation);

                int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
                if (gocCollider)
                {
                    void* collider = game::GOCCollider::GetShapeById(gocCollider, 2);
                    game::ColliShapeBase::SetLocalRotation(collider, &rotation);
                }
            }
            else if (State == STATE_DAMAGE)
            {
                radianRotation = DamageMotor.field_10 * sinf(DamageMotor.field_0C * (DamageMotor.field_04 + DamageMotor.field_00));
                csl::math::Vector3 translation{};
                translation.Y += fabs(sinf(DamageMotor.field_0C * (DamageMotor.field_04 + DamageMotor.field_00))) * EggParam.field_14;

                fnd::HFrame::SetLocalTranslation(&Parent, &translation);

                DoCheckPopEgg();
                if (DamageMotor.field_04 > DamageMotor.field_08 * 0.5f)
                {
                    State = STATE_IDLE;
                    IdleMotor = InitMotorParam(1.8f, 0, 8);
                }

                Eigen::Quaternion<float> q;
                q = Eigen::AngleAxis<float>(DamageMotor.field_00 * radianRotation, Eigen::Vector3f(0, 0, 1));
                csl::math::Quaternion rotation{ q.x(), q.y(), q.z(), q.w() };

                DamageMotor.field_04 += updateInfo.deltaTime;
                fnd::HFrame::SetLocalRotation(&Parent, &rotation);

                int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
                if (gocCollider)
                {
                    void* collider = game::GOCCollider::GetShapeById(gocCollider, 2);
                    game::ColliShapeBase::SetLocalRotation(collider, &rotation);
                }
            }
        }
    };

    GameObject* create_ObjEggBlock()
    {
        return new ObjEggBlock();
    }

    ObjEggBlockInfo* createObjInfo_ObjEggBlockInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjEggBlockInfo();
    }
}