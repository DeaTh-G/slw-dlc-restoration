#pragma once

namespace app
{
    class ObjOccludeStoneInfo : public CObjInfo
    {
    public:
        int Model;
        int ShadowModel;
        int Collision;
        debris::ResRandomSpaceDebris Debris;

        ObjOccludeStoneInfo() {}

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&Model, "zdlc03_obj_breakwall", &packFile);
            ObjUtil::GetModelResource(&ShadowModel, "zdlc03_obj_breakwall_shadow", &packFile);
            ObjUtil::GetPhysicsMeshResource(&Collision, "zdlc03_obj_breakwall", packFile);

            debris::ResRandomSpaceDebris::SDesc description{};
            description.Name = "zdlc03_obj_breakwall_brk";
            description.Packfile = packFile;
            debris::ResRandomSpaceDebris::Build(&Debris, &description);
        }

        const char* GetInfoName() override
        {
            return "ObjOccludeStoneInfo";
        }
    };

    class ObjOccludeStone : public CSetObjectListener
    {
    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCShadowSimple);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCCollider);

            ObjOccludeStoneInfo* info = (ObjOccludeStoneInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjOccludeStoneInfo");

            fnd::GOComponent::BeginSetup(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;
                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                visualDescriptor.Model = info->Model;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
            }

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                game::ShadowModelShapeCInfo shadowInfo;

                game::ShadowModelShapeCInfo::__ct(&shadowInfo, &info->ShadowModel);

                game::ShadowModelShapeCInfo* ppShadowInfo = &shadowInfo;
                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                csl::math::Vector3 position { 0, 22.5f, 0 };
                Eigen::Quaternion<float> q;
                q = Eigen::AngleAxis<float>(0.43633232f, Eigen::Vector3f(0, 1, 0));
                csl::math::Quaternion rotation{ q.x(), q.y(), q.z(), q.w() };

                game::ColliBoxShapeCInfo damageColliderInfo;

                int shapeCount = 2;
                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&damageColliderInfo);

                damageColliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                damageColliderInfo.MotionType = 2;
                damageColliderInfo.Size = csl::math::Vector3(30, 45, 25);
                ObjUtil::SetupCollisionFilter(0, &damageColliderInfo);
                damageColliderInfo.field_04 |=  1;
                game::CollisionObjCInfo::SetLocalPosition(&damageColliderInfo, &position);
                game::CollisionObjCInfo::SetLocalRotation(&damageColliderInfo, &rotation);
                game::GOCCollider::CreateShape(gocCollider, &damageColliderInfo);

                game::ColliMeshShapeCInfo colliderInfo;
                game::CollisionObjCInfo::__ct(&colliderInfo);
                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_MESH;
                colliderInfo.MotionType = 2;
                colliderInfo.field_02 = 8;
                colliderInfo.field_04 |= 0x100;
                colliderInfo.field_08 = 27;
                colliderInfo.Mesh = (int*)info->Collision;
                //game::GOCCollider::CreateShape(gocCollider, &colliderInfo);
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

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
            int doDamage{};
            if (message.field_2C == 3 && message.field_28 == 3)
                doDamage = 1;

            if (message.field_54 == 3 && ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], 0) && Transform.Data[0x24] == 7 || doDamage)
            {
                ObjOccludeStoneInfo* info = (ObjOccludeStoneInfo*)ObjUtil::GetObjectInfo((GameDocument*)field_24[1], "ObjOccludeStoneInfo");
                int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
                if (!gocTransform)
                    return;

                csl::math::Vector3 upVec { 0, 1, 0 };
                csl::math::Vector3 scaledUpVec{};
                csl::math::Quaternion rotation{};
                csl::math::Vector3 position{};
                csl::math::Matrix34 transform = *(csl::math::Matrix34*)(gocTransform + 0x44);
                math::CalculatedTransform::GetQuaternionRotation(&transform, &rotation);
                math::Vector3Rotate(&upVec, &rotation, &upVec);
                debris::SRandomSpaceDebrisInfo debrisInfo{};
                math::CalculatedTransform::GetTranslation(&transform, &position);
                math::Vector3Scale(&upVec, 22.5f, &scaledUpVec);
                math::Vector3Add(&position, &scaledUpVec, &position);

                debrisInfo.field_00 = position;
                debrisInfo.field_10 = rotation;
                debrisInfo.field_40 = 400;
                debrisInfo.field_44 = 2;
                debrisInfo.field_48 = 0.5f;
                debrisInfo.field_4C |= 1;
                debrisInfo.field_50 = message.field_30;
                debrisInfo.field_60 = upVec;
                debrisInfo.field_70 = 2;
                debrisInfo.field_74 = 1;
                debrisInfo.field_4C |= 4;
                debrisInfo.field_A0 = 0;
                debrisInfo.field_A4 = 10;
                debrisInfo.field_4C |= 2;
                debrisInfo.field_80 = debrisInfo.field_00;
                debrisInfo.field_90 = 0.5f;
                debrisInfo.SetModelRes(&info->Debris, 24);
                debrisInfo.field_C0 = 3;
                debrisInfo.field_C4 = 22.5f;
                debrisInfo.field_C8 = 25;
                debrisInfo.field_CC = 15;

                debrisUtil::CreateRandomSpaceDebris((GameDocument*)field_24[1], &debrisInfo);

                int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
                if (!gocEffect)
                    return;

                app::game::GOCEffect::CreateEffect(gocEffect, "ef_dl3_wall_break");

                int deviceTag[3];
                int* gocSound = GameObject::GetGOC(this, GOCSoundString);
                if (!gocSound)
                    return;

                game::GOCSound::Play(gocSound, deviceTag, "obj_zeldapuzzle_solution", 0);
                game::GOCSound::Play3D(gocSound, deviceTag, "obj_zeldarock_break", 0);
                CSetObjectListener::SetStatusRetire(this);
                GameObject::Kill(this);
            }
        }
    };

    inline static ObjOccludeStone* create_ObjOccludeStone() { return new ObjOccludeStone(); }

    inline static ObjOccludeStoneInfo* createObjInfo_ObjOccludeStoneInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjOccludeStoneInfo();
    }
}