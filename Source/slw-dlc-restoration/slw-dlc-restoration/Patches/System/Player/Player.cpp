#include "pch.h"
#include "Player.h"

HOOK(void, __cdecl, CreatePlayerHook, ASLR(0x0085A8F0), app::GameDocument* in_pDocument, app::Player::SCinfo& in_rCreateInfo)
{
	// These flags are never set up by the game as they were introduced in the DLCs. Originally this is not the location where
	// they are set up, however to avoid mid-asm hooking or outright replacing the original app::GameModeStage::SetupPlayer
	// the code for doing so is executed prior to executing the original app::Player::CreatePlayer function.
	auto* pGameMode = in_pDocument->GetGameMode();
	if (!strcmp(pGameMode->GetName(), "GameModeStage"))
	{
		auto* pStageData = static_cast<app::GameModeStage*>(pGameMode)->pStageData;

		in_rCreateInfo.Flags.set(12, pStageData->IsZeldaStage());
		in_rCreateInfo.Flags.set(13, pStageData->IsYoshiIslandStage());

		if (!pStageData->IsYoshiIslandStage() && !pStageData->IsZeldaStage())
			in_rCreateInfo.Flags.set(14, false);
		else
			in_rCreateInfo.Flags.set(14, true);

		// Force the flag used by the heart system on and off depending on the configuration set in the mod's configs.
		// This will make the heart system plugin initialize in levels other than The Legend of Zelda Zone DLC.
		if (CONFIGURATION.ZeldaTweaks.LinkSonicPlayType == slw_dlc_restoration::SConfiguration::SZeldaTweaks::EPlayType::ePlayType_Always)
			in_rCreateInfo.Flags.set(12, true);
		else if (CONFIGURATION.ZeldaTweaks.LinkSonicPlayType == slw_dlc_restoration::SConfiguration::SZeldaTweaks::EPlayType::ePlayType_Never)
			in_rCreateInfo.Flags.set(12, false);
	}

	if (CONFIGURATION.ZeldaTweaks.IsVirtualLinkSonic && !strcmp(pGameMode->GetName(), "GameModeStageBattle"))
	{
		auto* pStageData = static_cast<app::GameModeStageBattle*>(pGameMode)->pStageData;

		in_rCreateInfo.Flags.set(12, pStageData->IsZeldaStage());
		in_rCreateInfo.Flags.set(13, pStageData->IsYoshiIslandStage());

		if (!pStageData->IsYoshiIslandStage() && !pStageData->IsZeldaStage())
			in_rCreateInfo.Flags.set(14, false);
		else
			in_rCreateInfo.Flags.set(14, true);
	}

	originalCreatePlayerHook(in_pDocument, in_rCreateInfo);
}

HOOK(void, __fastcall, UpdatePlayerInformationHook, ASLR(0x00851F20), app::Player::CPlayer* in_pThis, void* edx)
{
    originalUpdatePlayerInformationHook(in_pThis, edx);

	size_t playerNo = in_pThis->GetPlayerNo();
	auto* pPlayerInfo = in_pThis->GetService<app::xgame::Blackboard>()->GetPlayerInformation(playerNo);
	if (!pPlayerInfo)
		return;

	// IsLandOnMovableGround was introduced in game's Yoshi's Island Zone DLC, which in turn likely meant that from it was IF_DEF'd for the update.
	// Restoring this value into the PlayerInformation is necessary for it the eggs to work properly when standing on the EggBlock, RotateLift or other moving platforms.
	auto* pStateGoc = in_pThis->GetPlayerGOC<app::Player::CStateGOC>();
	if (pStateGoc->GetCurrentState() != 105 || !app::Player::StateUtil::GetSnowBall(*pStateGoc))
		pPlayerInfo->IsLandOnMovableGround = pPlayerInfo->IsOnGround ? app::Player::StateUtil::IsLandOnMovableGround(*pStateGoc) : false;
}

void slw_dlc_restoration::Player::InstallHooks()
{
	// General Hooks
	INSTALL_HOOK(CreatePlayerHook);

	// The Legend of Zelda Zone DLC Hooks
	ResourceInfo::InstallHooks();
}

void slw_dlc_restoration::Player::CPlayer::InstallHooks()
{
	// Yoshi's Island Zone DLC Hooks
    INSTALL_HOOK(UpdatePlayerInformationHook);
}