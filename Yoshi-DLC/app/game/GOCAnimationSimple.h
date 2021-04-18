#pragma once

namespace app
{
    namespace game
    {
        class GOCAnimationSimple
        {
        public:
            inline static FUNCTION_PTR(int, __thiscall, Setup, ASLR(0x004B3C80), int* This, int* unit);
            inline static FUNCTION_PTR(void, __thiscall, Add, ASLR(0x004B3E50), int* This, const char* AnimationType, int resAnimSkeleton, int playPolicy);
            inline static FUNCTION_PTR(void, __thiscall, AddListener, ASLR(0x004B3FA0), int* This, animation::AnimationListener* listener);
            inline static FUNCTION_PTR(void, __thiscall, SetAnimation, ASLR(0x004B3A80), int* This, const char* AnimationType);
            inline static FUNCTION_PTR(bool, __thiscall, IsFinished, ASLR(0x004B38E0), int* This);
        };
    }
}