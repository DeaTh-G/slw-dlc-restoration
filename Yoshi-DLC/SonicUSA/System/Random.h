#pragma once

namespace SonicUSA
{
	namespace System
	{
		class Random
		{
		public:
			inline static FUNCTION_PTR(int, __thiscall, genrand_int32, ASLR(0x0097B1D0), int* This);
		};
	}
}