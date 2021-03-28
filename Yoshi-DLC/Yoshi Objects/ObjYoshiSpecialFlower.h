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
            app::ObjUtil::GetPackFile(&packFile, app::ObjUtil::GetStagePackName(&gameDocument));

            app::ObjUtil::GetModelResource(&Model, "zdlc02_obj_goalA_on", &packFile);
            app::ObjUtil::GetSkeletonResource(&Skeleton, "zdlc02_obj_goalA_on", packFile);
            app::ObjUtil::GetAnimationResource(&Animation, "zdlc02_obj_goalA_on_idle", &packFile);
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
            app::game::ColliSphereShapeCInfo collisionInfo;
            game::ShadowHemisphereShapeCInfo shadowInfo;

            app::fnd::GOComponent::Create(this, GOCVisualModel);
            app::fnd::GOComponent::Create(this, GOCAnimationSimple);
            app::fnd::GOComponent::Create(this, GOCCollider);
            app::fnd::GOComponent::Create(this, GOCEffect);
            app::fnd::GOComponent::Create(this, GOCSound);

            ObjYoshiSpecialFlowerInfo* info = (ObjYoshiSpecialFlowerInfo*)app::ObjUtil::GetObjectInfo(gameDocument, "ObjYoshiSpecialFlowerInfo");
            ObjYoshiSpecialFlowerData* data = (ObjYoshiSpecialFlowerData*)app::CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            FlowerID = data->FlowerID;

            app::fnd::GOComponent::BeginSetup(this);

            int* gocVisual = app::GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                app::fnd::GOCVisualModel::VisualDescription visualDescriptor;

                app::fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                app::fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = app::GameObject::GetGOC(this, GOCAnimation);
                if (gocAnimation)
                {
                    int animCount = 1;
                    app::game::GOCAnimationSimple::Setup(gocAnimation, &animCount);
                    app::fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    app::game::GOCAnimationSimple::Add(gocAnimation, "IDLE_LOOP", info->Animation, 1);
                    app::game::GOCAnimationSimple::SetAnimation(gocAnimation, "IDLE_LOOP");
                }
            }

            int* gocCollider = app::GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                int shapeCount = 1;
                app::game::GOCCollider::Setup(gocCollider, &shapeCount);
                app::game::CollisionObjCInfo::__ct(&collisionInfo);
                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
                collisionInfo.MotionType = 2;
                collisionInfo.Radius = 7.5f;
                app::ObjUtil::SetupCollisionFilter(6, &collisionInfo);
                collisionInfo.field_04 |= 1;

                app::game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            app::game::GOCEffect::SimpleSetup(this);
            app::game::GOCSound::SimpleSetup(this, 0, 0);

            app::fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::MessageNew& message) override
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

            GameObject::Kill(this);
            GameObject::SetStatusRetire(this);
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