#include "pch.h"

HOOK(bool, __cdecl, IsDLCStagePurchaseHook, ASLR(0x0095DB10), unsigned int dlc)
{
    if (dlc == 1)
        return true;
    if (dlc == 2)
        return true;
    else
        return originalIsDLCStagePurchaseHook(dlc);
}

void app::xgame::IsDLCStagePurchase::Func()
{
    INSTALL_HOOK(IsDLCStagePurchaseHook);
}