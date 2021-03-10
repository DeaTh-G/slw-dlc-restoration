#pragma once

namespace app
{
	namespace xgame
	{
		__declspec(align(16)) struct MsgGetExternalMovePosition
		{
		public:
			fnd::Message Base;
			csl::math::Matrix34* Transform;
		};
	}

}