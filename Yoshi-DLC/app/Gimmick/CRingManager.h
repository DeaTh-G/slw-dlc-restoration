#pragma once

namespace app
{
    namespace Gimmick
    {
        class CRingManager
        {
        public:
            inline static FUNCTION_PTR(void*, __thiscall, GetService, ASLR(0x0090B2E0), GameDocument* This, void* Class);
            inline static FUNCTION_PTR(void, __thiscall, RegisterRotateRing, ASLR(0x006652B0), void* Class, GameObject* Object);
        };
    }
}