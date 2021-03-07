#pragma once

namespace csl
{
	namespace math
	{
		__declspec(align(16)) class Quaternion
		{
		public:
			float X;
			float Y;
			float Z;
			float W;

			inline static FUNCTION_PTR(Quaternion*, __cdecl, QuaternionMultiply, ASLR(0x00963BE0), Quaternion* result, Quaternion* base, Quaternion* multiplier);

			Quaternion()
			{
				X = 0;
				Y = 0;
				Z = 0;
				W = 0;
			}

			Quaternion(float x, float y, float z, float w)
			{
				X = x;
				Y = y;
				Z = z;
				W = w;
			}
		};
	}
}