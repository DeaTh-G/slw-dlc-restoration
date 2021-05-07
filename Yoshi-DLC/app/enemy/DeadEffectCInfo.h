#pragma once

namespace app
{
    namespace enemy
    {
        class DeadEffectCInfo
        {
        public:
            int field_00;
            int field_04;
            int field_08;
            int field_0C;
            int field_10;
            int field_14;
            int field_18;
            int field_1C;
            csl::math::Matrix34 field_20;
            char field_60;
            char field_61;
            short field_62;
            int field_64;
            int field_68;
            int field_6C;

            inline static FUNCTION_PTR(int, __thiscall, __ct, ASLR(0x00728470), DeadEffectCInfo* This);
            inline static FUNCTION_PTR(int, __thiscall, SetMsgDamage, ASLR(0x00728400), DeadEffectCInfo* This, fnd::Message* message);
            inline static FUNCTION_PTR(int, __thiscall, SetTransform, ASLR(0x007284D0), DeadEffectCInfo* This, csl::math::Matrix34* transform);

            static char* SetYoshiIsland(DeadEffectCInfo* This)
            {
                This->field_62 &= ~(1 << 1);
                This->field_62 &= ~(1 << 2);
                This->field_62 |= 1 << 4;
                This->field_61 = 2;

                return &This->field_61;
            }

            static char* SetZeldaDekunuts(DeadEffectCInfo* This)
            {
                This->field_62 &= ~(1 << 2);
                This->field_62 |= 1 << 8;
                This->field_61 = 5;

                return &This->field_61;
            }

            char* SetZeldaKeese()
            {
                field_62 &= ~(1 << 1);
                field_62 |= 1 << 5;
                field_61 = 3;

                return &field_61;
            }
        };
    }
}