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
        };
    }
}