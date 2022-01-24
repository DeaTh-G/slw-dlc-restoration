#pragma once

namespace app
{
    namespace game
    {
        class GOCShadow
        {
        public:
            inline static FUNCTION_PTR(int, __thiscall, Setup, ASLR(0x004CBAE0), int* This, int* shadowCount);
            inline static FUNCTION_PTR(void*, __thiscall, CreateShape, ASLR(0x004CBB60), int* This, void* shadowShapeCInfo);
            inline static FUNCTION_PTR(void, __thiscall, SetVisible, ASLR(0x004CB9E0), int* This, bool isEnable);
        };
    }
}