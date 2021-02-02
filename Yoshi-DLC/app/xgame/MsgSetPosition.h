#pragma once
#include "..\framework.h"
#include "../app/fnd/Message.h"
#include "../../LWTypes.h"

namespace app
{
	namespace xgame
	{
		__declspec(align(16)) struct MsgSetPosition
		{
		public:
			fnd::Message Base;
			int field_18;
			int field_1C;
			Vector3 Position;
		};
	}

}