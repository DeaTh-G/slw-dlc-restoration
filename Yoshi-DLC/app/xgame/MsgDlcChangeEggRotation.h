#pragma once

namespace app
{
	namespace xgame
	{
		class MsgDlcChangeEggRotation : public fnd::MessageNew
		{
		public:
			bool field_18;
			INSERT_PADDING(3);

			MsgDlcChangeEggRotation() : MessageNew()
			{
				Type = fnd::PROC_MSG_DLC_CHANGE_EGG_ROTATION;
				field_18 = false;
			}
		};
	}
}