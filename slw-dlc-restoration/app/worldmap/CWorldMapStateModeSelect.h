#pragma once

static float m_Positions[18] = {
	0, 0, 34.63999939f,
	30, 0, 17.31999969f,
	30, 0, -17.31999969f,
	0, 0, -34.63999939f,
	-30, 0, -17.31999969f,
	-30, 0, 17.31999969f
};

static const char* m_IconNames[8] = {
	"icon_hex",
	"icon_hexdark",
	"icon_singleplay",
	"icon_timeattack",
	"icon_multiplay",
	"icon_ranking",
	"icon_comment",
	"icon_Miiverse",
};

namespace app
{
	namespace worldmap
	{
		class CWorldMapStateModeSelect
		{
		public:
			static void Constructor();
			static void AddCallback();
			static void ProcMsgHudKeyUpdate();
		};
	}
}