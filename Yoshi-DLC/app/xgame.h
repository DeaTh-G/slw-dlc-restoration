#pragma once

namespace app
{
	namespace xgame
	{
		HOOK(char, __cdecl, IsDLCStagePurchaseHook, ASLR(0x0095DB10), unsigned int dlc)
		{
			if (dlc == 1)
				return '\1';
			if (dlc == 2)
				return '\1';
			else
				return originalIsDLCStagePurchaseHook(dlc);
		}

		static void IsDLCStagePurchase()
		{
			INSTALL_HOOK(IsDLCStagePurchaseHook);
		}
	};
}