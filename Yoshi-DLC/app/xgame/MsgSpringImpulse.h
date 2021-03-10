#pragma once

namespace app
{
	namespace xgame
	{
		__declspec(align(16)) struct MsgSpringImpulse
		{
		public:
			fnd::Message Base;
			int field_18;
			int field_1C;
			csl::math::Vector3 ObjectPosition;
			csl::math::Vector3 LaunchVector;
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

			inline static FUNCTION_PTR(MsgSpringImpulse*, __thiscall, __ct, ASLR(0x00536CF0), MsgSpringImpulse* This, csl::math::Vector3* objectPosition, csl::math::Vector3* launchVector, float outOfControl, float travelTime);
		};
	}

}