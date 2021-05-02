#pragma once

namespace app
{
    namespace game
    {
        class PathEvaluator
        {
        public:
            int field_00;
            int field_04;
            float field_08;
            int field_0C;

            inline static FUNCTION_PTR(void, __thiscall, __ct, ASLR(0x004D8D90), PathEvaluator* This);
            inline static FUNCTION_PTR(float, __thiscall, GetLength, ASLR(0x004D8E30), PathEvaluator* This);
            inline static FUNCTION_PTR(csl::math::Vector3*, __thiscall, GetClosestPositionAlongSpline, ASLR(0x004D8EE0), PathEvaluator* This, csl::math::Vector3* a1, csl::math::Vector3* a2, float a3, float* a4);
            inline static FUNCTION_PTR(int*, __thiscall, SetDistance, ASLR(0x004D8CC0), PathEvaluator* This, float a2);
            inline static FUNCTION_PTR(int*, __thiscall, SetPathObject, ASLR(0x004D8DC0), PathEvaluator* This, int* a2);
            inline static FUNCTION_PTR(int*, __thiscall, GetPNT, ASLR(0x004D8E50), PathEvaluator* This, float a2, csl::math::Vector3* a3, csl::math::Vector3* a4, csl::math::Vector3* a5);
        };
    }
}