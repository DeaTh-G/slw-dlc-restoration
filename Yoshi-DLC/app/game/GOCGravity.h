#pragma once

namespace app
{
	namespace game
	{
		class GOCGravity
		{
		public:
			inline static FUNCTION_PTR(int, __cdecl, SimpleSetup, ASLR(0x004BF5C0), GameObject* This, int a2);
			inline static FUNCTION_PTR(csl::math::Vector3*, __thiscall, GetGravityDirection, ASLR(0x004BF160), int* This);
		};
	}
}