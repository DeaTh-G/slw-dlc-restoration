#pragma once

namespace app
{
    class GOCEnemyTarget
    {
    public:
        inline static FUNCTION_PTR(void, __thiscall, Setup, ASLR(0x00726510), int* This, int* a2);
        inline static FUNCTION_PTR(void, __thiscall, SetColliderEyesight, ASLR(0x00726BC0), int* This, int a2);
        inline static FUNCTION_PTR(void, __thiscall, SetSphereEyesight, ASLR(0x00726AA0), int* This, float a2);
        inline static FUNCTION_PTR(void, __thiscall, SetEyesightOffset, ASLR(0x00726580), int* This, csl::math::Vector3* offset);
        inline static FUNCTION_PTR(bool, __thiscall, IsFindTarget, ASLR(0x00726600), int* This);
        inline static FUNCTION_PTR(bool, __thiscall, LockTarget, ASLR(0x007266B0), int* This);
        inline static FUNCTION_PTR(bool, __thiscall, GetTargetCenterPosition, ASLR(0x00726610), int* This, csl::math::Vector3* targetPos);
        inline static FUNCTION_PTR(bool, __thiscall, SetEnableEyesight, ASLR(0x007265C0), int* This, bool isEnable);
        inline static FUNCTION_PTR(bool, __thiscall, SetEyesightFrame, ASLR(0x00726310), int* This, fnd::HFrame* frame);
    };
}