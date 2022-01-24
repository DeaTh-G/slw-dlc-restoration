#include "pch.h"

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
