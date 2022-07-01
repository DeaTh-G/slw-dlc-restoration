#pragma once

namespace app
{
    class HudWipeCrayPipe : public GameObject
    {
    public:
        game::HudLayerController* LayerController;

        HudWipeCrayPipe()
        {
            SetObjectCategory(this, 11);
        }

        void AddCallback(GameDocument* gameDocument) override
        {
            fnd::GOComponent::Create(this, GOCHud);
            fnd::GOComponent::BeginSetup(this);

            game::GOCHud* gocHud = (game::GOCHud*)GameObject::GetGOC(this, GOCHudString);
            if (gocHud)
            {
                int packFile{};
                int refCount{};

                game::GOCHud::Description hudDescription{};
                hudDescription.field_00 = "HudMissionBanner";
                hudDescription.field_04 = 8;
                hudDescription.field_08 = 9;
                hudDescription.field_0C = -21;
                hudDescription.field_10 = 2;
                hudDescription.field_14 = 2048;
                gocHud->Setup(&hudDescription);
                
                if (IsAlwaysHeartLife)
                    ObjUtil::GetPackFile(&packFile, "ui_zdlc02_gamemodezelda.pac");
                else
                    ObjUtil::GetPackFile(&packFile, "ui_zdlc02_gamemodestage.pac");
                gocHud->SetupProject(&refCount, "ui_wipe_zdlc02", packFile);
                if (refCount)
                {
                    LayerController = gocHud->CreateLayerController(refCount, "ui_wipe_zdlc02", "wipe_mask", 0);
                    LayerController->ReserveAnimation("Outro_Anim", 0, 0);
                    LayerController->PlayReservedAnimation();
                }
            }

            fnd::GOComponent::EndSetup(this);
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            if (!LayerController)
                return;

            if (!LayerController->IsEndReservedAnimation())
                return;

            Kill();
        }
    };
}