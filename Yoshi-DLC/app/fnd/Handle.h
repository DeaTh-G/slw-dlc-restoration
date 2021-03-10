#pragma once

namespace app
{
	namespace fnd
	{
		struct HandleBase
		{
			int field_00;
			int field_04;
		};

		class Handle
		{
		public:
			inline static FUNCTION_PTR(int*, __thiscall, Get, ASLR(0x0049D4C0), HandleBase* This);

		};
	}
}