#pragma once

namespace app
{
    namespace ObjUtil
    {
        class LayoutCylinder
        {
        public:
            struct Description
            {
                fnd::HFrame* field_00;
                int* field_04;
                int field_08;

                Description(fnd::HFrame* a1, int* a2)
                {
                    field_00 = a1;
                    field_04 = a2;
                }
            };

        private:
            inline static FUNCTION_PTR(LayoutCylinder*, __thiscall, __ct, ASLR(0x0073E100), LayoutCylinder* This);
            inline static FUNCTION_PTR(void, __thiscall, f_Setup, ASLR(0x0073E410), LayoutCylinder* This, Description* description);
            inline static FUNCTION_PTR(int*, __thiscall, f_CalcTransform, ASLR(0x0073E520), LayoutCylinder* This, csl::math::Matrix34* a2, csl::math::Vector3* a3);

        public:
            csl::math::Vector3 field_00;
            csl::math::Vector3 field_10;
            csl::math::Vector3 field_20;
            float field_30;
            game::PathEvaluator PathEvaluator;
            int field_44;
            int field_48;
            int field_4C;

            LayoutCylinder()
            {
                __ct(this);
            }

            void Setup(Description* description)
            {
                f_Setup(this, description);
            }

            int* CalcTransform(csl::math::Matrix34* a2, csl::math::Vector3* a3)
            {
                return f_CalcTransform(this, a2, a3);
            }
        };
    }
}