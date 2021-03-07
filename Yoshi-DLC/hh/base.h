#pragma once

namespace hh
{
	namespace base
	{
		class CRefCountObject
		{
		public:
			int vftable;
			int RefCount;
		};
		
		inline static FUNCTION_PTR(int*, __cdecl, GetUserMemoryAllocator, ASLR(0x00C1E3B0), int* a1);
	}
}