#include "pch.h"
#include "PlayerVisualHuman.h"

HOOK(void, __fastcall, InitializeShadowHook, ASLR(0x008FDAF0), app::Player::CVisualHuman* in_pThis, void* edx, app::res::ResShadowModelData* in_pShadowModelData, hh::mr::CVertexDeclarationPtr* in_pVertexDeclaration, hh::mr::CRenderingInfrastructure* in_pRenderingInfrastructure)
{
	// Whenever this hook executes the player(s)'s shadow model asset is swapped out for that of Link Sonic.
	if (auto* pLinksonicInfo = app::ObjUtil::GetObjectInfo<app::Player::SonicZeldaInfo>(*in_pThis->pPlayer->GetDocument()))
	{
		in_pShadowModelData = &pLinksonicInfo->ShadowModel.ref();
		in_pVertexDeclaration = &pLinksonicInfo->ShadowModel.m_VertexDecl;
	}

	originalInitializeShadowHook(in_pThis, edx, in_pShadowModelData, in_pVertexDeclaration, in_pRenderingInfrastructure);
}

void slw_dlc_restoration::Player::CVisualHuman::InstallHooks()
{
	// The Legend of Zelda Zone DLC Hooks
	if (CONFIGURATION.ZeldaTweaks.IsVirtualLinkSonic)
		INSTALL_HOOK(InitializeShadowHook);
}
