#pragma once

namespace csl
{
    namespace math
    {
        struct alignas(16) Vector3
        {
            float X;
            float Y;
            float Z;

            Vector3()
            {
                X = 0;
                Y = 0;
                Z = 0;
            }

            Vector3(float x, float y, float z)
            {
                X = x;
                Y = y;
                Z = z;
            }

            csl::math::Vector3& operator+=(const csl::math::Vector3& other)
            {
                this->X = this->X + other.X;
                this->Y = this->Y + other.Y;
                this->Z = this->Z + other.Z;
            }
        };

        inline static FUNCTION_PTR(float, __cdecl, Vector3Length, ASLR(0x00962510), Vector3* a1);
        inline static FUNCTION_PTR(float, __cdecl, Vector3Normalize, ASLR(0x009626B0), Vector3* a1, Vector3* a2);
    }
}