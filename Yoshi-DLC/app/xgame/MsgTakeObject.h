#pragma once

namespace app
{
	namespace xgame
	{
		class MsgTakeObject : public fnd::MessageNew
		{
		public:
			int field_18;
			int field_1C;
			short field_20;
			short field_22;
			int shapeID;

			MsgTakeObject(int a1) : MessageNew()
			{
				Type = fnd::PROC_MSG_TAKE_OBJECT;
				field_18 = a1;
				field_1C = -1;
				field_20 = 0;
				field_22 = 0;
				shapeID = 0;
			}

			inline static FUNCTION_PTR(void, __thiscall, SetShapeUserID, ASLR(0x00536700), MsgTakeObject* This, char a2);
		};
	}
}