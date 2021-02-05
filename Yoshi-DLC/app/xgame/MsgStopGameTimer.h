#pragma once

namespace app
{
	namespace xgame
	{
		__declspec(align(16)) struct MsgStopGameTimer
		{
		public:
			fnd::Message Base;
		};
	}

}