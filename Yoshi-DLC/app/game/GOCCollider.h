#pragma once
#include "../../framework.h"
#include "CollisionObjCinfo.h"

namespace app
{
	namespace game
	{
		class GOCCollider
		{
		public:
			inline static FUNCTION_PTR(bool, __thiscall, SetEnable, ASLR(0x004CB4E0), int* This, bool isEnable);
			inline static FUNCTION_PTR(int, __thiscall, Setup, ASLR(0x004B68D0), int* This, int* unit);
			inline static FUNCTION_PTR(void, __thiscall, CreateShape, ASLR(0x004B6C50), int* This, CollisionObjCInfo* Collision);
		};
	}
}