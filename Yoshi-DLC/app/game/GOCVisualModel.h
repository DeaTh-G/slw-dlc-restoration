#pragma once

namespace app
{
	namespace fnd
	{
		class GOCVisualModel
		{
		public:
			class Description
			{
			public:
				int Model;
				int Skeleton;
				int field_08;
				int Animation;
				int field_10;
				int* HFramePointer;
				float field_18;
				int field_1C;
				int field_20;
				int field_24;
				int field_28;
				int field_2C;
				int field_30;
				int field_34;
				int field_38;
			
				inline static FUNCTION_PTR(Description*, __thiscall, __ct, ASLR(0x00495480), Description* This);
			};

			inline static FUNCTION_PTR(void, __thiscall, Setup, ASLR(0x00495850), int* This, Description* visualDescriptor);
			inline static FUNCTION_PTR(void, __thiscall, AttachAnimation, ASLR(0x004951B0), int* This, int* gocAnimation);
		};

		class GOCVisualTransformed
		{
		public:
			inline static FUNCTION_PTR(void, __thiscall, SetLocalTranslation, ASLR(0x00495EA0), int* This, Vector3* translation);
			inline static FUNCTION_PTR(void, __thiscall, SetLocalRotation, ASLR(0x00495ED0), int* This, Quaternion* rotation);
			inline static FUNCTION_PTR(void, __thiscall, SetLocalScale, ASLR(0x00496240), int* This, Vector3* scale);
		};
	}
}