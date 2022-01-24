#include "pch.h"

HOOK(bool, __cdecl, IsLinkCostumeHook, ASLR(0x008EFFB0), int* a1, void* edx)
{
    if (LinkSonicPlayType == PlayType::NEVER)
        if (!strncmp((const char*)*(*(((int***)*app::Document) + 3) + 5), "GameModeStageBattle", 19) && IsVirtualLinkSonic)
            return true;
        else
            return false;

    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);
    if (!strncmp(packFileName, "zdlc03", 6) || LinkSonicPlayType == PlayType::ALWAYS)
        if (!strncmp((const char*)*(*(((int***)*app::Document) + 3) + 5), "GameModeStageBattle", 19) && !IsVirtualLinkSonic)
            return false;
        else
            return true;

    return false;
}

void app::Player::CVisualBase::IsLinkCostume()
{
    INSTALL_HOOK(IsLinkCostumeHook);
}
