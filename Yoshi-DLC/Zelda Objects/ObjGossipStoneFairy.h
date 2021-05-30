#pragma once

namespace app
{
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

        INSERT_PADDING(96);

        ObjGossipStoneFairy(CInfo& info)
        {

        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::BeginSetup(this);

            fnd::GOComponent::EndSetup(this);
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {

        }

        static ObjGossipStoneFairy* Create(GameDocument& gameDocument, CInfo& info)
        {
            ObjGossipStoneFairy* object = new ObjGossipStoneFairy(info);
            if (!object)
                return 0;
            //GameDocument::AddGameObject(*(GameDocument**)&gameDocument, object);
            return object;
        }
    };
}