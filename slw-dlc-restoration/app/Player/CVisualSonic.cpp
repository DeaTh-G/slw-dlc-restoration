#include "pch.h"

HOOK(void, __cdecl, RegisterResourceHook, ASLR(0x008FE300), int* a1, int* a2, int* a3, int a4)
{
    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);

    switch (LinkSonicPlayType)
    {
    case PlayType::DEFAULT:
    {
        if (strncmp(packFileName, "zdlc03", 6) == 0)
            a4 = 1;
        else
            a4 = 0;

        break;
    }
    case PlayType::ALWAYS:
        a4 = 1;
        break;
    case PlayType::NEVER:
        a4 = 0;
        break;
    }

    originalRegisterResourceHook(a1, a2, a3, a4);
}

HOOK(void, __fastcall, ActivateSubHook, ASLR(0x008FE290), int* a1, int* edx)
{
    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);

    switch (LinkSonicPlayType)
    {
    case PlayType::DEFAULT:
    {
        if (strncmp(packFileName, "zdlc03", 6) == 0)
            *(*((*(unsigned int***)(a1 + 4)) + 210) + 6) |= 0x80000000;
        else
            *(*((*(unsigned int***)(a1 + 4)) + 210) + 6) &= ~0x80000000;

        break;
    }
    case PlayType::ALWAYS:
        *(*((*(unsigned int***)(a1 + 4)) + 210) + 6) |= 0x80000000;
        break;
    case PlayType::NEVER:
        *(*((*(unsigned int***)(a1 + 4)) + 210) + 6) &= ~0x80000000;
        break;
    }

    originalActivateSubHook(a1, edx);
}

void app::Player::CVisualSonic::ActivateSub()
{
    INSTALL_HOOK(ActivateSubHook);
}

void app::Player::CVisualSonic::RegisterResource()
{
    INSTALL_HOOK(RegisterResourceHook);
}
