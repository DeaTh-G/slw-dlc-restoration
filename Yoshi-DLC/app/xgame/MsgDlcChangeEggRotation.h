#pragma once

namespace app
{
	namespace xgame
	{
		class MsgDlcChangeEggRotation : public fnd::MessageNew
		{
		public:
			int field_18;

			MsgDlcChangeEggRotation() : MessageNew()
			{
				Type = fnd::PROC_MSG_DLC_CHANGE_EGG_ROTATION;
				field_18 = 0;
			}
		};
	}
}