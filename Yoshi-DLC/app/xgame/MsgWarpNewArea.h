#pragma once

namespace app
{
	namespace xgame
	{
		__declspec(align(16)) struct MsgWarpNewArea
		{
		public:
			fnd::Message Base;
			int field_18;
			bool IsDestination2D;
			int8_t field_1D;
			int8_t field_1E;
			int8_t field_1F;
			csl::math::Vector3 TargetPosition;
			csl::math::Quaternion TargetRotation;
			int field_40;
			short field_44;
			short field_46;
			int field_48;
			int field_4C;
		};
	}

}