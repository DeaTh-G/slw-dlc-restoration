#include "pch.h"
#include "HudGameMainDisplay.h"

void slw_dlc_restoration::HUD::CHudGameMainDisplay::HeartLifeUpdate(app::HUD::CHudGameMainDisplay::Sinfo& in_rInfo, float in_deltaTime, bool in_isEnable)
{
	// An extra check is added here for the configurable option so that the HUD can update outside of The Legend of Zelda Zone DLC.
	// The rest of the function is the same as the game's original implementation of it found in LWAPI.
	if (!Flags.test(8) && !CONFIGURATION.ZeldaTweaks.IsAlwaysHeartLife)
		return;

	if (Unk9.Test(0))
	{
		TimeToHide -= in_deltaTime;
		if (TimeToHide <= 0.0f && !pLayerController->IsCurrentAnimation("Hide_Anim"))
		{
			app::game::HudLayerController::PlayInfo animationInfo{};
			animationInfo.pAnimationName = "Hide_Anim";

			pLayerController->PlayAnimation(animationInfo);
		}
	}

	int numHearts = csl::math::Min(ms_HeartLifeCap, in_rInfo.NumHearts);
	int maxNumHearts = csl::math::Min(ms_HeartLifeCap, in_rInfo.MaxNumHearts);
	if (DoRecoverHeart)
	{
		HeartLifeAnimationTime += in_deltaTime;
		if (HeartLifeAnimationTime >= 0.3f)
		{
			HeartLifeAnimationTime -= 0.3f;

			if (numHearts > NumHearts)
				numHearts = NumHearts + 1;
			else
				DoRecoverHeart = false;
		}
		else
		{
			numHearts = NumHearts;
		}
	}

	if (MaxNumHearts != maxNumHearts)
	{
		app::game::HudLayerController::PlayInfo animationInfo{};

		if (maxNumHearts >= 4 && maxNumHearts <= 6)
			animationInfo.pAnimationName = ms_pHeartAnimationNames[maxNumHearts & 3];
		else
			animationInfo.pAnimationName = "mode_heart0";

		pLayerController->PlayAnimation(animationInfo);
		MaxNumHearts = maxNumHearts;
	}

	if (NumHearts != numHearts)
	{
		if (DoRecoverHeart && !ScreenType && NumHearts < numHearts && NumHearts > 0)
			app::ObjUtil::PlaySE2D("obj_zeldaheart_get", { HH_SOUND_DEVICE_AUTO });

		NumHearts = numHearts;

		app::game::HudLayerController::PlayInfo heartbeatAnimationInfo{};
		heartbeatAnimationInfo.pAnimationName = "life_volume";
		heartbeatAnimationInfo.Frame = numHearts * 20.0f;
		pLayerController->PlayAnimation(heartbeatAnimationInfo);

		app::game::HudLayerController::PlayInfo animationInfo{};
		animationInfo.pAnimationName = "Usual_Anim";
		animationInfo.PlayPolicy = app::game::HudPlayPolicy::eHudPlayPolicy_Loop;
		pLayerController->PlayAnimation(animationInfo);
	}

	pLayerController->SetVisible(true);
}

HOOK(app::HUD::CHudGameMainDisplay*, __fastcall, ConstructorHook, ASLR(0x00503290), app::HUD::CHudGameMainDisplay* in_pThis, void* edx, bool in_unk1, bool in_unk2, bool in_unk3, bool in_unk4, char const* in_pStageName, app::HUD::CHudGameMainDisplay::eScreenType in_screenType)
{
	originalConstructorHook(in_pThis, edx, in_unk1, in_unk2, in_unk3, in_unk4, in_pStageName, in_screenType);

	// The flags for the hud are never set for the two missing DLCs from the game's pc port.
	// The following code does that, just like on the Wii U.
	if (auto* pStageData = app::StageInfo::CStageInfo::GetInstance()->GetStageData(in_pThis->StageName.c_str()))
	{
		if (pStageData->IsYoshiIslandStage())
		{
			in_pThis->Flags.set(6);
			in_pThis->Flags.set(5);
		}

		if (pStageData->IsZeldaStage())
		{
			in_pThis->Flags.set(7);
			in_pThis->Flags.set(8);
		}
	}
	
	return in_pThis;
}

HOOK(void, __fastcall, InitLayerHook, ASLR(0x00503780), app::HUD::CHudGameMainDisplay* in_pThis, void* edx)
{
	in_pThis->InitLayer();

	if (CONFIGURATION.ZeldaTweaks.IsAlwaysHeartLife)
	{
		// Create an instance of the ui_gameplay_zdlc03.swif file found inside the ui_zdlc03_gamemodestage.pac so that it can be used to draw the
		// Heart Display from The Legend of Zelda DLC in any level.
		auto rawData = csl::fnd::Singleton<app::fnd::ResourceManager>::GetInstance()->Get<app::fnd::ResRawData>("ui_zdlc03_gamemodestage.pac");
		if (rawData.IsValid())
		{
			hh::ut::PackFile packFile{ rawData->GetAddress() };

			size_t zeldaHudIndex{ static_cast<size_t>(-1) };
			for (size_t i = 0; i < in_pThis->pHudGoc->rcProjects.size(); i++)
			{
				if (!strcmp(in_pThis->pHudGoc->rcProjects[i]->GetName(), "ui_gameplay_zdlc03"))
				{
					zeldaHudIndex = i;
					break;
				}
			}

			SurfRide::ReferenceCount<SurfRide::Project> rcProject{};
			if (zeldaHudIndex != static_cast<size_t>(-1))
				rcProject = in_pThis->pHudGoc->rcProjects[zeldaHudIndex];
			else
				in_pThis->pHudGoc->SetupProject(&rcProject, "ui_gameplay_zdlc03", packFile);

			if (rcProject)
			{
				if (auto rcScene = rcProject->GetScene("ui_gameplay_zdlc03"))
					app::HUD::SRUtility::AllLayerHide(rcScene);
			}
		}

		if (in_pThis->pHudGoc->rcProjects.size() > 1)
		{
			if (in_pThis->rcLayers[1])
			{
				// Offset the animation found inside the info_ring layer to move it down by a slot on the HUD.
				if (in_pThis->IsTimeTrial && !in_pThis->pLayerController)
					in_pThis->rcLayers[1]->pBinaryLayer->pAnimations[1].pLinks[0].pTracks[0].pKeyFrame->Value.Float -= 52.0f;

				// Invoke the secondary position animation for the info_ring to move it down by a slot on the HUD.
				app::HUD::SRUtility::SetAnimation(in_pThis->rcLayers[1], "mode_1_Param", false, -1.0f, false, false);
			}

			if (in_pThis->ScreenType == 1 && !in_pThis->pLayerController)
			{
				// Offset the cast found inside the info_challenge layer to move it down by a slot on the HUD.
				if (in_pThis->rcLayers[0])
				{
					auto* pTransform = static_cast<SurfRide::SRS_TRS3D*>(in_pThis->rcLayers[0]->prcCasts[1]->pBinaryTransform);
					pTransform->Translation.SetY(pTransform->Translation.GetY() - 52.0f);
				}

				// Offset the cast found inside the info_s_ring layer to move it down by a slot on the HUD.
				if (in_pThis->rcLayers[5])
				{
					auto* pTransform = static_cast<SurfRide::SRS_TRS3D*>(in_pThis->rcLayers[5]->prcCasts[1]->pBinaryTransform);
					pTransform->Translation.SetY(pTransform->Translation.GetY() - 52.0f);
				}

				// Offset the cast found inside the info_g_ring layer to move it down by a slot on the HUD.
				if (in_pThis->rcLayers[6])
				{
					auto* pTransform = static_cast<SurfRide::SRS_TRS3D*>(in_pThis->rcLayers[6]->prcCasts[1]->pBinaryTransform);
					pTransform->Translation.SetY(pTransform->Translation.GetY() - 52.0f);
				}
			}

			// Offset the cast found inside the info_ring_zdlc03 layer to move it down by a slot on the HUD.
			if (in_pThis->IsTimeTrial && !in_pThis->pLayerController)
			{
				auto* pTransform = static_cast<SurfRide::SRS_TRS3D*>(in_pThis->pHudGoc->rcProjects[1]->prcScenes[0]->prcLayers[6]->prcCasts[1]->pBinaryTransform);
				pTransform->Translation.SetY(pTransform->Translation.GetY() - 52.0f);
			}

			in_pThis->pLayerController = in_pThis->pHudGoc->CreateLayerController({ in_pThis->pHudGoc->rcProjects[1]->prcScenes[0] }, "info_ring_zdlc03", 20);
			in_pThis->pLayerController->PlayAnimation("Intro_Anim", app::game::HudPlayPolicy::eHudPlayPolicy_Once, false);
			in_pThis->pLayerController->SetVisible(true);
		}
	}
}

HOOK(bool, __fastcall, ProcessMessageHook, ASLR(0x005060D0), app::fnd::CActor* in_pThis, void* edx, app::fnd::Message& in_rMessage)
{
	auto* pThis = static_cast<app::HUD::CHudGameMainDisplay*>(in_pThis);

	switch (in_rMessage.GetType())
	{
	case app::xgame::MsgHudUpdateInfo::MessageID:
	{
		// Upon this message being sent off by GameModeStage the parameters for heart system's functionality aren't filled in.
		// Due it being processed immediately, we can not intercept the message in GameModeStage::SendCockpitInfo so its done here.
		auto* pPlayerInfo = pThis->pLevelInfo->GetPlayerInfo(0); // TODO: Check back on this
		auto& message = static_cast<app::xgame::MsgHudUpdateInfo&>(in_rMessage);
		message.Info.NumHearts = pPlayerInfo->NumHearts;
		message.Info.MaxNumHearts = pPlayerInfo->MaxNumHearts;

		// The process message function of this specific messsage is missing the assignment of the heart system parameters from the message to the HUD.
		// The following code assigns those parameters as they should be, before the original code is executed.
		pThis->Info.NumHearts = message.Info.NumHearts;
		pThis->Info.MaxNumHearts = message.Info.MaxNumHearts;

		return originalProcessMessageHook(in_pThis, edx, in_rMessage);
	}
	// These two messages are never handled here like they should be on the PC port due to The Legend of Zelda Zone DLC missing from the game.
	case app::xgame::MsgDlcZeldaTakeHeartContainer::MessageID:		return pThis->ProcMsgDlcZeldaTakeHeartContainer(static_cast<app::xgame::MsgDlcZeldaTakeHeartContainer&>(in_rMessage));
	case app::xgame::MsgDlcZeldaHeartAllRecovery::MessageID:		return pThis->ProcMsgDlcZeldaHeartAllRecovery(static_cast<app::xgame::MsgDlcZeldaHeartAllRecovery&>(in_rMessage));
	default:														return originalProcessMessageHook(in_pThis, edx, in_rMessage);
	}
}

HOOK(void, __fastcall, SetInfoHook, ASLR(0x00505DF0), slw_dlc_restoration::HUD::CHudGameMainDisplay* in_pThis, void* edx, void* in_pUnk1, float in_deltaTime)
{
    originalSetInfoHook(in_pThis, edx, in_pUnk1, in_deltaTime);
	
	// The update function for the Heart System's HUD elements are never called as it does not exist on the PC version.
	// This function call replicates the same functionality from the Wii U version.
	in_pThis->HeartLifeUpdate(in_pThis->Info, in_deltaTime, false);
}

HOOK(void, __fastcall, SetSpecialRingHook, ASLR(0x005020A0), app::HUD::CHudGameMainDisplay* in_pThis, void* edx)
{ 
	// This check is originally performed in app::HUD::CHudGameMainDisplay::InitLayer on the Wii U version of the game, however
	// as the SetSpecialFlower function doesn't exist on the PC version the check is also gone. To avoid a a mid-asm hook into
	// that function the check is performed as soon as the SetSpecialRing function begins execution instead.
	if (in_pThis->Flags.test(5))
		in_pThis->SetSpecialFlower();
	else
		originalSetSpecialRingHook(in_pThis, edx);
}

HOOK(void, __fastcall, SpecialRingUpdateHook, ASLR(0x005027A0), app::HUD::CHudGameMainDisplay* in_pThis, void* edx, float in_deltaTime, bool in_unk)
{
	// This check is originally performed in app::HUD::CHudGameMainDisplay::SetInfo on the Wii U version of the game, however
	// as the SpecialFlowerUpdate function doesn't exist on the PC version the check is also gone. To avoid a a mid-asm hook into
	// that function the check is performed as soon as the SpecialFlowerRing function begins execution instead.
	if (in_pThis->Flags.test(5))
		in_pThis->SpecialFlowerUpdate(in_deltaTime, in_unk);
	else
		originalSpecialRingUpdateHook(in_pThis, edx, in_deltaTime, in_unk);
}

void slw_dlc_restoration::HUD::CHudGameMainDisplay::InstallHooks()
{
	// General Patches
	INSTALL_HOOK(ConstructorHook);
	INSTALL_HOOK(InitLayerHook);
	INSTALL_HOOK(SetInfoHook);

	// Yoshi's Island Zone DLC Patches
	INSTALL_HOOK(SetSpecialRingHook);
	INSTALL_HOOK(SpecialRingUpdateHook);

	// The Legend of Zelda Zone DLC Patches
	INSTALL_HOOK(ProcessMessageHook);
}