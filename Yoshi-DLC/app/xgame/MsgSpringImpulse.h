#pragma once
#include "..\framework.h"
#include "../app/fnd/Message.h"
#include "../../LWTypes.h"

namespace app
{
	namespace xgame
	{
		struct MsgSpringImpulse
		{
		public:
			fnd::Message Base;
			int field_18;
			int field_1C;
			Vector3 ObjectPosition;
			Vector3 LaunchVector;
			float OutOfControl;
			float TravelTime;
			int field_48;
			float OutOfParkour;
			int field_50;
			int field_54;
			int field_58;
			int field_5C;
			int field_60;
			int field_64;
			int field_68;
			int field_6C;

			inline static FUNCTION_PTR(MsgSpringImpulse*, __thiscall, __ct, ASLR(0x00536CF0), MsgSpringImpulse* This, Vector3* objectPosition, Vector3* launchVector, float outOfControl, float travelTime);
		};
	}

}