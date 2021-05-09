#pragma once

namespace csl
{
    namespace math
    {
        class alignas(16) Matrix34
        {
        public:
            float data[4][4];
        };

        inline static FUNCTION_PTR(float, __cdecl, Matrix34Inverse, ASLR(0x00961240), Matrix34* a1, Matrix34* a2);
    }
}