#include "pch.h"

HOOK(bool, __cdecl, IsLinkCostumeHook, ASLR(0x008EFFB0), int* a1, void* edx)
{
    if (LinkSonicPlayType == PlayType::NEVER)
        return false;

    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0 || LinkSonicPlayType == PlayType::ALWAYS)
        return true;

    return false;
}

void app::Player::CVisualBase::IsLinkCostume()
{
    INSTALL_HOOK(IsLinkCostumeHook);
}
