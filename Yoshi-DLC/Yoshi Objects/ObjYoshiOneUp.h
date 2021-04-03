#pragma once

namespace app
{
    class ObjYoshiOneUp : public GameObject3D
    {
    public:
        class CInfo
        {
        public:
            int PlayerNo{};
            int field_04{};
            int field_08{};
            int field_0C{};
            csl::math::Vector3 Position{ 0, 0, 0 };
            csl::math::Quaternion Rotation{ 0, 0, 0, 1 };

            CInfo() { }

            CInfo(int playerNo, csl::math::Vector3 position, csl::math::Quaternion rotation)
            {
                PlayerNo = playerNo;
                Position = position;
                Rotation = rotation;
            }
        };

        int field_318{};
        int field_31C{};
        CInfo* Info = new CInfo();
        int PlayerNo{};
        int field_328{};
        int field_32C{};

        ObjYoshiOneUp(CInfo& info)
        {
            Info = &info;
            PlayerNo = info.PlayerNo;
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            fnd::GOComponent::Create(this, GOCEffect);
            fnd::GOComponent::Create(this, GOCSound);
            fnd::GOComponent::Create(this, GOCMovementComplex);

            CObjOneUpInfo* info = (CObjOneUpInfo*)ObjUtil::GetObjectInfo(gameDocument, "CObjOneUpInfo");

            fnd::GOComponent::BeginSetup(this);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (gocTransform)
            {
                csl::math::Vector3 upVec{ 0, 1, 0 };
                csl::math::Vector3 position{};
                math::Vector3Rotate(&position, &Info->Rotation, &upVec);
                math::Vector3Scale(&position, 11, &position);
                math::Vector3Add(&Info->Position, &position, &position);

                fnd::GOCTransform::SetLocalTranslation(gocTransform, &position);
                fnd::GOCTransform::SetLocalRotation(gocTransform, &Info->Rotation);
            }

            int* gocVisual = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
            if (gocVisual)
            {
                csl::math::Vector3 scale { 0.7f, 0.7f, 0.7f };
                fnd::GOCVisualModel::VisualDescription visualDescriptor{};

                visualDescriptor.Model = info->Model;
                visualDescriptor.Skeleton = info->Skeleton;
                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
                fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);

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

            game::GOCEffect::SimpleSetup(this);
            game::GOCSound::SimpleSetup(this, 0, 0);

            fnd::GOComponent::EndSetup(this);

            int deviceTag[3]{};
            int* gocSound = GameObject::GetGOC(this, GOCSoundString);
            if (!gocSound)
                return;

            game::GOCSound::Play(gocSound, deviceTag, "obj_yossy_1up", 0);
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
            if (!gocMovement)
                return;

            int* controller = game::GOCMovement::GetMoveController(gocMovement, 4);
            if (!controller || controller[19] < 0.5f)
                return;
            
            xgame::MsgTakeObject takeMessage{ 16 };
            ObjUtil::SendMessageImmToPlayer(this, PlayerNo, &takeMessage);
            if (takeMessage.field_20)
                ObjUtil::AddScorePlayerAction(this, "GET_ONE_UP", PlayerNo);

            int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
            if (!gocEffect)
                return;

            game::GOCEffect::CreateEffect(gocEffect, "ef_ob_com_yh1_1up");
            GameObject::Kill(this);
        }

        static ObjYoshiOneUp* Create(GameDocument& gameDocument, CInfo& info)
        {
            ObjYoshiOneUp* result = new ObjYoshiOneUp(info);
            if (result)
                GameDocument::AddGameObject(&gameDocument, result);

            return result;
        }
    };
}