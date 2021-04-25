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
        // Disable DOF in Yoshi DLC
        WRITE_MEMORY(ASLR(0x00FEFC7C), 0);
        WRITE_MEMORY(ASLR(0x00404C3E), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
        WRITE_MEMORY(ASLR(0x0040498C), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
        WRITE_MEMORY(ASLR(0x00404C3E), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);

        *(This + 0x7A) |= 0x40;
        *(This + 0x7A) |= 0x20;
    }
    else
    {
        // Restore DOF Code if not Yoshi DLC
        WRITE_MEMORY(ASLR(0x00FEFC7C), ((char*)ASLR(0x00F6072C))[9]);
        WRITE_MEMORY(ASLR(0x00404C3E), 0x88, 0x15, 0x7C, 0xFC, 0xFE, 0x00);
        WRITE_MEMORY(ASLR(0x0040498C), 0x88, 0x0D, 0x7C, 0xFC, 0xFE, 0x00);
        WRITE_MEMORY(ASLR(0x00404C3E), 0x88, 0x15, 0x7C, 0xFC, 0xFE, 0x00);
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
