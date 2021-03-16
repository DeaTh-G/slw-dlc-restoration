#pragma once

namespace app
{
	namespace xgame
	{
		class MsgLeaveEventCollision : public fnd::MessageNew
		{
		public:
			int field_18;
			int field_1C;
			int ActorID;
			int field_24;
			int field_28;
		};
	}
}