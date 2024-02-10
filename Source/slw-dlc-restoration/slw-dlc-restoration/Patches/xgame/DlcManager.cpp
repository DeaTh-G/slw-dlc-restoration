#include "pch.h"
#include "DlcManager.h"

HOOK(void, __fastcall, SetupHook, ASLR(0x0095E280), app::xgame::DlcManager* in_pThis, void* edx, app::xgame::DlcManager::SetupInfo& in_rSetupInfo)
{
	// Register the DLCs missing from initialization on the PC version of the game to ensure their functionality.
	// The first two DLCs are missing due to the fact that those are not available DLCs on the PC version of the game,
	// whilst the latter two likely got deleted due to the RC vehicle/Multiplayer system not making its way to the port of the game.
	in_rSetupInfo.SetContent(17, 1); // Yoshi's Island Zone DLC
	in_rSetupInfo.SetContent(18, 1); // The Legend of Zelda Zone DLC
	in_rSetupInfo.SetContent(22, 2); // Golden Omochao RC Vehicle DLC
	in_rSetupInfo.SetContent(23, 2); // Sonic Omochao RC Vehicle DLC

	originalSetupHook(in_pThis, edx, in_rSetupInfo);
}

void slw_dlc_restoration::xgame::DlcManager::InstallHooks()
{
	// General Hooks
	INSTALL_HOOK(SetupHook);
}
