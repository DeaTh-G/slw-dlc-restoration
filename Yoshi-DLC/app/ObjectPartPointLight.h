#pragma once

namespace app
{
    class ObjectPartPointLight : public GameObject3D
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
            bool field_2C;
            INSERT_PADDING(2);

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

    protected:
        CInfo* pInfo;
        float field_324;
        short field_328;
        char field_32C;
        INSERT_PADDING(5);

    private:
        inline static FUNCTION_PTR(void, __thiscall, f_ChangeColor, ASLR(0x00740A80), ObjectPartPointLight* This, csl::ut::ColorF color);

    public:
        void ChangeColor(csl::ut::ColorF color) { f_ChangeColor(this, color); }

        void ChangeRadius(float radius)
        {
            ((gfx::RenderManager*)ASLR(0x00FD3CC4))->SetLocalLightRadius(this->field_328, 0, 0, 0, radius);
        }
    };
}