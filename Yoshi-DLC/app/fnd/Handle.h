#pragma once

namespace app
{
	namespace fnd
	{
		class HandleBase
		{
		public:
			int field_00;
			int field_04;

			inline static FUNCTION_PTR(int*, __thiscall, __as, ASLR(0x004999A0), void* a1, void* a2);
		};

		class Handle
		{
		public:
			inline static FUNCTION_PTR(int*, __thiscall, Get, ASLR(0x0049D4C0), HandleBase* This);
		};
	}
}