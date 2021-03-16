#pragma once

namespace app
{
	namespace xgame
	{
		class Blackboard
		{
		public:
			inline static FUNCTION_PTR(void*, __cdecl, GetService, ASLR(0x009530E0));
			inline static FUNCTION_PTR(int*, __thiscall, GetPlayerInformation, ASLR(0x0049CAC0), int* This, void* a2);
		};
	}
}