#pragma once

namespace app
{
    class EnemyState
    {
    public:
        inline static FUNCTION_PTR(int, __thiscall, Enter, ASLR(0x00692130), void* This, void* a1, int a2);
        inline static FUNCTION_PTR(int, __thiscall, Leave, ASLR(0x006F8AC0), void* This, void* a1, int a2);
        inline static FUNCTION_PTR(int, __thiscall, Update, ASLR(0x005EF7A0), void* This, void* a1, float a2);
    };
}