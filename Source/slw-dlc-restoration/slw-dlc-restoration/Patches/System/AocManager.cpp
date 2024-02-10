#include "pch.h"
#include "AocManager.h"

HOOK(void, __fastcall, GetPurchaseInfoHook, ASLR(0x004B0920), app::AocManager* in_pThis, void* edx, void* pOpenedTitle, bool* out_pIsPurchased, ushort* in_pDlcIndex, uint in_count)
{
	originalGetPurchaseInfoHook(in_pThis, edx, pOpenedTitle, out_pIsPurchased, in_pDlcIndex, in_count);

	// Depending on the configuration of the mod the DLCs get marked as purchased in the following code.
	// This is not accurate as to how the game would be validating purchases and is just forcing them on and off.
	for (size_t i = 0; i < in_count; i++)
	{
		switch (in_pDlcIndex[i])
		{
		case 0x0010:
			out_pIsPurchased[i] = CONFIGURATION.IsNightsDlcEnabled;
			break;

		case 0x0011:
			out_pIsPurchased[i] = CONFIGURATION.IsYoshiDlcEnabled;
			break;

		case 0x0012:
			out_pIsPurchased[i] = CONFIGURATION.IsZeldaDlcEnabled && !CONFIGURATION.GlobalTweaks.IsSHC2021Build;
			break;

		case 0x0013:
			out_pIsPurchased[i] = CONFIGURATION.Is4StarBombWispDlcEnabled && !CONFIGURATION.GlobalTweaks.IsSHC2021Build;
			break;

		case 0x0014:
			out_pIsPurchased[i] = CONFIGURATION.Is25LivesDlcEnabled && !CONFIGURATION.GlobalTweaks.IsSHC2021Build;
			break;

		case 0x0015:
			out_pIsPurchased[i] = CONFIGURATION.IsBombWispDlcEnabled && !CONFIGURATION.GlobalTweaks.IsSHC2021Build;
			break;

		case 0x0016:
			out_pIsPurchased[i] = CONFIGURATION.IsGoldOmochaoDlcEnabled && !CONFIGURATION.GlobalTweaks.IsSHC2021Build;
			break;

		case 0x0017:
			out_pIsPurchased[i] = CONFIGURATION.IsSonicOmochaoDlcEnabled && !CONFIGURATION.GlobalTweaks.IsSHC2021Build;
			break;

		default:
			break;
		}
	}

	// Turn off the pre-order bonus DLCs if the "Disable DLC Prizes" code is enabled.
	if (*(int*)(ASLR(0x0094595B)) == 0x90909090 && *((int*)(ASLR(0x0094595B)) + 1) == 0x90909090 && *((int*)(ASLR(0x0094595B)) + 2) == 0x90909090
		&& *(int*)(ASLR(0x00945630)) == 0x90909090 && *((byte*)(ASLR(0x00945630)) + 4) == 0x90)
	{
		for (size_t i = 0; i < in_count; i++)
		{
			switch (in_pDlcIndex[i])
			{
			case 0x0013:
			case 0x0014:
			case 0x0015:
			case 0x0016:
			case 0x0017:
				out_pIsPurchased[i] = false;
				break;
			}
		}
	}
}

void slw_dlc_restoration::AocManager::InstallHooks()
{
	// General Hooks
	INSTALL_HOOK(GetPurchaseInfoHook);
}
