#pragma once

namespace app
{
	namespace game
	{
		class GOCHud
		{
		public:
			struct alignas(4) Description
			{
				const char* field_00;
				int field_04;
				int field_08;
				uint8_t field_09;
				uint8_t field_0A;
				uint8_t field_0B;
				uint8_t field_0D;
				int field_0C;
				int field_10;
				int field_14;
			};

			inline static FUNCTION_PTR(void, __thiscall, Setup, ASLR(0x004BFF90), GOCHud* This, Description* hudDescriptor);
			inline static FUNCTION_PTR(int* , __thiscall, SetupProject, ASLR(0x004BFA20), GOCHud* This, int* a2, const char* a3, int* packFile);
		};
	}
}