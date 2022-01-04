#pragma once

namespace app
{
    namespace fnd
    {
        class GOCTransform
        {
        private:
            inline static FUNCTION_PTR(void, __thiscall, __dt, ASLR(0x00494B90), GOCTransform* This, size_t deletingFlags);
            inline static FUNCTION_PTR(void, __thiscall, f_SetLocalTranslationAndRotation, ASLR(0x004944A0), GOCTransform* This, csl::math::Vector3* position, csl::math::Quaternion* rotation);
            inline static FUNCTION_PTR(void, __thiscall, f_SetLocalTransform, ASLR(0x00494690), GOCTransform* This, math::Transform* transform);

        public:
            int Data[0x78]{};

            inline static FUNCTION_PTR(void, __thiscall, SetLocalTranslation, ASLR(0x00494440), int* This, csl::math::Vector3* position);
            inline static FUNCTION_PTR(void, __thiscall, SetLocalRotation, ASLR(0x00494470), int* This, csl::math::Quaternion* position);
            inline static FUNCTION_PTR(bool, __thiscall, IsExistParent, ASLR(0x00494240), GOCTransform* This);
            inline static FUNCTION_PTR(char, __thiscall, SetInheriteFlags, ASLR(0x00494250), GOCTransform* This, char a2);

            ~GOCTransform() { __dt(this, 0); }
            void SetLocalTranslationAndRotation(csl::math::Vector3* position, csl::math::Quaternion* rotation)
            {
                f_SetLocalTranslationAndRotation(this, position, rotation);
            }

            void SetLocalTransform(math::Transform* transform)
            {
                f_SetLocalTransform(this, transform);
            }
        };
    }
}