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
        };

        inline static FUNCTION_PTR(float, __cdecl, Vector3Length, ASLR(0x00962510), Vector3* a1);
    }
}