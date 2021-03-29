#pragma once

namespace app
{
    namespace fnd
    {
        class GOCVisualContainer
        {
        public:
            inline static FUNCTION_PTR(void, __thiscall, Setup, ASLR(0x00494DD0), int* visualContainer, int* arraySize);
            inline static FUNCTION_PTR(void, __thiscall, Add, ASLR(0x00494D70), int* visualContainer, int* gocVisual);
        };
    }
}