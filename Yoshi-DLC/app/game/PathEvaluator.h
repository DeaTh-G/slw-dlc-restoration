#pragma once

namespace app
{
	namespace game
	{
		class PathEvaluator
		{
		public:
			int field_00;
			int field_04;
			int field_08;
			int field_0C;

			inline static FUNCTION_PTR(void, __thiscall, __ct, ASLR(0x004D8D90), PathEvaluator* This);
			inline static FUNCTION_PTR(int*, __thiscall, SetDistance, ASLR(0x004D8CC0), PathEvaluator* This, void* a2);
			inline static FUNCTION_PTR(int*, __thiscall, SetPathObject, ASLR(0x004D8DC0), PathEvaluator* This, void* a2);
		};
	}
}