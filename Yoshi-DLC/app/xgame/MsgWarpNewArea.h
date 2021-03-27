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
			csl::math::Vector3 TargetPosition;
			csl::math::Quaternion TargetRotation;
			int field_40;
			char field_44;
			char field_45;
			short field_46;
			int field_48;
			int field_4C;
		};
	}

}