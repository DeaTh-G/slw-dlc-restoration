#pragma once
#include "../../framework.h"

namespace app
{
	namespace game
	{
		struct ShadowShapeCInfo
		{
		};

		class ShadowHemisphereShapeCInfo
		{
		public:
			int field_00;
			int field_04;
			int field_08;
			int field_0C;
			short field_10;
			short field_12;
			float field_14;

			inline static FUNCTION_PTR(ShadowShapeCInfo*, __thiscall, __ct, ASLR(0x004CC740), ShadowHemisphereShapeCInfo* This, float radius);
		};
		
		class ShadowSphereShapeCInfo
		{
		public:
			int field_00;
			int field_04;
			int field_08;
			int field_0C;
			uint8_t field_10;
			uint8_t field_11;
			short field_12;
			float field_14;

			inline static FUNCTION_PTR(ShadowShapeCInfo*, __thiscall, __ct, ASLR(0x004CC710), ShadowSphereShapeCInfo* This, float radius);
		};
	}
}