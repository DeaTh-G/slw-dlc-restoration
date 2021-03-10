#pragma once

namespace app
{
	namespace xgame
	{
		__declspec(align(16)) struct MsgCatchPlayer
		{
		public:
			fnd::Message Base;
			int field_18;
			int field_1C;
			csl::math::Matrix34 field_20;
			int field_60;
			int field_64;
			int field_68;
			int field_6C;
		};
	}

}