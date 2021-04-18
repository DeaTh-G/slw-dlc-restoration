#include "pch.h"

HOOK(void, __cdecl, RegisterResourceHook, ASLR(0x008FE300), int* a1, int* a2, int* a3, int a4)
{
    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0)
        a4 = 1;

    if (IsAlwaysLinkSonic)
        a4 = 1;

    originalRegisterResourceHook(a1, a2, a3, a4);
}

void app::Player::CVisualSonic::RegisterResource()
{
    INSTALL_HOOK(RegisterResourceHook);
}
