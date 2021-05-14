#pragma once

namespace app
{
    class ObjectPartPointLight
    {
    public:
        class CInfo
        {
        private:
            inline static FUNCTION_PTR(CInfo*, __thiscall, __ct, ASLR(0x007409D0), CInfo* This);

        public:
            csl::math::Vector3 field_00{};
            float field_10;
            float field_14;
            float field_18;
            float field_1C;
            float field_20;
            float field_24;
            int* field_28;
            int field_2C;

            CInfo()
            {
                __ct(this);
            }

            CInfo(csl::math::Vector3 a1, float a2, float a3, float a4, float a5, float a6, float a7, int* a8, int a9) : CInfo()
            {
                field_00 = a1;
                field_10 = a2;
                field_14 = a3;
                field_18 = a4;
                field_1C = a5;
                field_20 = a6;
                field_24 = a7;
                field_28 = a8;
                field_2C = a9;
            }
        };

        inline static FUNCTION_PTR(ObjectPartPointLight*, __cdecl, Create, ASLR(0x00740A10), GameDocument* document, CInfo* info);
    };
}