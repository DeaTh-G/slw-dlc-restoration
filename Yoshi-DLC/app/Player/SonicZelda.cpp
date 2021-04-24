#include "pch.h"

HOOK(void, __fastcall, SonicZeldaInfoHook, ASLR(0x008F8800), int* a1, int* edx, int* a2)
{
    originalSonicZeldaInfoHook(a1, edx, a2);

    int packFile = 0;
    app::ObjUtil::GetPackFile(&packFile, "Linksonic.pac");

    app::ObjUtil::GetModelResource(a1 + 4, "chr_Linksonic_fixed", &packFile);
}

void app::Player::SonicZeldaInfo::Initialize()
{
    INSTALL_HOOK(SonicZeldaInfoHook);
}
