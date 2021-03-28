#pragma once

namespace app
{
	namespace xgame
	{
		class MsgNotifyObjectEvent : public fnd::MessageNew
		{
		public:
			int field_18{};

			MsgNotifyObjectEvent() : MessageNew()
			{
				Type = fnd::PROC_MSG_NOTIFY_OBJECT_EVENT;
			}
		};
	}

}