#pragma once

namespace app
{
	namespace game
	{
		struct alignas(16) EffectCreateInfo
		{
			const char* Name;
			float					field_04;
			int						field_08;
			int						Padding;
			csl::math::Vector3		Position;
			csl::math::Quaternion	Rotation;
			bool					field_30;
			int						field_34;
			int						field_38;
			int						field_3C;
			int						field_40;
			int						field_44;
			int						field_48;
			int						field_4C;

		public:
			static EffectCreateInfo* __ct(EffectCreateInfo* This)
			{
				This->Name = "";
				This->field_04 = 0;
				This->field_08 = 0;
				This->Padding = 0;
				This->field_30 = 0;
				This->field_34 = 0;
				This->field_38 = 0;
				This->field_3C = 0;
				This->field_40 = 0;
				This->field_44 = 0;
				This->field_48 = 0;
				This->field_4C = 0;
				return This;
			}
		};

		class GOCEffect
		{
		public:
			inline static FUNCTION_PTR(void, __cdecl, SimpleSetup, ASLR(0x004BD980), GameObject* Object);
			inline static FUNCTION_PTR(void, __thiscall, CreateEffect, ASLR(0x004BD5F0), int* This, const char* EffectName);
			inline static FUNCTION_PTR(void, __thiscall, CreateEffectEx, ASLR(0x004BDA20), int* This, EffectCreateInfo* EffectName);
		};
	}
}