#include "pch.h"
#include "GameSequence.h"

HOOK(void, __fastcall, CompleteDLCStageHook, ASLR(0x009122C0), app::CGameSequence* in_pThis, void* edx, app::xgame::DlcStageIndex in_index, uint in_unk1, uint in_unk2)
{
    if (CONFIGURATION.GlobalTweaks.IsSHC2021Build && !strcmp(in_pThis->pGame->GetGameMode()->GetName(), "GameModeStage"))
    {
        auto* pGameMode = static_cast<app::GameModeStage*>(in_pThis->pGame->GetGameMode());
        if (!strcmp(pGameMode->pLevelInfo->Level, "zdlc02"))
        {
            in_pThis->PushMovieEventQueue(327);
            in_pThis->SeqGotoPlayMovie();

            return;
        }
    }

    originalCompleteDLCStageHook(in_pThis, edx, in_index, in_unk1, in_unk2);
}

void slw_dlc_restoration::CGameSequence::InstallPatches()
{
    // General Patches
    // Replace junk data in the rewards DLC array with the proper values.
    WRITE_MEMORY(ASLR(0x00E05654), uint, 22);
    WRITE_MEMORY(ASLR(0x00E05658), uint, 23);

    // Run the for loop for the actual length of the array instead of the trimmed PC version.
    WRITE_MEMORY(ASLR(0x0090D3FD), byte, 5);
}

void slw_dlc_restoration::CGameSequence::InstallHooks()
{
    // General Hooks
    INSTALL_HOOK(CompleteDLCStageHook);
}