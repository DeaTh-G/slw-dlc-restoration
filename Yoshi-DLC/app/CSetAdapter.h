#pragma once
#include "../framework.h"
#include "../app/GameObject.h"
#include "../LWTypes.h"

namespace app
{
	class CSetAdapter
	{
	public:
		inline static FUNCTION_PTR(int*, __thiscall, GetData, ASLR(0x008408F0), int* This);
		inline static FUNCTION_PTR(int*, __thiscall, GetRotation, ASLR(0x008407F0), int* This, Quaternion* rotation);
	};
}