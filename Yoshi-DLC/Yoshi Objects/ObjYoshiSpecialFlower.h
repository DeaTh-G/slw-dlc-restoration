#pragma once

namespace app
{
    struct ObjYoshiSpecialFlowerData
    {
        unsigned int FlowerID;
    };

    class ObjYoshiSpecialFlowerInfo : public CObjInfo
    {
    public:
        int Model{};
        int Skeleton{};
        int Animation{};

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&Model, "zdlc02_obj_goalA_on", &packFile);
            ObjUtil::GetSkeletonResource(&Skeleton, "zdlc02_obj_goalA_on", packFile);
            ObjUtil::GetAnimationResource(&Animation, "zdlc02_obj_goalA_on_idle", &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjYoshiSpecialFlowerInfo";
        }
    };

    class ObjYoshiSpecialFlower : public CSetObjectListener
    {
        unsigned int FlowerID;
        INSERT_PADDING(12);
   
    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            game::ColliSphereShapeCInfo collisionInfo;
            game::ShadowHemisphereShapeCInfo shadowInfo;

            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);

            ObjYoshiSpecialFlowerInfo* info = (ObjYoshiSpecialFlowerInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjYoshiSpecialFlowerInfo");
            ObjYoshiSpecialFlowerData* data = (ObjYoshiSpecialFlowerData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            FlowerID = data->FlowerID;

            fnd::GOComponent::BeginSetup(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
                if (gocAnimation)
                {
                    int animCount = 1;
                    game::GOCAnimationSimple::Setup(gocAnimation, &animCount);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    game::GOCAnimationSimple::Add(gocAnimation, "IDLE_LOOP", info->Animation, 1);
                    game::GOCAnimationSimple::SetAnimation(gocAnimation, "IDLE_LOOP");
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 7.5f;
                ObjUtil::SetupCollisionFilter(6, &collisionInfo);
                collisionInfo.field_04 |= 1;

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            if (message.Type != fnd::PROC_MSG_HIT_EVENT_COLLISION)
                return CSetObjectListener::ProcessMessage(message);

            ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
            return true;
        }

    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            EggManager* eggManager = EggManager::GetService((GameDocument*)field_24[1]);
            if (eggManager)
                eggManager->TakeYoshiSpecialFlower(FlowerID);

            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (gocEffect)
                game::GOCEffect::CreateEffect(gocEffect, "ef_dl2_goal_get");

            int deviceTag[3];
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (gocSound)
                game::GOCSound::Play(gocSound, deviceTag, "obj_specialflower_get", 0);

            GameObject::Kill();
            SetStatusRetire();
        }
    };

    inline static ObjYoshiSpecialFlower* create_ObjYoshiSpecialFlower()
    {
        return new ObjYoshiSpecialFlower();
    }

    inline static ObjYoshiSpecialFlowerInfo* createObjInfo_ObjYoshiSpecialFlowerInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjYoshiSpecialFlowerInfo();
    }
}