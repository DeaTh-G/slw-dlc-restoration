#pragma once

namespace app
{
	namespace game
	{
		__declspec(align(16)) struct MotorInfo
		{
			csl::math::Quaternion someVector;
			float Speed;
			int Padding[3];
		};
		
		class GOCMotorRotate
		{
		public:
			inline static FUNCTION_PTR(int, __thiscall, Setup, ASLR(0x004C2B40), int* This, MotorInfo* Motor);
		};
	}
}