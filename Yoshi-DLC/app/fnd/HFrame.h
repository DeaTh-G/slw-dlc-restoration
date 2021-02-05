#pragma once

namespace app
{
	namespace fnd
	{
		class HFrame
		{
		/*private:
			int* vfTable;
			int field_04;
			int field_08;
			int* field_0C; // GameObject + 0x19C
			int* field_10; // GameObject + 0x19C
			int* branchHFramePointer1;
			int* branchHFramePointer2;
			char Flag;
			char field_21;
			short field_22;
			int field_24;
			int field_28;
			int* field_2C;
			int* field_30;
			int* field_34;
			int field_38;
			int field_3C;
			int field_40;
			int* GOCVisualModelPointer;
			int* CollisionShapePointer;
			int field_4C;
			Quaternion field_50[7];
			int field_C0;
			int field_C4;
			int field_C8;
			int field_CC;
			Quaternion field_D0[5];
			int field_120;
			int field_124;
			int field_128;
			int field_12C;*/

		public:
			inline static FUNCTION_PTR(HFrame*, __thiscall, __ct, ASLR(0x0048E210), char* This);
			inline static FUNCTION_PTR(void, __thiscall, ResetFlag, ASLR(0x0048DCD0), int* This, int arg2);
			inline static FUNCTION_PTR(Vector3*, __thiscall, SetLocalTranslation, ASLR(0x0048DD30), int* This, Vector3* position);
			inline static FUNCTION_PTR(Quaternion*, __thiscall, SetLocalRotation, ASLR(0x0048DD80), int* This, Quaternion* rotation);
			inline static FUNCTION_PTR(void, __thiscall, AddChild, ASLR(0x0048E650), int* This, int* location);
		};
	}
}