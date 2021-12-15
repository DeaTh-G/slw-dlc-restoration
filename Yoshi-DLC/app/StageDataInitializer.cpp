#include "pch.h"

bool IsZeldaStageInitialized()
{
    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0)
        return true;

    return false;
}

HOOK(int, __fastcall, StageDataInitializerEndLoadLevelDataHook, ASLR(0x00933520), int* This, void* edx)
{
    if (IsZeldaStageInitialized)
        app::xgame::StagePathUtil::CreatePathObjects((app::GameDocument*)(This[4]), "zdlc03_misc.pac/zdlc03batl_path");

    int result = originalStageDataInitializerEndLoadLevelDataHook(This, edx);
    return result;
}

void app::StageDataInitializer::EndLoadLevelData()
{
    INSTALL_HOOK(StageDataInitializerEndLoadLevelDataHook);
}