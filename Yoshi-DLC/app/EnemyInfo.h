#pragma once
#include "..\framework.h"

namespace app
{
	class EnemyInfo
	{
	public:
		inline static FUNCTION_PTR(int*, __thiscall, GetEnemyPackfile, ASLR(0x00672480), int* This, int* a2);
		inline static FUNCTION_PTR(int*, __thiscall, GetModelResource, ASLR(0x006725F0), int* This, int* a2, const char* name);
		inline static FUNCTION_PTR(int*, __thiscall, GetSkeletonResource, ASLR(0x00672690), int* This, int* a2, const char* name);
		inline static FUNCTION_PTR(int*, __thiscall, GetAnimationResource, ASLR(0x00672640), int* This, int* a2, const char* name);
	};
}