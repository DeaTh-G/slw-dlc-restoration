#pragma once

namespace app
{
    inline static float BREAKFLOOR_LAYOUT[] = { 1, 3, 3, 1 };

    class ObjBreakFloorInfo : public CObjInfo
    {
    public:
        int Model{};
        int Skeleton{};
        int Collision{};
        debris::ResRandomSpaceDebris Debris{};

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&Model, "zdlc03_obj_breakfloor", &packFile);
            ObjUtil::GetPhysicsMeshResource(&Collision, "zdlc03_obj_breakfloor", packFile);

            debris::ResRandomSpaceDebris::SDesc description{};
            description.Name = "zdlc03_obj_breakfloor_brk";
            description.Packfile = packFile;
            debris::ResRandomSpaceDebris::Build(&Debris, &description);
        }

        const char* GetInfoName() override
        {
            return "ObjBreakFloorInfo";
        }
    };

    class ObjBreakFloor : public CSetObjectListener
    {
    public:
        fnd::HFrame Parts[8];
        INSERT_PADDING(16);

        ObjBreakFloor()
        {
            for (fnd::HFrame& part : Parts)
                fnd::HFrame::__ct(&part);
        }

        void Destructor(size_t deletingFlags)
        {
            for (fnd::HFrame& part : Parts)
                fnd::HFrame::__dt(&part, 2);

            CSetObjectListener::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            csl::math::Vector3 offset{};

            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCCollider);

            ObjBreakFloorInfo* info = (ObjBreakFloorInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjBreakFloorInfo");

            fnd::GOComponent::BeginSetup(this);
            
            *((char*)GameObject::GetGOC(this, GOCTransformString) + 0x1D1) = 0;
            fnd::HFrame* transformFrame = (fnd::HFrame*)(GameObject::GetGOC(this, GOCTransformString) + 0x28);

            for (fnd::HFrame& part : Parts)
                fnd::HFrame::AddChild(transformFrame, &part);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                csl::math::Vector3 backVector { 0, 0, -1 };
                math::Vector3Scale(&backVector, 45, &offset);

                fnd::GOCVisualModel::VisualDescription visualDescriptor;
                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.field_08 = 1;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &offset);
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                csl::math::Vector3 position { 0, -9, 0 };
                game::ColliBoxShapeCInfo damageColliderInfo;

                int shapeCount = 9;
                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&damageColliderInfo);

                damageColliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                damageColliderInfo.MotionType = 2;
                damageColliderInfo.Size = csl::math::Vector3(15, 50, 15);
                ObjUtil::SetupCollisionFilter(5, &damageColliderInfo);
                damageColliderInfo.field_04 |= 1;
                game::CollisionObjCInfo::SetLocalPosition(&damageColliderInfo, &position);
                for (size_t i = 0; i < 8; i++)
                {
                    damageColliderInfo.field_0C = i;
                    damageColliderInfo.Parent = &Parts[i];
                    game::GOCCollider::CreateShape(gocCollider, &damageColliderInfo);
                }

                game::ColliMeshShapeCInfo colliderInfo;
                game::CollisionObjCInfo::__ct(&colliderInfo);
                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_MESH;
                colliderInfo.MotionType = 2;
                colliderInfo.field_02 = 1;
                colliderInfo.field_04 |= 0x100;
                colliderInfo.field_0C = 8;
                colliderInfo.Mesh = (int*)info->Collision;
                game::CollisionObjCInfo::SetLocalPosition(&colliderInfo, &offset);
                game::GOCCollider::CreateShape(gocCollider, &colliderInfo);
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

            game::GravityManager* gravityManager = game::GravityManager::GetService((GameDocument*)field_24[1]);
            if (gravityManager)
            {
                csl::math::Matrix34 transform;

                int* object = gravityManager->GetObjectAtPoint(GameObject::GetGOC(this, GOCTransformString) + 0x18);
                ObjUtil::LayoutCylinder layoutCylinder{};
                ObjUtil::LayoutCylinder::Description description { transformFrame, object };
                layoutCylinder.Setup(&description);

                csl::math::Vector3 offset{};;
                float xUp = 26;
                float zDown = -30;
                float xDown = xUp * -0.5f;
                float xOffset{};
                float zOffset{};

                int frameID = 0;
                for (size_t i = 0; i < 4; i++)
                {
                    xOffset = xDown * (BREAKFLOOR_LAYOUT[i] - 1);
                    zOffset = zDown * i;

                    if (!BREAKFLOOR_LAYOUT[i])
                        continue;

                    int count{};
                    while (count < BREAKFLOOR_LAYOUT[i])
                    {
                        offset = Vector3(xOffset, 0, zOffset);
                        if (!count)
                            offset.X = 0;
                        else if (count == 2)
                            offset.X *= -1;

                        layoutCylinder.CalcTransform(&transform, &offset);
                        fnd::HFrame::SetLocalTranslation(&Parts[frameID], (csl::math::Vector3*)&transform.data[0][0]);
                        fnd::HFrame::SetLocalRotation(&Parts[frameID], (csl::math::Quaternion*)&transform.data[1][0]);

                        count++;
                        frameID++;
                    }
                }
            }

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            if (message.Type != fnd::PROC_MSG_DAMAGE)
                return CSetObjectListener::ProcessMessage(message);

            ProcMsgDamage((xgame::MsgDamage&)message);
            return 1;
        }

    private:
        void ProcMsgDamage(xgame::MsgDamage& message)
        {
            if (AttackType::IsDamaged(message.AttackType, 9) || (message.AttackType & 0x400080) == 0x400080
                || message.field_28 == 3 && message.field_2C == 3)
            {
                int deviceTag[3];
                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (!gocSound)
                    return;

                game::GOCSound::Play(gocSound, deviceTag, "obj_zeldapuzzle_solution", 0);
                game::GOCSound::Play3D(gocSound, deviceTag, "obj_zeldarock_break", 0);

                int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
                if (!gocEffect)
                    return;

                app::game::GOCEffect::CreateEffect(gocEffect, "ef_dl3_floor_break");
                message.SetReply(&message.field_30, 1);
                ObjUtil::AddScorePlayerAction(this, "CRYSTAL_FLOOR", message.field_50);

                ObjBreakFloorInfo* info = (ObjBreakFloorInfo*)ObjUtil::GetObjectInfo((GameDocument*)field_24[1], "ObjBreakFloorInfo");
                if (!info)
                    return;

                for (size_t i = 0; i < 8; i++)
                {
                    if (message.AttackType & 1)
                        ObjCrystalFloorBaseUtil::CreateUpDebris((int*)&Parts[i].Transform, &message.field_30, &info->Debris, (GameDocument*)field_24[1]);
                    else
                        ObjCrystalFloorBaseUtil::CreateDownDebris((int*)&Parts[i].Transform, &message.field_30, &info->Debris, (GameDocument*)field_24[1]);
                }

                SetStatusRetire();
                GameObject::Kill();
            }
        }
    };

    inline static ObjBreakFloor* create_ObjBreakFloor() { return new ObjBreakFloor(); }

    inline static ObjBreakFloorInfo* createObjInfo_ObjBreakFloorInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjBreakFloorInfo();
    }
}