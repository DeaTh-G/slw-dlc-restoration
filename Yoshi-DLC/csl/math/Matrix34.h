#pragma once

namespace csl
{
    namespace math
    {
        class alignas(16) Matrix34
        {
        public:
            float data[4][4];

            csl::math::Vector3 GetColumn(int column)
            {
                return Vector3(data[column][0], data[column][1], data[column][2]);
            }

            void SetColumn(int column, csl::math::Vector3& vector)
            {
                *(csl::math::Vector3*)data[column] = vector;
            }

            csl::math::Vector3 GetTransVector()
            {
                return Vector3(data[3][0], data[3][1], data[3][2]);
            }
        };

        inline static FUNCTION_PTR(float, __cdecl, Matrix34Inverse, ASLR(0x00961240), Matrix34* a1, Matrix34* a2);
    }
}