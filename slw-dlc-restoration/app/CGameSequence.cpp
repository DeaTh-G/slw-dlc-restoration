#include "pch.h"

static void* RETURN_ADDRESS = (void*)ASLR(0x0090F31A);

__declspec(naked) void CGameSequenceUnlockStageOnCompleteStageMidAsmHook()
{
    __asm
    {
        push ebx
        push edi
        mov ecx, esi
        call app::CGameSequence::UnlockMinigames
        jmp [RETURN_ADDRESS]
    }
}

HOOK(int, __fastcall, CGameSequenceCompleteDLCStageHook, ASLR(0x009122C0), app::CGameSequence* This, void* edx, int a2, int a3, int a4)
{
    const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*)*app::Document);
    if (strncmp(packFileName, "zdlc02", 6) == 0)
    {
        This->PushMovieEventQueue(327);
        return This->SeqGotoPlayMovie();
    }

    return originalCGameSequenceCompleteDLCStageHook(This, edx, a2, a3, a4);
}

void app::CGameSequence::CompleteDLCStage()
{
	INSTALL_HOOK(CGameSequenceCompleteDLCStageHook);
}

void app::CGameSequence::UnlockStageOnCompleteStage()
{
    WRITE_JUMP(ASLR(0x0090F306), &CGameSequenceUnlockStageOnCompleteStageMidAsmHook);
}