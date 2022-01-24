#pragma once

namespace app
{
    namespace fnd
    {
        class GOCVisual
        {
        public:
            inline static FUNCTION_PTR(bool, __thiscall, IsVisible, ASLR(0x00494C30), int* This);
            inline static FUNCTION_PTR(void, __thiscall, SetVisible, ASLR(0x00494C40), int* This, bool isVisible);
        };
    }
}