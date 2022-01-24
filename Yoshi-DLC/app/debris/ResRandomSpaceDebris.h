#pragma once

namespace app
{
    namespace debris
    {

        class ResRandomSpaceDebris
        {
        public:
            int Models[16];
            int field_40;

            struct SDesc
            {
                const char* Name;
                int Packfile;
            };

            inline static FUNCTION_PTR(int*, __thiscall, Build, ASLR(0x008479F0), ResRandomSpaceDebris* dataPtr, SDesc* description);
        };

        struct SRandomSpaceDebrisInfo
        {
        private:
            inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x008476D0), SRandomSpaceDebrisInfo* This);
            inline static FUNCTION_PTR(void, __thiscall, f_SetModelRes, ASLR(0x008475E0), SRandomSpaceDebrisInfo* This, ResRandomSpaceDebris* debris, int a3);

        public:
            csl::math::Vector3 field_00{};
            csl::math::Quaternion field_10{};
            int field_20{};
            int field_24{};
            int field_28{};
            int field_2C{};
            int field_30{};
            int field_34{};
            int field_38{};
            int field_3C{};
            float field_40{};
            float field_44{};
            float field_48{};
            int field_4C{};
            csl::math::Vector3 field_50{};
            csl::math::Vector3 field_60{};
            float field_70{};
            float field_74{};
            int field_78{};
            int field_7C{};
            csl::math::Vector3 field_80{};
            float field_90{};
            int field_94{};
            int field_98{};
            int field_9C{};
            float field_A0{};
            float field_A4{};
            int field_A8{};
            int field_AC{};
            int field_B0{};
            int field_B4{};
            int field_B8{};
            int field_BC{};
            int field_C0{};
            float field_C4{};
            float field_C8{};
            float field_CC{};
            int field_D0{};
            int field_D4{};
            int field_D8{};
            int field_DC{};
            int field_E0{};
            int field_E4{};
            int field_E8{};
            int field_EC{};
            int field_F0{};
            int field_F4{};
            int field_F8{};
            int field_FC{};
            int field_100{};
            int field_104{};
            int field_108{};
            int field_10C{};
            int field_110{};
            int field_114{};
            int field_118{};
            int field_11C{};

            SRandomSpaceDebrisInfo()
            {
                __ct(this);
            }

            void SetModelRes(ResRandomSpaceDebris* debris, int a3)
            {
                f_SetModelRes(this, debris, a3);
            }
        };
    }
}