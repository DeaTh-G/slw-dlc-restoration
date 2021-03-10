namespace app
{
	namespace math
	{
		inline static FUNCTION_PTR(bool, __cdecl, Vector3NormalizeIfNotZero, ASLR(0x00417C40), csl::math::Vector3* a1, csl::math::Vector3* a2);
		static void Vector3Inverse(csl::math::Vector3* a1)
		{
			a1->X *= -1;
			a1->Y *= -1;
			a1->Z *= -1;
		}
		
		static float Vector3DotProduct(csl::math::Vector3* a1, csl::math::Vector3* a2)
		{
			return a1->X * a2->X +
				   a1->Y * a2->Y +
				   a1->Z * a2->Z;
		}
	}
}