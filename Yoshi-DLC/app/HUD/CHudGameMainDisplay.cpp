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
    if (strncmp(packFileName, "zdlc02", 6) == 0 || strncmp(packFileName, "zdlc03", 6) == 0)
    {
        // Disable DOF in Yoshi and Zelda DLC
        WRITE_MEMORY(ASLR(0x00FEFC7C), 0);
        WRITE_MEMORY(ASLR(0x00404C3E), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
        WRITE_MEMORY(ASLR(0x0040498C), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
        WRITE_MEMORY(ASLR(0x00404C3E), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);

        if (strncmp(packFileName, "zdlc02", 6) == 0)
        {
            *(This + 0x7A) |= 0x40;
            *(This + 0x7A) |= 0x20;
        }
        else if (strncmp(packFileName, "zdlc03", 6) == 0)
        {
            *(This + 0x7A) |= 0x80;
            *(This + 0x7A) |= 0x100;
        }
    }
    else
    {
        // Restore DOF Code if not Yoshi or Zelda DLC
        WRITE_MEMORY(ASLR(0x00FEFC7C), ((char*)ASLR(0x00F6072C))[9]);
        WRITE_MEMORY(ASLR(0x00404C3E), 0x88, 0x15, 0x7C, 0xFC, 0xFE, 0x00);
        WRITE_MEMORY(ASLR(0x0040498C), 0x88, 0x0D, 0x7C, 0xFC, 0xFE, 0x00);
        WRITE_MEMORY(ASLR(0x00404C3E), 0x88, 0x15, 0x7C, 0xFC, 0xFE, 0x00);
    }

    return This;
}

HOOK(void, __fastcall, CHudGameMainDisplaySetInfoHook, ASLR(0x00505DF0), app::HUD::CHudGameMainDisplay* This, int* edx, int* a2, float a3)
{
    originalCHudGameMainDisplaySetInfoHook(This, edx, a2, a3);

    This->HeartLifeUpdate(a2, a3);
}

HOOK(void, __fastcall, CHudGameMainDisplayInitLayerHook, ASLR(0x00503780), app::HUD::CHudGameMainDisplay* This, int* edx)
{
    originalCHudGameMainDisplayInitLayerHook(This, edx);

    if ((This->Flags & 0x100))
    {
        This->LayerController = This->GOCHud->CreateLayerController(This->field_E0, "info_ring_zdlc03", 0x12);
        This->LayerController->PlayAnimation("Intro_Anim", 0, 0);
        This->LayerController->SetVisible(true);
    }
}

void app::HUD::CHudGameMainDisplay::__ct()
{
    INSTALL_HOOK(CHudGameMainDisplayHook);
}

void app::HUD::CHudGameMainDisplay::SpecialRingUpdate()
{
    INSTALL_HOOK(SpecialRingUpdateHook);
}

void app::HUD::CHudGameMainDisplay::InitLayer()
{
    INSTALL_HOOK(CHudGameMainDisplayInitLayerHook);
}

void app::HUD::CHudGameMainDisplay::SetInfo()
{
    INSTALL_HOOK(CHudGameMainDisplaySetInfoHook);
}
