#pragma once

namespace app
{
	namespace HUD 
	{
		inline static int* SetSpecialFlower(int* This)
		{
			char DstBuf[32]{};
			EggManager* eggManager = app::EggManager::GetService(*(app::GameDocument**)(This + 0xA));
			if (This[0x52])
			{
				int FlowerFlag = 0;
				if (eggManager)
				{
					for (size_t i = 0; i < 5; ++i)
					{
						int flower = eggManager->IsYoshiSpecialFlowerTaked(i);
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

		inline static void SpecialFlowerUpdate(int* This, float a2, char a3)
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

		class CHudGameMainDisplay
		{
		public:
			static void __ct();
			static void SpecialRingUpdate();
		};
	}
}