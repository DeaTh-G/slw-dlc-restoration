#pragma once

namespace app
{
	namespace game
	{
		class MoveStraight
		{
		public:
			struct MoveParameter {
				Vector3 Position;
				Quaternion Rotation;
				float field_20;
				int field_24;
				short field_28;
				char field_2A;
				char field_2B;
				int field_2C;
				Vector3 field_30;
				Vector3 field_40;
				Vector3 field_50;
				float field_60;
			};

			inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x004C9350), void* This);
			inline static FUNCTION_PTR(int*, __thiscall, SetMoveDistance, ASLR(0x004C9160), void* This, float Distance, float a3);
			inline static FUNCTION_PTR(int*, __thiscall, SetupParamater, ASLR(0x004C9910), void* This, MoveParameter* startPos);
		};
	}
}