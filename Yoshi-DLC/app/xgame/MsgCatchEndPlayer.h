#pragma once
#include "..\framework.h"
#include "../app/fnd/Message.h"

namespace app
{
	namespace xgame
	{
		__declspec(align(4)) struct MsgCatchEndPlayer
		{
		public:
			fnd::Message Base;
			bool field_18;
			int8_t field_19;
			int8_t field_20;
			int8_t field_21;
		};
	}

}