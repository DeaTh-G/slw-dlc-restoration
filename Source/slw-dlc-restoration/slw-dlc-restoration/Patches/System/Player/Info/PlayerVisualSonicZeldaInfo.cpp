#include "pch.h"
#include "PlayerVisualSonicZeldaInfo.h"

static const char* LINKSONIC_PACKFILE_NAME = (const char*)ASLR(0x00E02C80);

HOOK(void, __fastcall, InitializeHook, ASLR(0x008F8800), app::Player::SonicZeldaInfo* in_pThis, void* edx, app::GameDocument& in_rDocument)
{
	originalInitializeHook(in_pThis, edx, in_rDocument);

	// Replace Link Sonic's model with a fixed asset created by M&M because the materials are assigned incorrectly on the original.
	// This replacement only occurs if the fixed variant exists in Linksonic.pac. This means mods overwriting that packfile
	// don't need to include a replacement model twice.
	auto packFile = app::ObjUtil::GetPackFile(LINKSONIC_PACKFILE_NAME);
	if (auto model = app::ObjUtil::GetModelResource("chr_Linksonic_fixed", packFile))
		in_pThis->Model = model;
}

void slw_dlc_restoration::Player::SonicZeldaInfo::InstallHooks()
{
	// The Legend of Zelda Zone DLC Hooks
	if (CONFIGURATION.ZeldaTweaks.IsLinkSonicFixed)
		INSTALL_HOOK(InitializeHook);
}
