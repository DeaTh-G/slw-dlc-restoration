#pragma once

namespace SurfRide
{
    class Layer
    {
    public:
        inline static FUNCTION_PTR(int, __cdecl, SetCastPatternIndex, ASLR(0x00525800), int* This, char* TxtBuf, int Bit);
        inline static FUNCTION_PTR(void, __thiscall, SetHideFlag, ASLR(0x00C61D20), int* This, bool isHidden);
    };
}