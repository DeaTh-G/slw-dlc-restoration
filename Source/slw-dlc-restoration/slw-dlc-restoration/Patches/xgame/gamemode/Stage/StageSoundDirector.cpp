#include "pch.h"
#include "StageSoundDirector.h"

HOOK(void, __fastcall, StopBGMHook, ASLR(0x0095AFB0), app::xgame::CStageSoundDirector* in_pThis, void* edx, float in_time)
{
	// These checks are originally performed in app::GameModeStage::StatePlay, however as to avoid the resorting to a mid-asm hook
	// they have been moved to the earliest point of code execution inside app::xgame::CStageSoundDirector::StopBGM.
	if (!in_pThis->pStageData->IsYoshiIslandStage() && !in_pThis->pStageData->IsZeldaStage())
		originalStopBGMHook(in_pThis, edx, in_time);
}

HOOK(void, __fastcall, PlayResultBGMHook, ASLR(0x0095CB50), app::xgame::CStageSoundDirector* in_pThis, void* edx)
{
	// These checks are originally performed in app::GameModeStage::StatePlay, however as to avoid the resorting to a mid-asm hook
	// they have been moved to the earliest point of code execution inside app::xgame::CStageSoundDirector::PlayResultBGM.
	if (!in_pThis->pStageData->IsYoshiIslandStage() && !in_pThis->pStageData->IsZeldaStage())
		originalPlayResultBGMHook(in_pThis, edx);
}

HOOK(void, __fastcall, LoadDataHook, ASLR(0x0095B240), app::xgame::CStageSoundDirector* in_pThis, void* edx)
{
	originalLoadDataHook(in_pThis, edx);

	// Ensure that the cue sheet for the Legend of Zelda Zone DLC is loaded in every level by forcefully loading it.
	// This change is made so that the Heart System's alert sound can be played in any level.
	csl::fnd::Singleton<app::fnd::FileLoader>::GetInstance()->LoadFile("sound/se_object_zdlc03.acb", {});
}

void slw_dlc_restoration::xgame::CStageSoundDirector::InstallHooks()
{
	// General Hooks
	INSTALL_HOOK(StopBGMHook);
	INSTALL_HOOK(PlayResultBGMHook);

	// The Legend of Zelda Zone DLC Hooks
	if (CONFIGURATION.ZeldaTweaks.IsAlwaysHeartLife)
		INSTALL_HOOK(LoadDataHook);
}
