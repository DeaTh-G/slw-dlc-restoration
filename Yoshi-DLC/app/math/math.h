using csl::math::Vector3;
using csl::math::Quaternion;
using csl::math::Matrix34;

namespace app
{
	namespace math
	{
		inline static FUNCTION_PTR(bool, __cdecl, Vector3NormalizeIfNotZero, ASLR(0x00417C40), Vector3* a1, Vector3* a2);
		inline static FUNCTION_PTR(Quaternion*, __cdecl, Vector3Rotate, ASLR(0x00533000), Vector3* a1, Quaternion* a2, Vector3* a3);
		inline static FUNCTION_PTR(Matrix34*, __cdecl, Matrix34AffineTransformation, ASLR(0x005325D0), Matrix34* a1, Vector3* a2, Quaternion* a3);

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

		static void Vector3Scale(Vector3* srcVec, float scalar, Vector3* dstVec)
		{
			dstVec->X = srcVec->X * scalar;
			dstVec->Y = srcVec->Y * scalar;
			dstVec->Z = srcVec->Z * scalar;
		}

		static void Vector3Add(Vector3* a, Vector3* b, Vector3* abVec)
		{
			abVec->X = a->X + b->X;
			abVec->Y = a->Y + b->Y;
			abVec->Z = a->Z + b->Z;
		}

		static void Vector3Subtract(Vector3* a, Vector3* b, Vector3* a_bVec)
		{
			a_bVec->X = a->X - b->X;
			a_bVec->Y = a->Y - b->Y;
			a_bVec->Z = a->Z - b->Z;
		}

		static float Clamp(float result, float min, float max)
		{
			if (result < min)
				result = min;
			if (result > max)
				result = max;

			return result;
		}
	}
}