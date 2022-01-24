#pragma once

namespace csl
{
    namespace math
    {
        class alignas(16) Quaternion
        {
        private:
            inline static FUNCTION_PTR(Quaternion*, __thiscall, __ct, ASLR(0x00964060), Quaternion* result, Vector3* direction, float angle);

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

            Quaternion(Quaternion* result, Vector3* direction, float angle)
            {
                __ct(result, direction, angle);
            }
        };

        inline static FUNCTION_PTR(Quaternion*, __cdecl, QuaternionNormalize, ASLR(0x00963EB0), Quaternion* result, Quaternion* base);
        inline static FUNCTION_PTR(Quaternion*, __cdecl, QuaternionRotationBetweenNormals, ASLR(0x00963DA0), Quaternion* result, Vector3* base, Vector3* a3);
    }
}