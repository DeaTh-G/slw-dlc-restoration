#include "pch.h"
#include "ObjDroppedRingCreate.h"

HOOK(void, __cdecl, DroppedRingCreateHook, ASLR(0x00662DF0), app::GameDocument& in_rDocument, const app::SDroppedRingInfo& in_rInfo)
{
	// The update that was released alongside the Yoshi's Island Zone DLC modified the original app::dropped_ring::Create function.
	// To not cause incompatibility issues with mods in the future, the update code will be only be used in that level.
	if (strcmp(in_rDocument.GetService<app::CLevelInfo>()->Level, "zdlc02") == 0)
		app::dropped_ring::Create(in_rDocument, in_rInfo);
	else
		originalDroppedRingCreateHook(in_rDocument, in_rInfo);
}

void slw_dlc_restoration::dropped_ring::InstallHooks()
{
	// Yoshi's Island Zone DLC Hooks
	INSTALL_HOOK(DroppedRingCreateHook);
}