#pragma once

namespace app
{
    namespace fnd
    {
        class alignas(16) HFrame
        {
        public:
            INSERT_PADDING(0x70);
            csl::math::Matrix34 Transform;
            INSERT_PADDING(0x80);

            inline static FUNCTION_PTR(HFrame*, __thiscall, __ct, ASLR(0x0048E210), HFrame* This);
            inline static FUNCTION_PTR(HFrame*, __thiscall, __dt, ASLR(0x0048E4F0), HFrame* This, int a2);
            inline static FUNCTION_PTR(void, __thiscall, ResetFlag, ASLR(0x0048DCD0), HFrame* This, int arg2);
            inline static FUNCTION_PTR(csl::math::Matrix34*, __thiscall, SetLocalTransform, ASLR(0x0048E160), HFrame* This, csl::math::Matrix34* matrix);
            inline static FUNCTION_PTR(csl::math::Vector3*, __thiscall, SetLocalTranslation, ASLR(0x0048DD30), HFrame* This, csl::math::Vector3* position);
            inline static FUNCTION_PTR(csl::math::Quaternion*, __thiscall, SetLocalRotation, ASLR(0x0048DD80), HFrame* This, csl::math::Quaternion* rotation);
            inline static FUNCTION_PTR(void, __thiscall, AddChild, ASLR(0x0048E650), HFrame* This, HFrame* location);
        };
    }
}