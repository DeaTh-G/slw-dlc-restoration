#include "pch.h"

HOOK(void, __cdecl, RegisterResourceHook, ASLR(0x008FE300), int* a1, int* a2, int* a3, int a4)
{
    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0 || LinkSonicPlayType == PlayType::ALWAYS)
        a4 = 1;

    if (LinkSonicPlayType == PlayType::NEVER)
        a4 = 0;

    originalRegisterResourceHook(a1, a2, a3, a4);
}

HOOK(void, __fastcall, ActivateSubHook, ASLR(0x008FE290), int* a1, int* edx)
{
    if (LinkSonicPlayType == PlayType::NEVER)
    {
        originalActivateSubHook(a1, edx);
        return;
    }

    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0 || LinkSonicPlayType == PlayType::ALWAYS)
        *(*((*(unsigned int***)(a1 + 4)) + 210) + 6) |= 0x80000000;

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
