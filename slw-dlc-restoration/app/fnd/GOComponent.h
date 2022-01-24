#pragma once

namespace app
{
    namespace fnd
    {
        class GOComponent
        {
        public:
            inline static FUNCTION_PTR(int*, __cdecl, Create, ASLR(0x0049DBA0), GameObject* object, int* component);
            inline static FUNCTION_PTR(void, __cdecl, BeginSetup, ASLR(0x0049DCC0), GameObject* object);
            inline static FUNCTION_PTR(void, __cdecl, EndSetup, ASLR(0x0049DD00), GameObject* object);
            inline static FUNCTION_PTR(int*, __cdecl, CreateSingle, ASLR(0x0049DBD0), GameObject* object, int* component);
        };
    }
}