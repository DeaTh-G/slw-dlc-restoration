#pragma once
#include "../framework.h"
#include "../LWTypes.h"

namespace app
{
	namespace fnd
	{
		class GOCTransform
		{
		public:
			inline static FUNCTION_PTR(void, __thiscall, SetLocalTranslation, ASLR(0x00494440), int* This, Vector3* position);
		};
	}
}