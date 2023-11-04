#include "pch.h"
#include "PlayerVisualVirtualSonicInfo.h"

static const char* LINKSONIC_PACKFILE_NAME = (const char*)ASLR(0x00E02C80);
static const char* LINKSONIC_ASSET_NAME = (const char*)ASLR(0x00E02C90);

HOOK(void, __fastcall, LoadHook, ASLR(0x008F8BA0), app::Player::VirtualSonicInfo* in_pThis, void* edx, app::CObjInfoFileLoader& in_rLoader)
{
	originalLoadHook(in_pThis, edx, in_rLoader);
	
	// Linksonic's packfile never gets loaded in multiplayer so it needs to be manually loaded.
	// The archive name is referenced from the game to ensure mod compatibility.
	// In addition another packfile is loaded for the added alt skin added by the mod.
	in_rLoader.Load(LINKSONIC_PACKFILE_NAME, 0x4001);
}

HOOK(void, __fastcall, InitializeHook, ASLR(0x008F8C00), app::Player::VirtualSonicInfo* in_pThis, void* edx, app::GameDocument& in_rDocument)
{
	originalInitializeHook(in_pThis, edx, in_rDocument);

	// The models of the two Virtual Sonics for multiplayer get replaced by Link Sonic, and an alt costume for Link Sonic.
	// The packfile and the asset names for Player 1 are referenced from the game's memory to ensure compatibility with mods.
	auto packFile = app::ObjUtil::GetPackFile(LINKSONIC_PACKFILE_NAME);
	in_pThis->Models[0] = app::ObjUtil::GetModelResource(LINKSONIC_ASSET_NAME, packFile);
	in_pThis->Models[1] = app::ObjUtil::GetModelResource("chr_Linksonic2p", packFile);
	for (size_t i = 0; i < in_pThis->Models.size(); i++)
	{
		if (in_pThis->Models[i]->MeshGroupCount > 1 && in_pThis->Models[i].GetResMeshGroup(1).GetNumResMeshes() > 1 && !strcmp(in_pThis->Models[i].GetResMeshGroup(1).GetResMesh(1)->MaterialName.pValue, "chr_linksonic_leather2"))
		{
			auto mesh = in_pThis->Models[i].GetResMeshGroup(1).GetResMesh(1);
			auto otherMesh = in_pThis->Models[i].GetResMeshGroup(1).GetResMesh(0);

			mesh->pMaterial = otherMesh->pMaterial;
			mesh->MaterialName = otherMesh->MaterialName;
		}

		if (in_pThis->Models[i]->MeshGroupCount > 2 && in_pThis->Models[i].GetResMeshGroup(2).GetNumResMeshes() > 1 && !strcmp(in_pThis->Models[i].GetResMeshGroup(2).GetResMesh(1)->MaterialName.pValue, "chr_linksonic_leather2"))
		{
			auto mesh = in_pThis->Models[i].GetResMeshGroup(2).GetResMesh(1);
			auto otherMesh = in_pThis->Models[i].GetResMeshGroup(2).GetResMesh(0);

			mesh->pMaterial = otherMesh->pMaterial;
			mesh->MaterialName = otherMesh->MaterialName;
		}
	}

	// The antenna models gets stubbed out for the multiplayer Link Sonics as their looks do not fit
	// the looks that the option is going for.
	in_pThis->AntennaModels[0] = { nullptr };
	in_pThis->AntennaModels[1] = { nullptr };
}

void slw_dlc_restoration::Player::VirtualSonicInfo::InstallPatches(ModInfo_t* in_pModInfo)
{
	// The Legend of Zelda Zone DLC Patches
	if (CONFIGURATION.ZeldaTweaks.IsVirtualLinkSonic)
	{
		in_pModInfo->API->BindFile(in_pModInfo->CurrentMod, "+Linksonic.pac", "dlc/0012/sonic2013_dlc_0012/Linksonic2p.pac", 1);
	}
}

void slw_dlc_restoration::Player::VirtualSonicInfo::InstallHooks()
{
	// The Legend of Zelda Zone DLC Hooks
	if (CONFIGURATION.ZeldaTweaks.IsVirtualLinkSonic)
	{
		INSTALL_HOOK(LoadHook);
		INSTALL_HOOK(InitializeHook);
	}
}