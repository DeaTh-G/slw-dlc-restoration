#pragma once
#include "ModLoader.h"

namespace slw_dlc_restoration::Player
{
	class VirtualSonicInfo
	{
	public:
		static void InstallPatches(ModInfo_t* in_pModInfo);
		static void InstallHooks();
	};
}