#pragma once

namespace app
{
    class CGameSequence
    {
    private: 
        inline static FUNCTION_PTR(int, __thiscall, f_PushMovieEventQueue, ASLR(0x0090DBE0), CGameSequence* This, int eventNo);
        inline static FUNCTION_PTR(int, __thiscall, f_SeqGotoPlayMovie, ASLR(0x00911B70), CGameSequence* This);
        inline static FUNCTION_PTR(void, __thiscall, ms_fpSaveRedRingInfo, ASLR(0x0090D5C0), CGameSequence*, int, int);

    public:
        static void CompleteDLCStage();
        static void UnlockStageOnCompleteStage();

        int PushMovieEventQueue(int eventNo) { return f_PushMovieEventQueue(this, eventNo); }
        int SeqGotoPlayMovie() { return f_SeqGotoPlayMovie(this); }
        void SaveRedRingInfo(int a2, int a3) { ms_fpSaveRedRingInfo(this, a2, a3); }

        void OnMinigameOpened(int row, int col)
        {
            int save{};
            app::WorldAreaMapCInfo::Event mapEvent{ 3, row, col };

            ((app::WorldAreaMapCInfo*)((int*)this + 0x37))->AddEvent(&mapEvent);

            app::SaveData::CSaveData* saveData = app::SaveData::CSaveManager::GetSaveData(&save);
            app::SaveData::CGameData* gameData = saveData->GetCurrentGameData(&save);
            app::SaveData::CFlagSet* flagSet = gameData->GetFlagSet(&save);
            flagSet->SetMiniGameOpened(row, col, true);
        }

        int UnlockMinigames(int zoneIndex, int a3)
        {
            int save{};

            app::SaveData::CSaveData* saveData = app::SaveData::CSaveManager::GetSaveData(&save);
            app::SaveData::CGameData* gameData = saveData->GetCurrentGameData(&save);
            app::SaveData::CFlagSet* flagSet = gameData->GetFlagSet(&save);

            int preSaveRedRingCount = flagSet->CountRedRingZone(zoneIndex, 20);
            this->SaveRedRingInfo(zoneIndex, a3);
            int postSaveRedRingCount = flagSet->CountRedRingZone(zoneIndex, 20);
            if (postSaveRedRingCount / 5 > preSaveRedRingCount / 5)
            {
                if (!flagSet->GetMiniGameOpened(zoneIndex, postSaveRedRingCount / 5 - 1))
                    OnMinigameOpened(zoneIndex, postSaveRedRingCount / 5 - 1);
            }

            return postSaveRedRingCount;
        }
    };
}