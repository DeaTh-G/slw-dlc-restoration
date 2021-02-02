#pragma once
#include "framework.h"
#include "../app/GameObject.h"

namespace app
{
	namespace game
	{
		class GOCSound
		{
		public:
			inline static FUNCTION_PTR(void, __cdecl, SimpleSetup, ASLR(0x004CCE30), GameObject* Object, int a2, int a3);
			inline static FUNCTION_PTR(void, __thiscall, Play, ASLR(0x004CD180), int* This, int* DeviceTag, const char* CueName, int a4);
			inline static FUNCTION_PTR(void, __thiscall, Play3D, ASLR(0x004CD060), int* This, float* DeviceTag, const char* CueName, int a4);
		};
	}
}