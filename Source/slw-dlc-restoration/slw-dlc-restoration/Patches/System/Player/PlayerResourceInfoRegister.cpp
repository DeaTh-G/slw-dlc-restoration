#include "pch.h"
#include "PlayerResourceInfoRegister.h"

HOOK(void, __cdecl, RegisterResourceInfosHook, ASLR(0x008F73F0), app::GameDocument* in_pDocument, const app::Player::ResourceInfo& in_rResourceInfo, csl::fnd::IAllocator& in_rAllocator)
{
	app::Player::ResourceInfo resourceInfo{ in_rResourceInfo };
	
	// This flag is never set up by the game as it was introduced in The Legend of Zelda Zone DLC.
	// Originally this is not the location where they are set up, however to avoid mid-asm hooking
	// or outright replacing the original app::GameModeStage::RegisterObjInfos the code for doing so
	// is executed prior to executing the original app::Player::RegisterResourceInfos function.
	auto* pGameMode = in_pDocument->GetGameMode();
	if (!strcmp(pGameMode->GetName(), "GameModeStage"))
	{
		auto* pStageData = static_cast<app::GameModeStage*>(pGameMode)->pStageData;
		resourceInfo.Flags.set(4, pStageData->IsZeldaStage());

		// Force the flag used by the Link Sonic on and off depending on the configuration set in the mod's configs.
		// This will make Link Sonic show up in levels other than The Legend of Zelda Zone DLC.
		if (CONFIGURATION.ZeldaTweaks.LinkSonicPlayType == slw_dlc_restoration::SConfiguration::SZeldaTweaks::EPlayType::ePlayType_Always)
			resourceInfo.Flags.set(4, true);
		else if (CONFIGURATION.ZeldaTweaks.LinkSonicPlayType == slw_dlc_restoration::SConfiguration::SZeldaTweaks::EPlayType::ePlayType_Never)
			resourceInfo.Flags.set(4, false);
	}

	if (CONFIGURATION.ZeldaTweaks.IsVirtualLinkSonic && !strcmp(pGameMode->GetName(), "GameModeStageBattle"))
		resourceInfo.Flags.set(4, true);

	originalRegisterResourceInfosHook(in_pDocument, resourceInfo, in_rAllocator);
}

void slw_dlc_restoration::Player::ResourceInfo::InstallHooks()
{
	INSTALL_HOOK(RegisterResourceInfosHook);
}
