#pragma once

namespace app
{
    namespace dekunuts_shot
    {
        struct CreateInfo
        {
            csl::math::Matrix34 Transform;
            int Model;
            float field_44;
            float field_48;
            float field_4C;
        };
    }

    class EnemyDekunutsShot : public GameObject3D
    {
        INSERT_PADDING(4);
        dekunuts_shot::CreateInfo* CInfo;
        INSERT_PADDING(12);
        csl::math::Vector3 field_330;
        float field_340;
        float field_344;
        float field_348;
        int field_34C;

    public:
        EnemyDekunutsShot(dekunuts_shot::CreateInfo* info)
        {
            CInfo = info;
            field_340 = info->field_48;
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