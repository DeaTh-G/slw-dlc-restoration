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

char IsZeldaDLC()
{
    const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*) * app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0)
        return 1;
    return 0;
}

HOOK(void, __fastcall, CStageSoundDirectorPlayResultBGMHook, ASLR(0x0095CB50), int* This, void* edx)
{
	if (IsZeldaDLC())
		return;

	originalCStageSoundDirectorPlayResultBGMHook(This, edx);
}

void app::xgame::CStageSoundDirector::LoadData()
{
    INSTALL_HOOK(CStageSoundDirectorLoadDataHook);
}

void app::xgame::CStageSoundDirector::PlayResultBGM()
{
	INSTALL_HOOK(CStageSoundDirectorPlayResultBGMHook);
}