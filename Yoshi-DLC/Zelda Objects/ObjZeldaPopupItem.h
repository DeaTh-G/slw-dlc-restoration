#pragma once

namespace app
{
    inline static int RUPEE_TO_ANIMAL[] = { 1, 5, 20, 50, 200 };

    class ObjZeldaPopupItem;
    enum class ObjZeldaPopupItemType : char
    {
        RUPEE_GREEN,
        RUPEE_BLUE,
        RUPEE_RED,
        RUPEE_PURPLE,
        RUPEE_GOLD,
        HEART
    };

    namespace zelda_popupitem
    {
        inline static const char* MODEL_NAMES[] =
        {
            "zdlc03_obj_rupee_green",
            "zdlc03_obj_rupee_blue",
            "zdlc03_obj_rupee_red",
            "zdlc03_obj_rupee_purple",
            "zdlc03_obj_rupee_gold",
            "zdlc03_obj_heart"
        };

        inline static const char* SKELETON_NAMES[] =
        {
            "zdlc03_obj_rupee",
            "zdlc03_obj_heart"
        };

        inline static const char* ANIMATION_NAMES[] =
        {
            "zdlc03_obj_rupee_popup",
            "zdlc03_obj_heart_popup"
        };

        struct ZeldaPopupItemCinfo
        {
            csl::math::Matrix34 Transform{};
            ObjZeldaPopupItemType Type{};
            int PlayerNumber{};
            int field_48{};
            int field_4C{};

            ZeldaPopupItemCinfo() {}
            ZeldaPopupItemCinfo(csl::math::Matrix34* transform, ObjZeldaPopupItemType type, int playerNo)
            {
                Transform = *transform;
                Type = type;
                PlayerNumber = playerNo;
            }
        };

        ObjZeldaPopupItem* CreateZeldaPopupItem(GameDocument& gameDocument, ZeldaPopupItemCinfo* cInfo);
    }

    class ObjZeldaPopupItemInfo : public CObjInfo
    {
    public:
        int Models[6];
        int Skeletons[2];
        int Animations[2];
        GameObjectHandleBase PopupHandle;

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            for (size_t i = 0; i < 6; i++)
                ObjUtil::GetModelResource(&Models[i], zelda_popupitem::MODEL_NAMES[i], &packFile);

            for (size_t i = 0; i < 2; i++)
                ObjUtil::GetSkeletonResource(&Skeletons[i], zelda_popupitem::SKELETON_NAMES[i], packFile);

            for (size_t i = 0; i < 2; i++)
                ObjUtil::GetAnimationResource(&Animations[i], zelda_popupitem::ANIMATION_NAMES[i], &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjZeldaPopupItemInfo";
        }

        void SetPopupItem(ObjZeldaPopupItem* object);
    };

    class ObjZeldaPopupItem : public GameObject3D
    {
    public:
        zelda_popupitem::ZeldaPopupItemCinfo* CInfo = new zelda_popupitem::ZeldaPopupItemCinfo();
        int field_320{};
        ObjZeldaPopupItemType Type{};
        int PlayerNumber{};
        Effect::CEffectHandle EffectHandle{};
        int field_338{};
        int field_33C{};

        ObjZeldaPopupItem(zelda_popupitem::ZeldaPopupItemCinfo* cInfo)
        {
            *CInfo = *cInfo;
            Type = cInfo->Type;
            PlayerNumber = cInfo->PlayerNumber;
        }

        void Destructor(size_t deletingFlags)
        {
            delete CInfo;

            GameObject3D::Destructor(deletingFlags);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);

            ObjZeldaPopupItemInfo* info = (ObjZeldaPopupItemInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjZeldaPopupItemInfo");
            bool isHeart = Type == ObjZeldaPopupItemType::HEART ? true : false;
            info->SetPopupItem(this);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
            {
                csl::math::Quaternion rotation = GetRotationFromMatrix(&CInfo->Transform);

                fnd::GOCTransform::SetLocalTranslation(gocTransform, (csl::math::Vector3*) & (CInfo->Transform.data[3][0]));
                fnd::GOCTransform::SetLocalRotation(gocTransform, &rotation);
            }

            fnd::GOComponent::BeginSetup(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                csl::math::Vector3 position{ 0, 3.0f, 0 };
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->Models[(int)Type];
                visualDescriptor.Skeleton = info->Skeletons[0];
                if (isHeart)
                    visualDescriptor.Skeleton = info->Skeletons[1];
                visualDescriptor.Animation |= 0x400000;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
                if (gocAnimation)
                {
                    int animCount = 1;
                    game::GOCAnimationSimple::Setup(gocAnimation, &animCount);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    if (!isHeart)
                        game::GOCAnimationSimple::Add(gocAnimation, "POPUP", info->Animations[0], 0);
                    else
                        game::GOCAnimationSimple::Add(gocAnimation, "POPUP", info->Animations[1], 0);
                    game::GOCAnimationSimple::SetAnimation(gocAnimation, "POPUP");
                }
            }

            game::GOCEffect::SimpleSetupEx(this, 1, 1);
            game::GOCSound::SimpleSetup(this, 0, 0);

            fnd::GOComponent::EndSetup(this);
            CInfo = nullptr;

            Effect::CEffectHandle effectHandle{};
            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;

            game::EffectCreateInfo effectInfo;
            game::EffectCreateInfo::__ct(&effectInfo);
            effectInfo.Name = "ef_dl3_itemget_locus";
            effectInfo.field_04 = 1;
            effectInfo.field_08 = 1;
            effectInfo.Position = Vector3(0, 0, 0);
            effectInfo.Rotation = Quaternion(0, 0, 0, 1);
            effectInfo.field_30 = 0;
            effectInfo.field_40 = GameObject::GetGOC(this, GOCVisual);
            effectInfo.field_44 = "Body";
            effectInfo.field_48 = -1;

            game::GOCEffect::CreateEffectLoopEx(gocEffect, &effectHandle, &effectInfo);
            EffectHandle = effectHandle;

            int deviceTag[3];
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            if (isHeart)
                game::GOCSound::Play(gocSound, deviceTag, "obj_zeldaheart_get", 0);
            else
                game::GOCSound::Play(gocSound, deviceTag, "obj_zeldarupy_get", 0);


            if (Type == ObjZeldaPopupItemType::HEART)
                return;

            xgame::MsgGetAnimal animalMessage{ RUPEE_TO_ANIMAL[(char)Type] };
            ObjUtil::SendMessageImmToGameActor(this, &animalMessage);
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], PlayerNumber);
            if (!playerInfo)
                return;

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            fnd::GOCTransform::SetLocalTranslation(gocTransform, (csl::math::Vector3*)(playerInfo + 4));
            fnd::GOCTransform::SetLocalRotation(gocTransform, (csl::math::Quaternion*)(playerInfo + 8));

            int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
            if (!gocAnimation)
                return;

            if (game::GOCAnimationSimple::IsFinished(gocAnimation))
                ObjZeldaPopupItem::toKill();
        }

        void EndPopup() { toKill(); }

    private:
        void toKill()
        {
            GameObject::Kill(this);
        }
    };

    namespace zelda_popupitem
    {
        inline static ObjZeldaPopupItem* CreateZeldaPopupItem(GameDocument& gameDocument, ZeldaPopupItemCinfo* cInfo)
        {
            ObjZeldaPopupItem* object = new ObjZeldaPopupItem(cInfo);
            if (!object)
                return 0;
            GameDocument::AddGameObject(*(GameDocument**)&gameDocument, object);
            return object;
        }
    }

    inline void ObjZeldaPopupItemInfo::SetPopupItem(ObjZeldaPopupItem* object)
    {
        ObjZeldaPopupItem* popupItem = (ObjZeldaPopupItem*)PopupHandle.Get();
        if (popupItem)
            popupItem->EndPopup();

        PopupHandle = object;
    }
}