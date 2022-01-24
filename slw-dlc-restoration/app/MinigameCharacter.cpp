#include "pch.h"

HOOK(void, __fastcall, MinigameCharacterInfoLoadHook, ASLR(0x007335D0), int* a1, int* edx, app::CObjInfoFileLoader& loader)
{
    loader.Load("Linksonic.pac", 0x4001);
    originalMinigameCharacterInfoLoadHook(a1, edx, loader);
}


HOOK(void, __fastcall, MinigameCharacterInfoInitializeHook, ASLR(0x00733AB0), int* a1, int* edx, int* a2)
{
    originalMinigameCharacterInfoInitializeHook(a1, edx, a2);

    int packFile = 0;
    app::ObjUtil::GetPackFile(&packFile, "Linksonic.pac");

    app::ObjUtil::GetModelResource(a1 + 4, "chr_Linksonic", &packFile);
    app::ObjUtil::GetSkeletonResource(a1 + 10, "chr_Linksonic", packFile);
}

void app::MinigameCharacterInfo::Load()
{
    INSTALL_HOOK(MinigameCharacterInfoLoadHook);
}

void app::MinigameCharacterInfo::Initialize()
{
    INSTALL_HOOK(MinigameCharacterInfoInitializeHook);
}
