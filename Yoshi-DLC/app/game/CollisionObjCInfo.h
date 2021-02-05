#pragma once

namespace app
{
	namespace game
	{
		class alignas(16) CollisionObjCInfo
		{
		public:
			short field_00;
			short field_02;
			short CollisionType;
			short field_06;
			int ShapeType;
			int field_0C;
			int* HFramePointer;
			int field_14;
			int field_18;
			int field_1C;
			Vector3 Position;
			int flags;
			int field_34;
			int field_38;
			float field_3C;
			unsigned int Data[4];
			Vector3 CollisionSize;


			inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x4B6190), CollisionObjCInfo* This);
			inline static FUNCTION_PTR(void, __thiscall, SetLocalPosition, ASLR(0x4B61D0), CollisionObjCInfo* This, Vector3* position);
		};
	}
}