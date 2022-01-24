#pragma once

namespace app
{
    namespace game
    {
        class GOCPhysics
        {
        public:
            inline static FUNCTION_PTR(int, __thiscall, Setup, ASLR(0x004B68D0), int* This, int* unit);
            inline static FUNCTION_PTR(void, __thiscall, CreateShape, ASLR(0x004CB710), int* This, CollisionObjCInfo* Collision);
        };
    }
}