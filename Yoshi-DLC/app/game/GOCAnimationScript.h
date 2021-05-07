#pragma once

namespace app
{
    namespace game
    {
        class GOCAnimationScript
        {
        public:
            inline static FUNCTION_PTR(int, __thiscall, Setup, ASLR(0x004B35B0), int* This, animation::AnimationResContainer** a2);
            inline static FUNCTION_PTR(float, __thiscall, GetFrame, ASLR(0x004B3360), int* This);
            inline static FUNCTION_PTR(int, __thiscall, SetAnimation, ASLR(0x004B36E0), int* This, const char* animationName);
            inline static FUNCTION_PTR(int, __thiscall, IsCurrentAnimation, ASLR(0x004B3340), int* This, const char* animationName);
            inline static FUNCTION_PTR(int, __thiscall, IsFinished, ASLR(0x004B3330), int* This);
            inline static FUNCTION_PTR(float, __thiscall, ChangeAnimation, ASLR(0x004B36F0), int* This, const char* animationName);
            inline static FUNCTION_PTR(int*, __thiscall, RegisterCallback, ASLR(0x004B3400), int* This, int a2, void* a3);
            inline static FUNCTION_PTR(int*, __thiscall, ExitLoopSeqInsideAnimation, ASLR(0x004B35A0), int* This);
        };
    }
}