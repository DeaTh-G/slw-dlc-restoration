#pragma once

namespace app
{
    inline static const char* ZELDA_BUSH_NAMES[] = { "zdlc03_obj_bushA", "zdlc03_obj_bushB" };
    inline static const char* ZELDA_BUSH_SHADOW_NAMES[] = { "zdlc03_obj_bushA_shadow", "zdlc03_obj_bushA_shadow" };
    inline static int ZELDA_BUSH_ODDS[] = { 35, 20, 15, 30};
    inline static ObjZeldaPopupItemType ZELDA_BUSH_ITEMS[] =
    {
        ObjZeldaPopupItemType::RUPEE_GREEN,
        ObjZeldaPopupItemType::RUPEE_BLUE,
        ObjZeldaPopupItemType::RUPEE_RED,
        ObjZeldaPopupItemType::HEART
    };

    enum class ObjZeldaBushType : char
    {
        A,
        B
    };

    struct ObjZeldaBushData
    {
        ObjZeldaBushType CreateType;
    };

    class ObjZeldaBushInfo : public CObjInfo
    {
    public:
        int Models[2]{};
        int Skeletons[2]{};
        int Animations[2]{};
        int Shadows[2]{};

        ObjZeldaBushInfo() {}

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            for (size_t i = 0; i < 2; i++)
            {
                ObjUtil::GetModelResource(&Models[i], ZELDA_BUSH_NAMES[i], &packFile);
                ObjUtil::GetSkeletonResource(&Skeletons[i], ZELDA_BUSH_NAMES[i], packFile);
                ObjUtil::GetAnimationResource(&Animations[i], ZELDA_BUSH_NAMES[i], &packFile);
                ObjUtil::GetModelResource(&Shadows[i], ZELDA_BUSH_SHADOW_NAMES[i], &packFile);
            }
        }

        const char* GetInfoName() override
        {
            return "ObjZeldaBushInfo";
        }

        void RegistCallback(int& container) override
        {
            ObjZeldaPopupItemInfo* popupObject = new(pAllocator) ObjZeldaPopupItemInfo();
            if (popupObject)
                CObjInfoContainer::Register(&container, popupObject->GetInfoName(), popupObject);
        }
    };

    class ObjZeldaBush : public CSetObjectListener
    {
        ObjZeldaBushType Type{};
        INSERT_PADDING(12);

    public:
        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            fnd::GOComponent::Create(this, GOCCollider);
            fnd::GOComponent::Create(this, GOCShadow);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCEffect);

            ObjZeldaBushInfo* info = (ObjZeldaBushInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjZeldaBushInfo");
            ObjZeldaBushData* data = (ObjZeldaBushData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
            Type = data->CreateType;

            fnd::GOComponent::BeginSetup(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;
                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

                visualDescriptor.Model = info->Models[(int)Type];
                visualDescriptor.Skeleton = info->Skeletons[(int)Type];
                visualDescriptor.Animation |= 0x400000;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
                if (gocAnimation)
                {
                    int animCount = 1;
                    game::GOCAnimationSimple::Setup(gocAnimation, &animCount);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    game::GOCAnimationSimple::Add(gocAnimation, "Touch", info->Animations[(int)Type], 0);
                }
            }

            int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
            if (gocCollider)
            {
                csl::math::Vector3 sizeA { 7.5f, 7.5f, 7.5f };
                csl::math::Vector3 sizeB { 11, 11, 11 };
                csl::math::Vector3 offset{};
                offset.Y = Type == ObjZeldaBushType::A ? 11 : 15;
                game::ColliBoxShapeCInfo collisionInfo;

                int shapeCount = 1;
                game::GOCCollider::Setup(gocCollider, &shapeCount);
                game::CollisionObjCInfo::__ct(&collisionInfo);

                collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
                collisionInfo.MotionType = 2;
                collisionInfo.Size = Type == ObjZeldaBushType::A ? sizeA : sizeB;
                ObjUtil::SetupCollisionFilter(0, &collisionInfo);
                collisionInfo.field_04 |= 1;
                game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &offset);

                game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
            }

            int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
            if (gocShadow)
            {
                game::ShadowModelShapeCInfo shadowInfo;

                game::ShadowModelShapeCInfo::__ct(&shadowInfo, Type == ObjZeldaBushType::A ? &info->Shadows[0] : &info->Shadows[1]);
                shadowInfo.field_04 = 6;

                game::ShadowModelShapeCInfo* ppShadowInfo = &shadowInfo;
                game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
            }

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

            fnd::GOComponent::EndSetup(this);
        }

        bool ProcessMessage(fnd::Message& message) override
        {
            if (PreProcessMessage(message))
                return true;

            switch (message.Type)
            {
            case fnd::PROC_MSG_HIT_EVENT_COLLISION:
                ProcMsgHitEventCollision((xgame::MsgHitEventCollision&)message);
                return true;
            case fnd::PROC_MSG_DAMAGE:
                ProcMsgDamage((xgame::MsgDamage&)message);
                return true;
            default:
                return CSetObjectListener::ProcessMessage(message);
            }
        }

    private:
        void ProcMsgHitEventCollision(xgame::MsgHitEventCollision& message)
        {
            int deviceTag[3];
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play3D(gocSound, deviceTag, "obj_bush", 0);

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            if (!game::GOCAnimationSimple::IsCurrentAnimaton(gocAnimation, "Touch") ||
                (game::GOCAnimationSimple::IsFinished(gocAnimation)) != 0)
            {
                game::GOCAnimationSimple::SetAnimation(gocAnimation, "Touch");
                Resume();
            }
        }

        void ProcMsgDamage(xgame::MsgDamage& message)
        {
            if (!(message.AttackType & 0x40))
                return;

            unsigned int random = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));
            random = floorf((random * 2.328306436538696e-10f) * 100);

            int deviceTag[3];
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play(gocSound, deviceTag, "obj_zeldabush_cut", 0);

            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;

            app::game::GOCEffect::CreateEffect(gocEffect, Type == ObjZeldaBushType::A ? "ef_dl3_bush_break_a" : "ef_dl3_bush_break_b");

            SetStatusRetire();
            GameObject::Kill();

            if (random < 60)
            {
                unsigned int typeRand = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));
                typeRand = floorf((typeRand * 2.328306436538696e-10f) * 100);

                int ModelType = 0;
                int limit = 0;
                for (size_t i = 0; i < 4; i++)
                {
                    limit += ZELDA_BUSH_ODDS[i];
                    if (typeRand > limit)
                    {
                        ModelType++;
                        continue;
                    }

                    int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], message.field_50);
                    if (!playerInfo)
                        return;

                    csl::math::Matrix34 playerMatrix{};
                    math::Matrix34AffineTransformation(&playerMatrix, (csl::math::Vector3*)(playerInfo + 4), (csl::math::Quaternion*)(playerInfo + 8));
                    zelda_popupitem::ZeldaPopupItemCinfo popupInfo{ &playerMatrix, ZELDA_BUSH_ITEMS[ModelType], message.field_50 };
                    zelda_popupitem::CreateZeldaPopupItem((GameDocument&)field_24[1], &popupInfo);
                    return;
                }
            }
        }
    };

    inline static ObjZeldaBush* create_ObjZeldaBush() { return new ObjZeldaBush(); }

    inline static ObjZeldaBushInfo* createObjInfo_ObjZeldaBushInfo(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) ObjZeldaBushInfo();
    }
}