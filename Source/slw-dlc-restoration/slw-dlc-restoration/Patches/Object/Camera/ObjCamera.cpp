#include "pch.h"
#include "ObjCamera.h"

HOOK(bool, __fastcall, ProcMsgNotifyObjectEventHook, ASLR(0x005F81D0), app::CSetObjectListener* in_pThis, void* edx, app::xgame::MsgNotifyObjectEvent& in_rMessage)
{
	// Bandaid fix to the game hijacking Player 1's camera for every camera object started by a Switch/Switch Collision.
	// Ideally the message should be able to track which player the event is coming from, but without a properly working multiplayer
	// mode, this is outside of the scope of this version of DLC Restoration.
	auto* pLevelInfo = in_pThis->GetDocument()->GetService<app::CLevelInfo>();

	if (!strcmp(in_pThis->GetDocument()->pGameMode->GetName(), "GameModeStageBattle") && !strcmp(pLevelInfo->Level, "zdlc03"))
		return false;

	return originalProcMsgNotifyObjectEventHook(in_pThis, edx, in_rMessage);
}

void slw_dlc_restoration::CObjCamera::InstallHooks()
{
	// The Legend of Zelda Zone DLC Hooks
	if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
		INSTALL_HOOK(ProcMsgNotifyObjectEventHook);
}
