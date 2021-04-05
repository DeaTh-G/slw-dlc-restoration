#include "pch.h"

HOOK(int, __fastcall, LoadLoadingScreenDataHook, ASLR(0x0093A6D0), int* This, void* edx, int a2)
{
    app::fnd::FileLoaderParam loader{};
    loader.SetMultiLanguageAttr(true, 0);
    app::GameMode::LoadFile("ui/ui_zdlc02_load_all.pac", &loader);
    loader.SetMultiLanguageAttr(0, 0);

    int result = originalLoadLoadingScreenDataHook(This, edx, a2);
    return result;
}

void app::GameModeStageBattle::LoadLoadingScreenData()
{
    INSTALL_HOOK(LoadLoadingScreenDataHook);
}
