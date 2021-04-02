#pragma once

namespace app
{
    namespace game
    {
        class ShadowShape
        {
        public:
            inline static FUNCTION_PTR(int, __thiscall, SetMaxDownDist, ASLR(0x004CC030), void* This, float distance);
            inline static FUNCTION_PTR(int, __thiscall, SetLocalOffsetPosition, ASLR(0x004CC1E0), void* This, csl::math::Vector3* position);
        };
    }
}