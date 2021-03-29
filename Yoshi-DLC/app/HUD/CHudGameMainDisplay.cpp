#include "pch.h"

HOOK(int*, __fastcall, SpecialRingUpdateHook, ASLR(0x005027A0), int* This, void* edx, float a2, char a3)
{
    int* result = 0;
    const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*) * app::Document);
    if (strncmp(packFileName, "zdlc02", 6) == 0)
        app::HUD::SpecialFlowerUpdate(This, a2, a3);
    else
        result = originalSpecialRingUpdateHook(This, edx, a2, a3);
    return result;
}

HOOK(int*, __fastcall, CHudGameMainDisplayHook, ASLR(0x00503290), int* This, int* edx, char a2, int a3, int a4, char a5, int a6, int a7)
{
    originalCHudGameMainDisplayHook(This, edx, a2, a3, a4, a5, a6, a7);

    const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*) * app::Document);
    if (strncmp(packFileName, "zdlc02", 6) == 0)
    {
        *(This + 0x7A) |= 0x40;
        *(This + 0x7A) |= 0x20;
    }

    return This;
}

void app::HUD::CHudGameMainDisplay::__ct()
{
    INSTALL_HOOK(CHudGameMainDisplayHook);
}

void app::HUD::CHudGameMainDisplay::SpecialRingUpdate()
{
    INSTALL_HOOK(SpecialRingUpdateHook);
}
