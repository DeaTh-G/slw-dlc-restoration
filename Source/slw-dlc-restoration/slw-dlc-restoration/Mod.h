#pragma once

namespace slw_dlc_restoration
{
	struct SConfiguration
	{
		struct SGlobalTweaks
		{
			bool FixMultiplayerBugs = true;
			bool IsSHC2021Build = false;
		};

		struct SYoshiTweaks
		{
			bool IsConsistentShadows = false;
			bool DisablePipeTransition = false;
		};

		struct SZeldaTweaks
		{
			bool FixLoftBirdMemoryLeak = true;
			bool FixTreasureChestRupeeCount = true;
			bool ScaleObjectsWithPlayer = true;

			enum EPlayType : uint
			{
				ePlayType_Default,
				ePlayType_Always,
				ePlayType_Never
			};

			EPlayType LinkSonicPlayType = EPlayType::ePlayType_Default;
			bool IsLinkSonicFixed = false;
			bool IsWaterParticleFixed = false;
			bool IsVirtualLinkSonic = false;
			bool IsAlwaysHeartLife = false;
			bool DisableChestLetterboxing = false;
			bool IsStalbabyFixed = false;
			bool UseUnusedGoalAnimation = false;
		};

		SGlobalTweaks GlobalTweaks{};
		SYoshiTweaks YoshiTweaks{};
		SZeldaTweaks ZeldaTweaks{};

		bool IsNightsDlcEnabled = true;
		bool IsYoshiDlcEnabled = true;
		bool IsZeldaDlcEnabled = true;
		bool Is4StarBombWispDlcEnabled = false;
		bool Is25LivesDlcEnabled = false;
		bool IsBombWispDlcEnabled = false;
		bool IsGoldOmochaoDlcEnabled = false;
		bool IsSonicOmochaoDlcEnabled = false;
	};
}

extern slw_dlc_restoration::SConfiguration CONFIGURATION;