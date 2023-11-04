#include "pch.h"
#include "StageInfo.h"

HOOK(void, __cdecl, ReadMission_MissionHook, ASLR(0x009131D0), const char* in_pCat, app::game::LuaScript& in_rScript, void* in_pCtx)
{
    originalReadMission_MissionHook(in_pCat, in_rScript, in_pCtx);

	// Due to the below two stage attributes getting added through DLC, the code for them is missing on PC.
	// To ensure future compatibility with mods, we set these up after the fact instead of using mid-asm hooks
	// or the replacement of the original game's function.
	auto* pStageInfo = static_cast<app::StageInfo::CStageInfo*>(in_pCtx);
	auto* pStageData = *pStageInfo->Stages.get(pStageInfo->Stages.size() - 1);
	
	app::ut::InplaceTempArray<const char*, 10> stageAttrs{};
	if (in_rScript.GetStringArray("attrs", stageAttrs))
	{
		for (size_t i = 0; i < stageAttrs.size(); i++)
		{
			if (!strcmp(stageAttrs[i], "yoshiisland"))
				pStageData->StageFlags.set(app::StageInfo::SStageData::STAGE_FLAG_IS_YOSHIISLAND_STAGE, true);

			if (!strcmp(stageAttrs[i], "zelda"))
				pStageData->StageFlags.set(app::StageInfo::SStageData::STAGE_FLAG_IS_ZELDA_STAGE, true);
		}
	}
}

void slw_dlc_restoration::StageInfo::CStageInfo::InstallHooks()
{
	// General Hooks
    INSTALL_HOOK(ReadMission_MissionHook);
}
