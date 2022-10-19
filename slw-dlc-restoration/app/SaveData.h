#pragma once

namespace app
{
	namespace SaveData
	{
		static int CalcIndexFlagMinigameUnlocked(unsigned int row, unsigned int col)
		{
			return 4 * row + col + 490;
		}

		class CFlagSet
		{
		private:
			inline static FUNCTION_PTR(bool, __thiscall, f_GetFlagZoneUnlocked, ASLR(0x008FFDC0), CFlagSet*, int);
			inline static FUNCTION_PTR(int, __thiscall, f_CountRedRingZone, ASLR(0x00900080), CFlagSet*, int, int);

		public:
			bool GetFlagZoneUnlocked(int zoneNo)
			{
				return f_GetFlagZoneUnlocked(this, zoneNo);
			}

			int CountRedRingZone(int zoneNo, int zoneRedRingMax)
			{
				return f_CountRedRingZone(this, zoneNo, zoneRedRingMax);
			}

			bool GetBitFlag(int flagIndex)
			{
				return (*(int*)(*(int*)this + 4 * (flagIndex >> 5) + 16) & (1 << (flagIndex & 0x1F))) != 0;
			}

			bool GetMiniGameOpened(int row, int col)
			{
				int flagIndex = CalcIndexFlagMinigameUnlocked(row, col);
				return GetBitFlag(flagIndex);
			}
		};

		class CGameData
		{
		private:
			inline static FUNCTION_PTR(CFlagSet*, __thiscall, f_GetFlagSet, ASLR(0x00900B10), CGameData*, void*);

		public:
			CFlagSet* GetFlagSet(void* out)
			{
				return f_GetFlagSet(this, out);
			}
		};

		class CSaveData
		{
		private:
			inline static FUNCTION_PTR(CGameData*, __thiscall, f_GetCurrentGameData, ASLR(0x00900C90), CSaveData*, void*);

		public:
			CGameData* GetCurrentGameData(void* out)
			{
				return f_GetCurrentGameData(this, out);
			}
		};

		class CSaveManager
		{
		private:
			inline static FUNCTION_PTR(CSaveData*, __thiscall, f_GetSaveData, ASLR(0x00901220), CSaveManager*, void*);
		
		public:
			static CSaveData* GetSaveData(void* out)
			{
				return f_GetSaveData(*(CSaveManager**)ASLR(0x00FD3F9C), out);
			}
		};
	}

	static bool IsExistNextOpenCircusStage(int zoneNo, int* outCount, int* outCount2, void* out)
	{
		app::SaveData::CSaveData* saveData = app::SaveData::CSaveManager::GetSaveData(out);
		app::SaveData::CGameData* gameData = saveData->GetCurrentGameData(out);
		app::SaveData::CFlagSet* flagSet = gameData->GetFlagSet(out);

		if (!flagSet->GetFlagZoneUnlocked(zoneNo))
			return false;
	
		int redRingCount = flagSet->CountRedRingZone(zoneNo, 20);
		if (redRingCount >= 20)
			return false;
	
		*outCount = redRingCount / 5;
		*outCount2 = 5 * (redRingCount / 5) + 5 - redRingCount;
		
		return true;
	}
}