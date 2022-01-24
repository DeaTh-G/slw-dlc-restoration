#pragma once

namespace app
{
    namespace game
    {
        class GOCMovement
        {
        public:
            struct ContextParam
            {
                csl::math::Vector3 Position;
                csl::math::Quaternion Rotation;
                csl::math::Vector3 field_20;
            };

        public:
            inline static FUNCTION_PTR(int, __thiscall, SetupController, ASLR(0x004C3C70), int* This, void* a2);
            inline static FUNCTION_PTR(int, __thiscall, DisableMovementFlag, ASLR(0x004C3800), int* This, bool isDisabled);
            inline static FUNCTION_PTR(int, __thiscall, EnableMovementFlag, ASLR(0x004C37E0), int* This, bool isEnabled);
            inline static FUNCTION_PTR(int*, __thiscall, GetContextParam, ASLR(0x00934460), int* This);
            inline static FUNCTION_PTR(int*, __thiscall, GetMoveController, ASLR(0x004C3850), int* This, int a2);
        };
    }
}