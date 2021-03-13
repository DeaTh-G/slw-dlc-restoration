#pragma once

namespace app
{
	namespace xgame
	{
		HOOK(bool, __cdecl, IsDLCStagePurchaseHook, ASLR(0x0095DB10), unsigned int dlc)
		{
			if (dlc == 1)
				return true;
			else
				return originalIsDLCStagePurchaseHook(dlc);
		}

		static void IsDLCStagePurchase()
		{
			INSTALL_HOOK(IsDLCStagePurchaseHook);
		}
	};
}