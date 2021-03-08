#pragma once

namespace app
{
	namespace fnd
	{
		class ReferencedObject
		{
		public:
			int field_00;
			short field_04;
			short field_06;
			csl::fnd::IAllocator* field_08;

			inline static FUNCTION_PTR(ReferencedObject*, __cdecl, New, ASLR(0x0048D4A0), int size, csl::fnd::IAllocator* allocator);
		};
	}
}