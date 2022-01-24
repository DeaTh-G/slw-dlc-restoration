#include "pch.h"

HOOK(void, __fastcall, VirtualSonicInfoLoadHook, ASLR(0x008F8BA0), int* a1, int* edx, app::CObjInfoFileLoader& loader)
{
    loader.Load("Linksonic_red.pac", -1);
    originalVirtualSonicInfoLoadHook(a1, edx, loader);
}

HOOK(void, __fastcall, VirtualSonicInfoInitializeHook, ASLR(0x008F8C00), int* a1, int* edx, int* a2)
{
    originalVirtualSonicInfoInitializeHook(a1, edx, a2);

    int packFile = 0;
    app::ObjUtil::GetPackFile(&packFile, "Linksonic.pac");
    app::ObjUtil::GetModelResource(a1 + 4, "chr_Linksonic", &packFile);
    if (IsLinkSonicFixed)
        app::ObjUtil::GetModelResource(a1 + 4, "chr_Linksonic_fixed", &packFile);

    app::ObjUtil::GetPackFile(&packFile, "Linksonic_red.pac");
    app::ObjUtil::GetModelResource(a1 + 5, "chr_Linksonic2p", &packFile);
    if (IsLinkSonicFixed)
        app::ObjUtil::GetModelResource(a1 + 5, "chr_Linksonic2p_fixed", &packFile);

    a1[8] = 0;
    a1[9] = 0;
}

void app::Player::VirtualSonicInfo::Load()
{
    INSTALL_HOOK(VirtualSonicInfoLoadHook);
}

void app::Player::VirtualSonicInfo::Initialize()
{
    INSTALL_HOOK(VirtualSonicInfoInitializeHook);
}
