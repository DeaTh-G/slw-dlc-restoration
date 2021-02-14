#pragma once

namespace app
{
	namespace game
	{
		class GOCMovement
		{
		public:
			inline static FUNCTION_PTR(int, __thiscall, SetupController, ASLR(0x004C3C70), int* This, void* a2);
			inline static FUNCTION_PTR(int, __thiscall, DisableMovementFlag, ASLR(0x004C3800), int* This, bool isDisabled);
			inline static FUNCTION_PTR(int*, __thiscall, GetContextParam, ASLR(0x00934460), int* This);
		};
	}
}