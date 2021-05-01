#pragma once

namespace app
{
    class ObjBreakRoofInfo : public CObjInfo
    {
    public:
        int Model;
        int Skeleton;
        int Collision;
        debris::ResRandomSpaceDebris Debris;

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&Model, "zdlc03_obj_lid", &packFile);
            ObjUtil::GetSkeletonResource(&Skeleton, "zdlc03_obj_lid", packFile);
            ObjUtil::GetPhysicsMeshResource(&Collision, "zdlc03_obj_lid", packFile);

            debris::ResRandomSpaceDebris::SDesc description{};
            description.Name = "zdlc03_obj_lid_brk";
            description.Packfile = packFile;
            debris::ResRandomSpaceDebris::Build(&Debris, &description);
        }

        const char* GetInfoName() override
        {
            return "ObjBreakRoofInfo";
        }
    };

    class ObjBreakRoof : public CSetObjectListener
    {
    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCPhysics);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCCollider);

            ObjBreakRoofInfo* info = (ObjBreakRoofInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjBreakRoofInfo");

            fnd::GOComponent::BeginSetup(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;
                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                visualDescriptor.field_08 = 1;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                game::ColliBoxShapeCInfo colliderInfo;
                csl::math::Vector3 offset { 0, -1.5f, 0 };

                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&colliderInfo);

                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                colliderInfo.MotionType = 2;
                colliderInfo.Size = csl::math::Vector3(15, 11, 30);
                ObjUtil::SetupCollisionFilter(0, &colliderInfo);
                game::CollisionObjCInfo::SetLocalPosition(&colliderInfo, &offset);
                colliderInfo.field_04 |= 1;
                game::GOCCollider::CreateShape(gocCollider, &colliderInfo);
            }

            int* gocPhysics = GameObject::GetGOC(this, GOCPhysicsString);
            if (gocPhysics)
            {
                int shapeCount = 1;
                game::ColliMeshShapeCInfo colliderInfo;
                csl::math::Vector3 offset{ 0, -9, 0 };

                game::GOCPhysics::Setup(gocPhysics, &shapeCount);
                game::CollisionObjCInfo::__ct(&colliderInfo);

                colliderInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_MESH;
                colliderInfo.MotionType = 2;
                colliderInfo.Mesh = (int*)info->Collision;
                ObjUtil::SetupCollisionFilter(0, &colliderInfo);
                colliderInfo.field_02 = 1;
                game::GOCPhysics::CreateShape(gocPhysics, &colliderInfo);
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
            if (!AttackType::IsDamaged(message.AttackType, 9))
                return;

            csl::math::Matrix34 matrix{};

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (!gocVisual)
                return;

            matrix = *(csl::math::Matrix34*)(gocTransform + 0x44);
            for (size_t i = 0; i < 9; i++)
            {
                math::Transform transform{};
                fnd::GOCVisualModel::GetNodeTransformID(gocVisual, 0, i, &transform);

                csl::math::Quaternion rotation{};
                math::CalculatedTransform::GetQuaternionRotation(&matrix, &rotation);

                debris::SRandomSpaceDebrisInfo debrisInfo{};
                debrisInfo.field_00 = transform.Position;
                debrisInfo.field_10 = rotation;
                debrisInfo.field_40 = 400;
                debrisInfo.field_44 = 2;
                debrisInfo.field_48 = 0.5f;
                debrisInfo.field_4C |= 1;
                debrisInfo.field_50 = message.field_30;
                debrisInfo.field_60 = *(csl::math::Vector3*) & matrix.data[1][0];
                debrisInfo.field_70 = 5;
                debrisInfo.field_74 = 1;
                debrisInfo.field_4C |= 4;
                debrisInfo.field_A0 = 0;
                debrisInfo.field_A4 = 5;
                debrisInfo.field_4C |= 2;
                debrisInfo.field_80 = debrisInfo.field_00;
                debrisInfo.field_90 = 1;
                ObjBreakRoofInfo* info = (ObjBreakRoofInfo*)ObjUtil::GetObjectInfo((GameDocument*)field_24[1], "ObjBreakRoofInfo");
                debrisInfo.SetModelRes(&info->Debris, 24);

                debrisInfo.field_C0 = 3;
                debrisInfo.field_C4 = 15;
                debrisInfo.field_C8 = debrisInfo.field_C4;
                debrisInfo.field_CC = 10;

                debrisUtil::CreateRandomSpaceDebris((GameDocument*)field_24[1], &debrisInfo);
            }

            int deviceTag[3];
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play3D(gocSound, deviceTag, "obj_zeldabreakfloor", 0);

            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;

            app::game::GOCEffect::CreateEffect(gocEffect, "ef_dl3_lid_break");

            int playerNo = ObjUtil::GetPlayerNo(field_24[1], message.field_08);
            if (playerNo >= 0)
            {
                xgame::MsgShakeCamera shakeMessage { 3, 0.2f };
                ObjUtil::SendMessageImmToCamera(this, playerNo, &shakeMessage);
            }

            message.SetReply((csl::math::Vector3*)(gocTransform + 0x18), 1);
            CSetObjectListener::SetStatusRetire(this);
            GameObject::Kill(this);
        }
    };

    inline static ObjBreakRoof* create_ObjBreakRoof() { return new ObjBreakRoof(); }

    inline static ObjBreakRoofInfo* createObjInfo_ObjBreakRoofInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjBreakRoofInfo();
    }
}