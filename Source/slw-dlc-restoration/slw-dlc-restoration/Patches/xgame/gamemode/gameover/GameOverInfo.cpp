#include "pch.h"
#include "GameOverInfo.h"

static const char* LINKSONIC_PACKFILE_NAME = (const char*)ASLR(0x00E02C80);
static const char* LINKSONIC_ASSET_NAME = (const char*)ASLR(0x00E02C90);

HOOK(void, __fastcall, LoadHook, ASLR(0x00928CA0), app::GameOverInfo* in_pThis, void* edx, app::CObjInfoFileLoader& in_rLoader)
{
	originalLoadHook(in_pThis, edx, in_rLoader);

	// Linksonic's packfile never gets loaded in multiplayer so it needs to be manually loaded.
	// The archive name is referenced from the game to ensure mod compatibility.
	in_rLoader.Load(LINKSONIC_PACKFILE_NAME, 0x4001);
}

HOOK(void, __fastcall, InitializeHook, ASLR(0x00928CE0), app::GameOverInfo* in_pThis, void* edx, app::GameDocument& in_rDocument)
{
	originalInitializeHook(in_pThis, edx, in_rDocument);

	// The following code ensures that the model of the Sonic for the game over screen get replaced by Link Sonic.
	// The packfile and the asset names for the player are referenced from the game's memory to ensure compatibility with mods.
	auto packFile = app::ObjUtil::GetPackFile(LINKSONIC_PACKFILE_NAME);
	in_pThis->SonicModel = app::ObjUtil::GetModelResource(LINKSONIC_ASSET_NAME, packFile);
	if (CONFIGURATION.ZeldaTweaks.IsLinkSonicFixed)
	{
		if (auto model = app::ObjUtil::GetModelResource("chr_Linksonic_fixed", packFile))
			in_pThis->SonicModel = model;
	}
	in_pThis->SonicSkeleton = app::ObjUtil::GetSkeletonResource(LINKSONIC_ASSET_NAME, packFile);
}

void slw_dlc_restoration::GameOverInfo::InstallHooks()
{
	// The Legend of Zelda Zone DLC Hooks
	if (CONFIGURATION.ZeldaTweaks.LinkSonicPlayType == slw_dlc_restoration::SConfiguration::SZeldaTweaks::EPlayType::ePlayType_Always)
	{
		INSTALL_HOOK(LoadHook);
		INSTALL_HOOK(InitializeHook);
	}
}