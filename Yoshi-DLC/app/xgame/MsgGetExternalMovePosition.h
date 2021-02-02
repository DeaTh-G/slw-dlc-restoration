#pragma once
#include "..\framework.h"
#include "../app/fnd/Message.h"
#include "../../LWTypes.h"

namespace app
{
	namespace xgame
	{
		__declspec(align(4)) struct MsgGetExternalMovePosition
		{
		public:
			fnd::Message Base;
			Matrix34* Transform;
		};
	}

}