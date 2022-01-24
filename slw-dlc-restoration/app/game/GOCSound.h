#pragma once

namespace app
{
    namespace game
    {
        struct SoundFollowFrameInfo
        {
            const char* Name{};
            int field_04{};
            float field_08{};
            int field_10{};
            int field_14{};
            int field_18{};
            int field_1C{};
            bool field_20{};
            INSERT_PADDING(3);
            int field_24{};
            int field_28{};
            int field_2C{};
            int field_30{};
            int field_34{};
            int field_38{};
            int field_3C{};
            csl::math::Vector3 field_40{};

            SoundFollowFrameInfo(const char* pName)
            {
                field_04 = 0x80000000;
                field_24 = 1;
                Name = pName;
            }
        };

        class GOCSound
        {
        public:
            inline static FUNCTION_PTR(void, __cdecl, SimpleSetup, ASLR(0x004CCE30), GameObject* Object, int a2, int a3);
            inline static FUNCTION_PTR(void, __thiscall, Play, ASLR(0x004CD180), int* This, int* DeviceTag, const char* CueName, int a4);
            inline static FUNCTION_PTR(void, __thiscall, Play3D, ASLR(0x004CD060), int* This, int* DeviceTag, const char* CueName, int a4);
            inline static FUNCTION_PTR(void, __thiscall, Play3DFollow, ASLR(0x004CD300), int* This, int* DeviceTag, SoundFollowFrameInfo* info);
        };
    }
}