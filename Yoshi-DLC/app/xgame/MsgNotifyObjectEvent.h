#pragma once

namespace app
{
	namespace xgame
	{
		__declspec(align(16)) struct MsgNotifyObjectEvent
		{
		public:
			fnd::Message Base;
			int field_18;
		};
	}

}