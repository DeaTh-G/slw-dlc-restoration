#pragma once
#include "..\framework.h"
#include "../app/fnd/Message.h"

namespace app
{
	namespace xgame
	{
		__declspec(align(4)) struct MsgResumeGameTimer
		{
		public:
			fnd::Message Base;
		};
	}

}