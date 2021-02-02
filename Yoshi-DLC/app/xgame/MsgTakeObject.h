#pragma once
#include "framework.h"

struct MessageTakeObject // 0x28
{
	int field_00;
	int field_04;
	int field_08;
	int field_0C;
	short field_10;
	short field_12;
	int field_14;
	int field_18;
	int field_1C;
	short field_20;
	short field_22;
	int shapeID;
};

namespace app
{
	namespace xgame
	{
		class MsgTakeObject
		{
		public:
			inline static FUNCTION_PTR(int, __thiscall, __ct, ASLR(0x005366C0), MessageTakeObject* This, int a2);
			inline static FUNCTION_PTR(void, __thiscall, SetShapeUserID, ASLR(0x00536700), MessageTakeObject* This, char a2);
		};
	}
}