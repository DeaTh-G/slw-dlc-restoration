#pragma once

namespace app
{
	namespace Render
	{
		class CameraParam
		{
		private:
			inline static FUNCTION_PTR(csl::math::Matrix34*, __thiscall, f_GetInvViewMatrix, ASLR(0x008FF710), CameraParam* This, csl::math::Matrix34* matrix);
			inline static FUNCTION_PTR(bool, __thiscall, f_TransformNDC, ASLR(0x008FF660), CameraParam* This, csl::math::Vector3* a1, csl::math::Vector3* a2);

		public:
			csl::math::Matrix34* GetInvViewMatrix(csl::math::Matrix34* matrix) { return f_GetInvViewMatrix(this, matrix); }
			bool TransformNDC(csl::math::Vector3* a1, csl::math::Vector3* a2) { return f_TransformNDC(this, a1, a2); }
		};
	}
}