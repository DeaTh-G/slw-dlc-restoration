#pragma once

namespace app
{
    namespace game
    {
        class GOCAnimationContainer
        {
        public:
            inline static FUNCTION_PTR(void, __thiscall, Setup, ASLR(0x004B30F0), int* animationContainer, int* arraySize);
            inline static FUNCTION_PTR(void, __thiscall, Add, ASLR(0x004B30A0), int* animationContainer, int* gocAnimation);
        };
    }
}