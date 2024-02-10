#pragma once

namespace slw_dlc_restoration::HUD
{
	class CHudGameMainDisplay : public app::HUD::CHudGameMainDisplay
	{
	public:
		void HeartLifeUpdate(Sinfo& in_rInfo, float in_deltaTime, bool in_isEnable);
		static void InstallHooks();
	};
}