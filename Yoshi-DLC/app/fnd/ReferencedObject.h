#pragma once

namespace app
{
	namespace fnd
	{
		class ReferencedObject
		{
		public:
			inline static FUNCTION_PTR(int*, __cdecl, New, ASLR(0x0048D4A0), int size, csl::fnd::IAllocator* allocator);
		};
	}
}