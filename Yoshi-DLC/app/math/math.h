using csl::math::Vector3;
using csl::math::Quaternion;
using csl::math::Matrix34;

namespace app
{
    namespace math
    {
        inline static FUNCTION_PTR(bool, __cdecl, Vector3NormalizeIfNotZero, ASLR(0x00417C40), Vector3* a1, Vector3* a2);
        inline static FUNCTION_PTR(bool, __cdecl, Vector3NormalizeZero, ASLR(0x0041B980), Vector3* a1, Vector3* a2);
        inline static FUNCTION_PTR(float, __cdecl, Vector3NormalizeWithLength, ASLR(0x00423B30), Vector3* a1, Vector3* a2);
        inline static FUNCTION_PTR(Quaternion*, __cdecl, Vector3Rotate, ASLR(0x00533000), Vector3* a1, Quaternion* a2, Vector3* a3);
        inline static FUNCTION_PTR(Vector3*, __cdecl, Vector3RotateX, ASLR(0x00532E50), Vector3* a1, float a2, Vector3* a3);
        inline static FUNCTION_PTR(Vector3*, __cdecl, Vector3RotateY, ASLR(0x00532EE0), Vector3* a1, float a2, Vector3* a3);
        inline static FUNCTION_PTR(Matrix34*, __cdecl, Matrix34AffineTransformation, ASLR(0x005325D0), Matrix34* a1, Vector3* a2, Quaternion* a3);
        inline static FUNCTION_PTR(float, __cdecl, Matrix34OrthonormalDirection, ASLR(0x00532710), Matrix34* a1, Vector3* a2, Vector3* a3);

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

        static float Vector3Magnitude(Vector3* vector)
        {
            return sqrt(pow(vector->X, 2) + pow(vector->Y, 2) + pow(vector->Z, 2));
        }

        static void Vector3SquareMagnitude(Vector3* a, float* length)
        {
            *length = pow(a->X, 2) + pow(a->Y, 2) + pow(a->Z, 2);
        }

        static void Vector3Multiply(Vector3* a, Vector3* b, Vector3* ab)
        {
            *ab = Vector3(a->X * b->X, a->Y * b->Y, a->Z * b->Z);
        }

        static void MatrixConcat(Matrix34* a, Matrix34* b, Matrix34* ab)
        {
            for (size_t i = 0; i < 4; i++)
                for (size_t j = 0; j < 4; j++)
                    ab->data[i][j] = a->data[i][j] * b->data[i][j];
        }
    }
}