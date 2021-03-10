using csl::math::Vector3;

namespace app
{
	namespace math
	{
		inline static FUNCTION_PTR(bool, __cdecl, Vector3NormalizeIfNotZero, ASLR(0x00417C40), Vector3* a1, Vector3* a2);
		static void Vector3Inverse(Vector3* a1)
		{
			a1->X *= -1;
			a1->Y *= -1;
			a1->Z *= -1;
		}
		
		static float Vector3DotProduct(Vector3* a1, Vector3* a2)
		{
			return a1->X * a2->X +
				   a1->Y * a2->Y +
				   a1->Z * a2->Z;
		}

		static void Vector3CrossProduct(Vector3* a1, Vector3* a2, Vector3* a3)
		{
			a3->X = a1->Y * a2->Z - a1->Z * a2->Y;
			a3->Y = a1->Z * a2->X - a1->X * a2->Z;
			a3->Z = a1->X * a2->Y - a1->Y * a2->X;
		}

		static void Clamp(float* result, float a2, float a3)
		{
			if (*result < a2)
				*result = a2;
			if (*result > a3)
				*result = a3;
		}

		static void Lerp(float* result, float* a2, float* a3)
		{
			*result = (*a3 * (*a2 - *result)) + *result;
		}
	}
}