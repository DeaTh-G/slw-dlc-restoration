#include "pch.h"
#include "PlayerVisualSonicZeldaInfo.h"

static const char* LINKSONIC_PACKFILE_NAME = (const char*)ASLR(0x00E02C80);

HOOK(void, __fastcall, InitializeHook, ASLR(0x008F8800), app::Player::SonicZeldaInfo* in_pThis, void* edx, app::GameDocument& in_rDocument)
{
	originalInitializeHook(in_pThis, edx, in_rDocument);

	// Replace the material data used for the second mesh found in the mesh groups 'Sonic_Mouth' and 'Sonic_Mouth_L' if the material name is set to
	// 'chr_linksonic_leather2' to prevent the inside of Linksonic's mouth using the leather material texture instead of proper textures.
	// The original fix for this solution was created by M&M (https://gamebanana.com/members/1347950) by creating a new model that has these values set correctly.
	if (in_pThis->Model->MeshGroupCount > 1 && in_pThis->Model.GetResMeshGroup(1).GetNumResMeshes() > 1 && !strcmp(in_pThis->Model.GetResMeshGroup(1).GetResMesh(1)->MaterialName.pValue, "chr_linksonic_leather2"))
	{
		auto mesh = in_pThis->Model.GetResMeshGroup(1).GetResMesh(1);
		auto otherMesh = in_pThis->Model.GetResMeshGroup(1).GetResMesh(0);
		
		mesh->pMaterial = otherMesh->pMaterial;
		mesh->MaterialName = otherMesh->MaterialName;
	}

	if (in_pThis->Model->MeshGroupCount > 2 && in_pThis->Model.GetResMeshGroup(2).GetNumResMeshes() > 1 && !strcmp(in_pThis->Model.GetResMeshGroup(2).GetResMesh(1)->MaterialName.pValue, "chr_linksonic_leather2"))
	{
		auto mesh = in_pThis->Model.GetResMeshGroup(2).GetResMesh(1);
		auto otherMesh = in_pThis->Model.GetResMeshGroup(2).GetResMesh(0);

		mesh->pMaterial = otherMesh->pMaterial;
		mesh->MaterialName = otherMesh->MaterialName;
	}
}

void slw_dlc_restoration::Player::SonicZeldaInfo::InstallHooks()
{
	// The Legend of Zelda Zone DLC Hooks
	if (CONFIGURATION.ZeldaTweaks.IsLinkSonicFixed)
		INSTALL_HOOK(InitializeHook);
}