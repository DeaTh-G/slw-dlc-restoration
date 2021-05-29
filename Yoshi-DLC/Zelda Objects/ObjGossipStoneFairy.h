#pragma once

namespace app
{
    class ObjGossipStoneFairy : public GameObject3D
    {
    public:
        class CInfo
        {
        public:
        };

        INSERT_PADDING(96);

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

        }
    };
}