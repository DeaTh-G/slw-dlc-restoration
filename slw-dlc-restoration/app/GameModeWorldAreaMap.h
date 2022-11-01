#pragma once
#pragma push_macro("SendMessage")
#undef SendMessage

namespace app
{
    class HudRadiconAchieved : public GameObject
    {
    public:
        class Cinfo
        {
            int field_00{};
            int field_04{};
            int field_08{};
        };

    private:
        INSERT_PADDING(432);

        inline static FUNCTION_PTR(HudRadiconAchieved*, __thiscall, __ct, ASLR(0x0051A380), void* This, Cinfo* info);

    public:
        static HudRadiconAchieved* CreateInstance(Cinfo* info)
        {
            void* pMemory = GameObject::operator new(sizeof(HudRadiconAchieved));
            __ct(pMemory, info);
            return reinterpret_cast<HudRadiconAchieved*>(pMemory);
        }
    };

    class GameModeWorldAreaMap : public GameMode, TTinyFsm<GameModeWorldAreaMap, GameModeUtil::Event<GameModeWorldAreaMap>, false>
    {
    private:
        inline static FUNCTION_PTR(state_t*, __thiscall, f_StateModeSelect, ASLR(0x00945A30),
            GameModeWorldAreaMap* This, state_t* r, const GameModeUtil::Event<GameModeWorldAreaMap>& e);
        inline static FUNCTION_PTR(state_t*, __thiscall, f_StateGameEnd, ASLR(0x0093F200),
            GameModeWorldAreaMap* This, state_t* r, const GameModeUtil::Event<GameModeWorldAreaMap>& e);
        inline static FUNCTION_PTR(void, __thiscall, f_UpdateHudKey, ASLR(0x0093D650),
            GameModeWorldAreaMap* This, int* a2);

    public:
        static void Constructor();
        static void StateModeSelect();

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
		int* NewSubContext()
		{
			*((int*)this + 0x29) += 1;
			return ((int*)this + 0x2A) + (0x10 * *((int*)this + 0x29));
		}

		int** GetCurrentSubContext()
		{
			return (int**)((int*)this + 0x2A) + (0x10 * *((int*)this + 0x29));
		}

		void DeleteSubContext()
		{
			*((int*)this + 0x29) -= 1;
		}

		state_t StateModeSelect(const GameModeUtil::Event<GameModeWorldAreaMap>& e)
		{
			state_t r;
			f_StateModeSelect(this, &r, e);
			return r;
		}

		state_t StateGameEnd(const GameModeUtil::Event<GameModeWorldAreaMap>& e)
		{
			state_t r;
			f_StateGameEnd(this, &r, e);
			return r;
		}

		void UpdateHudKey(int* a1)
		{
			return f_UpdateHudKey(this, a1);
		}

		state_t* StateRadiconSelect(state_t* r, const GameModeUtil::Event<GameModeWorldAreaMap>& e)
		{
			switch (e.getSignal())
			{
			case SIGNAL_ENTER:
				break;
			case SIGNAL_LEAVE:
				break;
			case SIGNAL_UPDATE:
				*r = &GameModeWorldAreaMap::StateGameEnd;
				break;
			default:
				//*r = &GameModeWorldAreaMap::StateGameEnd;
				break;
			}
			return r;

			/*if (e.getSignal())
			{
				switch (e.getSignal())
				{
				case SIGNAL_ENTER:
				{
					int* subContext = NewSubContext();
					if (subContext)
						*subContext = 0;

					HudRadiconAchieved::Cinfo info{};
					HudRadiconAchieved* hud = HudRadiconAchieved::CreateInstance(&info);
					*subContext = (int)hud;
					this->m_pDocument->AddGameObject(hud);

					xgame::MsgHudCmnButtonDisplay msg { 0 };
					GameObject* hudDisplay = (GameObject*)*((int**)this + 0x5B1);
					hudDisplay->SendMessage(msg);
					break;
				}
				case SIGNAL_LEAVE:
				{
					xgame::MsgHudCmnButtonDisplay msg { 16 };
					GameObject* hud = (GameObject*)*((int**)this + 0x5B1);
					hud->SendMessage(msg);

					int** currentContext = GetCurrentSubContext();
					if (*currentContext)
						((GameObject*)*currentContext)->Kill();

					DeleteSubContext();
					break;
				}
				default:
					*r = &GameModeWorldAreaMap::StateModeSelect;
					break;
				}
			}
			else
			{
				int** currentContext = GetCurrentSubContext();
				this->UpdateHudKey(*currentContext);
				if (*(*currentContext + 0x75))
				{
					if (*(*currentContext + 0x75) == 2)
					{
						xgame::MsgHudCmnButtonDisplay msg{ 16 };
						GameObject* hud = (GameObject*)*((int**)this + 0x5B1);
						hud->SendMessage(msg);

						*r = &GameModeWorldAreaMap::StateModeSelect;
					}
					else
					{
						*((int*)this + 0x25) = 5 * *(*currentContext + 0x79) + *(*currentContext + 0x7A);
						*((int*)this + 0x28) |= 8;
						*((int*)this + 0x28) |= 2;
						*r = &GameModeWorldAreaMap::StateGameEnd;
					}
				}
			}
			return r;*/
		}
    };
}

#pragma pop_macro("SendMessage")