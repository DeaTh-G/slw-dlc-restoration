#pragma once
#include "framework.h"

namespace hh
{
	class base
	{
	public:
		FUNCTION_PTR(int*, __cdecl, GetUserMemoryAllocator, ASLR(0x00C1E3B0), int* a1);
	};
}