#pragma once

namespace app
{
    namespace game
    {
        struct ShadowShapeCInfo
        {
            int field_00;
            int field_04;
            int field_08;
            int field_0C;
            uint8_t field_10;
            uint8_t ShadowQualityType;
            short field_12;
        };

        class ShadowHemisphereShapeCInfo : public ShadowShapeCInfo
        {
        public:
            float field_14;

            inline static FUNCTION_PTR(ShadowShapeCInfo*, __thiscall, __ct, ASLR(0x004CC740), ShadowHemisphereShapeCInfo* This, float radius);
        };
        
        class ShadowSphereShapeCInfo : public ShadowShapeCInfo
        {
        public:
            float field_14;

            inline static FUNCTION_PTR(ShadowShapeCInfo*, __thiscall, __ct, ASLR(0x004CC710), ShadowSphereShapeCInfo* This, float radius);
        };

        class ShadowModelShapeCInfo : public ShadowShapeCInfo
        {
        public:
            int field_14;
            int field_18;
            int field_1C[2];
            int field_24;
            int field_28;

            inline static FUNCTION_PTR(ShadowShapeCInfo*, __thiscall, __ct, ASLR(0x004CC880), ShadowModelShapeCInfo* This, int* model);
            inline static FUNCTION_PTR(ShadowShapeCInfo*, __thiscall, __ct2, ASLR(0x004CC770), ShadowModelShapeCInfo* This, int* visual, res::ResShadowModel* model, float a4);
        };
    }
}