#pragma once

namespace slw_dlc_restoration
{
	class EnemyStalBabyInfo : public app::EnemyStalBabyInfo
	{
	private:
		inline static const char* ms_pPacfileName = "EnemyStalbabyFix.pac";

	public:
		EnemyStalBabyInfo();
	};
}