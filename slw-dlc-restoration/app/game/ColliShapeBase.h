#pragma once

namespace app
{
	namespace game
	{
		class ColliShapeBase
		{
		public:
			inline static FUNCTION_PTR(bool, __thiscall, SetLocalRotation, ASLR(0x004BA300), void* This, csl::math::Quaternion* rotation);
			inline static FUNCTION_PTR(GameObject*, __thiscall, GetGameObject, ASLR(0x004B9EB0), void* This);
		};
	}
}