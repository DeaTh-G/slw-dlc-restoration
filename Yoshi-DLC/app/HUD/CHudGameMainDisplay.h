#pragma once

namespace app
{
	namespace HUD 
	{
		int* SetSpecialFlower(int* This)
		{
			char DstBuf[32]{};
			void* eggManager = app::EggManager::GetService(*(app::GameDocument**)(This + 0xA));
			if (This[0x52])
			{
				int FlowerFlag = 0;
				if (eggManager)
				{
					for (size_t i = 0; i < 5; ++i)
					{
						int flower = app::EggManager::IsYoshiSpecialFlowerTaked(eggManager, i);
						if (flower)
							FlowerFlag |= 1 << i;
					}

					if (This[0x68] != FlowerFlag)
					{
						This[0x68] = FlowerFlag;
						for (size_t i = 0; i < 5; i++)
						{
							snprintf(DstBuf, 32, "icon_s_ring_%d", i);
							SurfRide::Layer::SetCastPatternIndex(This + 0x52, DstBuf, FlowerFlag & 1);
							FlowerFlag >>= 1;
						}
						if (!*(This + 0x228))
						{
							app::HUD::SRUtility::SetAnimation(This + 0x52, "Display_2_Anim", 0, -1, 0, 0);
							This[0x3F] = 0x40000000;
							This[0x39] |= 0x20;
						}
					}
				}
			}

			return This;
		}

		void SpecialFlowerUpdate(int* This, float a2, char a3)
		{
			int result = *(This + 0x52);
			if (This[0x52])
			{
				SetSpecialFlower(This);
				int unknown_1 = This[0x39] >> 5;
				if (!a3)
					unknown_1 = ~unknown_1;

				int unknown_2 = unknown_1 & 1;

				if (unknown_2)
				{
					This[0x3F] = 0x3F000000;
					return;
				}

				float timer = *(float*)(This + 0x3F);
				if (timer > 0.0)
				{
					float timeDown = timer - a2;
					*(float*)(This + 0x3F) = timeDown;
					if (timeDown <= 0.0)
					{
						if ((This[0x39] & 0x20) != 0)
							app::HUD::SRUtility::SetAnimation(This + 0x52, "Hide_Anim", 0, -1.0, 0, 0);
						else
							app::HUD::SRUtility::SetAnimation(This + 0x52, "Display_2_Anim", 0, -1.0, 0, 0);
						This[0x39] ^= 0x20;
					}
				}
			}
		}

		HOOK(int*, __fastcall, SpecialRingUpdateHook, ASLR(0x005027A0), int* This, void* edx, float a2, char a3)
		{
			int* result = 0;
			const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*) * app::Document);
			if (strncmp(packFileName, "zdlc02", 6) == 0)
				SpecialFlowerUpdate(This, a2, a3);
			else
				result = originalSpecialRingUpdateHook(This, edx, a2, a3);
			return result;
		}

		HOOK(int*, __fastcall, CHudGameMainDisplayHook, ASLR(0x00503290), int* This, int* edx, char a2, int a3, int a4, char a5, int a6, int a7)
		{
			originalCHudGameMainDisplayHook(This, edx, a2, a3, a4, a5, a6, a7);

			const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*) * app::Document);
			if (strncmp(packFileName, "zdlc02", 6) == 0)
			{
				*(This + 0x7A) |= 0x40;
				*(This + 0x7A) |= 0x20;
			}

			return This;
		}

		class CHudGameMainDisplay
		{
		public:
			static void __ct()
			{
				INSTALL_HOOK(CHudGameMainDisplayHook);
			}

			static void SpecialRingUpdate()
			{
				INSTALL_HOOK(SpecialRingUpdateHook);
			}
		};
	}
}