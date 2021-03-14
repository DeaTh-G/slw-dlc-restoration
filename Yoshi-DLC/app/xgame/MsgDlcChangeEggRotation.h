#pragma once

namespace app
{
	namespace xgame
	{
		class MsgDlcChangeEggRotation : public fnd::MessageNew
		{
		public:
			int field_18;

			MsgDlcChangeEggRotation()
			{
				Type = fnd::PROC_MSG_DLC_CHANGE_EGG_ROTATION;
				field_08 = 0;
				field_0C = 0;
				field_10 = 0;
				field_12 = 0;
				field_14 = -1;
				field_18 = 0;
			}
		};
	}
}