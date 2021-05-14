#include "pch.h"

HOOK(void, __fastcall, CStageSoundDirectorLoadDataHook, ASLR(0x0095B240), int* This, void* edx)
{
    originalCStageSoundDirectorLoadDataHook(This, edx);

	if (!IsAlwaysHeartLife)
		return;

	int result;
	app::fnd::FileLoaderParam loader{};
	app::fnd::FileLoader::LoadFile(*(app::fnd::FileLoader**)ASLR(0x00FD40A4), &result, "sound/se_object_zdlc03.acb", &loader);
}

void app::xgame::CStageSoundDirector::LoadData()
{
    INSTALL_HOOK(CStageSoundDirectorLoadDataHook);
}
