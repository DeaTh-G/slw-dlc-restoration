#include "pch.h"

HOOK(void, __fastcall, WorldAreaMapInfoInitializeHook, ASLR(0x00949330), int* a1, int* edx, int* a2)
{
    originalWorldAreaMapInfoInitializeHook(a1, edx, a2);

    int packFile = 0;
    app::ObjUtil::GetPackFile(&packFile, "Linksonic.pac");

    app::ObjUtil::GetModelResource(a1 + 4, "chr_Linksonic", &packFile);
    app::ObjUtil::GetSkeletonResource(a1 + 5, "chr_Linksonic", packFile);
}

HOOK(void, __fastcall, WorldAreaMapInfoLoadHook, ASLR(0x00948520), int* a1, int* edx, app::CObjInfoFileLoader& loader)
{
    loader.Load("Linksonic.pac", 0x4001);
    originalWorldAreaMapInfoLoadHook(a1, edx, loader);
}

void app::WorldAreaMapInfo::Load()
{
    INSTALL_HOOK(WorldAreaMapInfoLoadHook);
}

void app::WorldAreaMapInfo::Initialize()
{
    INSTALL_HOOK(WorldAreaMapInfoInitializeHook);
}