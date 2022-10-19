#pragma once

namespace app
{
    class GameModeWorldAreaMap
    {
    public:
        static void Constructor();

        void CreateCircusTents(void* out)
        {
            app::SaveData::CSaveData* saveData = app::SaveData::CSaveManager::GetSaveData(out);
            app::SaveData::CGameData* gameData = saveData->GetCurrentGameData(out);
            app::SaveData::CFlagSet* flagSet = gameData->GetFlagSet(out);

            for (size_t i = 0; i < 7; i++)
            {
                for (size_t j = 0; j < 4; j++)
                {
                    size_t minigameNo = 4 * i + 34 + j;
                    if (flagSet->GetMiniGameOpened(i, j))
                    {
                        *((int*)this + 0x520 + minigameNo) = 0;
                        (*((app::WorldAreaMapDioramaInfo*)((int*)this + 0x118) + minigameNo)).m_IsActUnlocked = 1;
                    }
                }
            }

            for (size_t i = 0; i < 7; i++)
            {
                int unlockedMiniGameCount = 0;
                int nextUnlockRequirement = 0;

                bool isValidNext = app::IsExistNextOpenCircusStage(i, &unlockedMiniGameCount, &nextUnlockRequirement, out);
                if (isValidNext)
                {
                    size_t minigameNo = i * 4 + unlockedMiniGameCount + 34;
                    *((int*)this + 0x5A1 + i) = unlockedMiniGameCount;
                    *((int*)this + 0x520 + minigameNo) = 4;
                    (*((app::WorldAreaMapDioramaInfo*)((int*)this + 0x118) + minigameNo)).m_RedRing = (uint8_t)nextUnlockRequirement;
                }
                else
                {
                    *((int*)this + 0x5A1 + i) = -1;
                }
            }
        }
    };
}