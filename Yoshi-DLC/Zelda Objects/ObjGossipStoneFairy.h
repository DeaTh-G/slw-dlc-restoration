#pragma once

namespace app
{
    class ObjGossipStoneInfo : public CObjInfo
    {
    public:
        int BaseModel{};
        int BaseSkeleton{};
        int BaseAnimation{};
        int FairyModel{};
        int FairySkeleton{};
        int FairyAnimation{};

        ObjGossipStoneInfo() {}

        void Initialize(GameDocument& gameDocument) override
        {
            int packFile = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

            ObjUtil::GetModelResource(&BaseModel, "zdlc03_obj_gossipstone", &packFile);
            ObjUtil::GetSkeletonResource(&BaseSkeleton, "zdlc03_obj_gossipstone", packFile);
            ObjUtil::GetAnimationResource(&BaseAnimation, "zdlc03_obj_gossipstone", &packFile);

            ObjUtil::GetModelResource(&FairyModel, "zdlc03_obj_fairy", &packFile);
            ObjUtil::GetSkeletonResource(&FairySkeleton, "zdlc03_obj_fairy", packFile);
            ObjUtil::GetAnimationResource(&FairyAnimation, "zdlc03_obj_fairy", &packFile);
        }

        const char* GetInfoName() override
        {
            return "ObjGossipStoneInfo";
        }
    };

    class ObjGossipStoneFairy : public GameObject3D
    {
    public:
        class CInfo
        {
        public:
            float field_00{};
            INSERT_PADDING(12);
            csl::math::Matrix34 field_10{};

            CInfo(csl::math::Matrix34 a1)
            {
                field_10 = a1;
            }
        };

        float field_320{};
        INSERT_PADDING(12);
        csl::math::Matrix34 field_330{};
        float Time{};
        INSERT_PADDING(12);

        ObjGossipStoneFairy(CInfo& info)
        {
            field_320 = info.field_00;
            field_330 = info.field_10;
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCVisualModel);
            fnd::GOComponent::Create(this, GOCAnimationSimple);
            
            ObjGossipStoneInfo* info = (ObjGossipStoneInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjGossipStoneInfo");

            fnd::GOComponent::BeginSetup(this);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (gocVisual)
            {
                fnd::GOCVisualModel::VisualDescription visualDescriptor;

                fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
                visualDescriptor.Model = info->FairyModel;
                visualDescriptor.Skeleton = info->FairySkeleton;

                fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

                int* gocAnimation = GameObject::GetGOC(this, GOCAnimation);
                if (gocAnimation)
                {
                    int animCount = 1;
                    game::GOCAnimationSimple::Setup(gocAnimation, &animCount);
                    game::GOCAnimationSimple::Add(gocAnimation, "IDLE", info->FairyAnimation, 1);
                    fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
                    game::GOCAnimationSimple::SetAnimation(gocAnimation, "IDLE");
                }
            }

            fnd::GOComponent::EndSetup(this);
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            Time += updateInfo.deltaTime;
            int timeThird = Time / 3;

            if (timeThird >= 3)
            {
                GameObject::Kill();
                return;
            }

            csl::math::Vector3 xUp { 1, 0, 0 };
            csl::math::Vector3 zUp { 0, 0, 1 };
            csl::math::Vector3 scaledZUp{};
            csl::math::Vector3 crossProduct{};
            csl::math::Vector3 scaledCrossProduct{};
            csl::math::Vector3 translation = *(csl::math::Vector3*)&field_330.data[3];
            float multiplier = SonicUSA::System::RadianMaskU(Time * ((3.1415927f * 2) / 3));
            
            float scalar1 = 10;
            float scalar2 = 5 * sinf((((fmodf(Time, 1.6f) / 1.6f) * 3.1415927f) + ((fmodf(Time, 1.6f) / 1.6) * 3.1415927f)));
            float scalar3{};

            if (timeThird)
            {
                if (timeThird == 2)
                {
                    scalar3 = ((1 - (fmodf(Time, 3) / 3)) * 7) + 10;
                    scalar2 *= (1 - (fmodf(Time, 3) / 3));
                    scalar1 *= (1 - (fmodf(Time, 3) / 3));
                }
                else
                {
                    scalar3 = 17;
                }
            }
            else
            {
                scalar3 = ((fmodf(Time, 3) / 3) * 7) + 10;
                scalar2 *= ((fmodf(Time, 3) / 3));
            }

            math::Vector3RotateY(&zUp, multiplier, &zUp);
            math::Vector3RotateY(&xUp, multiplier, &xUp);
            zUp = MultiplyMatrixSRByVector(&field_330, &zUp);
            xUp = MultiplyMatrixSRByVector(&field_330, &xUp);
            math::Vector3CrossProduct(&zUp, &xUp, &crossProduct);
            math::Vector3Scale(&zUp, scalar1, &scaledZUp);
            math::Vector3Add(&translation, &scaledZUp, &translation);
            math::Vector3Scale(&crossProduct, (scalar2 + scalar3), &scaledCrossProduct);
            math::Vector3Add(&translation, &scaledCrossProduct, &translation);

            csl::math::Matrix34 transform{};
            *(csl::math::Vector3*)&transform.data[0] = Vector3(-zUp.X, -zUp.Y, -zUp.Z);
            *(csl::math::Vector3*)&transform.data[1] = crossProduct;
            *(csl::math::Vector3*)&transform.data[2] = xUp;
            csl::math::Quaternion rotation = GetRotationFromMatrix(&transform);

            int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
            if (!gocTransform)
                return;

            fnd::GOCTransform::SetLocalTranslation(gocTransform, &translation);
            fnd::GOCTransform::SetLocalRotation(gocTransform, &rotation);

            int* gocVisual = GameObject::GetGOC(this, GOCVisual);
            if (!gocVisual)
                return;

            float timeScale = csl::math::Clamp(Time / 0.2f, 0, 1);
            timeScale = csl::math::Clamp(sinf(timeScale * 1.5707964f), 0.01f, 1) * 2;
            csl::math::Vector3 scale { timeScale, timeScale, timeScale };
            fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &scale);
        }

        static ObjGossipStoneFairy* Create(GameDocument& gameDocument, CInfo& info)

        {
            ObjGossipStoneFairy* object = new ObjGossipStoneFairy(info);
            if (!object)
                return 0;
            GameDocument::AddGameObject(*(GameDocument**)&gameDocument, object);
            return object;
        }
    };
}