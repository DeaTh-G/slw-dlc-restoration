#pragma once

namespace app
{
    namespace xgame
    {
        class IsDLCStagePurchase
        {
        public:
            static void Func();
        };

        inline static FUNCTION_PTR(const char*, __cdecl, ConvertPackNameForDLC, ASLR(0x0095DB40), const char* a1, const char* a2, const char* a3);
    };
}