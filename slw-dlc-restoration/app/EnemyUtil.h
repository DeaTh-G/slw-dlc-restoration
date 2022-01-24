#pragma once

namespace app
{
    class EnemyUtil
    {
    public:
        inline static FUNCTION_PTR(bool, __cdecl, IsDamage, ASLR(0x0072D0A0), int a1, int a2, int a3);
        inline static FUNCTION_PTR(bool, __cdecl, IsFatalDamage, ASLR(0x0072D0C0), fnd::Message& message);
    };
}