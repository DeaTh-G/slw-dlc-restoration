#pragma once

namespace app
{
	namespace fnd
	{
		class CActor : public csl::ut::NonCopyable
		{
		public:
			int field_00[7];

			inline static FUNCTION_PTR(char, __thiscall, SendMessageImm, ASLR(0x0049A470), int* This, int a2, void* msg);
		};
	}
}
