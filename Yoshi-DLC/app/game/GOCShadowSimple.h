#pragma once

namespace app
{
	namespace game
	{
		class GOCShadowSimple
		{
		public:
			inline static FUNCTION_PTR(int, __thiscall, Setup, ASLR(0x004CBDE0), int* This, int** shadowShapeCInfo);
			inline static FUNCTION_PTR(void, __thiscall, SetLocalOffsetPosition, ASLR(0x004CBEB0), int* This, csl::math::Vector3* position);
			inline static FUNCTION_PTR(void, __thiscall, SetMaxDistDown, ASLR(0x004CBE70), int* This, float a2);
			inline static FUNCTION_PTR(void, __thiscall, SetVisible, ASLR(0x004CBE50), int* This, bool isVisible);
		};
	}
}