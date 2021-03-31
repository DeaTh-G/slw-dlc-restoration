#pragma once

namespace app
{
    namespace fnd
    {
        struct SoundParam
        {
            float field_00;
            float field_04;
            int field_08;
            unsigned int field_0C;
            int field_10;
            int field_14;
            int field_18;
        };

        class SoundPlayerCri
        {
        public:
            inline static FUNCTION_PTR(void*, __thiscall, Play, ASLR(0x004A6A90), void* This, int* a2, int a3, const char* cueName, const SoundParam& soundParam);
        };
    }
}