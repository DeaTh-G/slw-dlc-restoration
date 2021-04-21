#pragma once

namespace app
{
    namespace game
    {
        class HudLayerController
        {
        private:
            inline static FUNCTION_PTR(void, __thiscall, f_ReserveAnimation, ASLR(0x004C10B0), HudLayerController* This, const char* animationName, int a2, int a3);
            inline static FUNCTION_PTR(void, __thiscall, f_PlayReservedAnimation, ASLR(0x004C0ED0), HudLayerController* This);
            inline static FUNCTION_PTR(bool, __thiscall, f_IsEndReservedAnimation, ASLR(0x004C0D90), HudLayerController* This);
            inline static FUNCTION_PTR(bool, __thiscall, f_IsCurrentAnimation, ASLR(0x004C0E30), HudLayerController* This, const char* animationName);

        public:
            void ReserveAnimation(const char* animationName, int a2, int a3)
            {
                f_ReserveAnimation(this, animationName, a2, a3);
            }

            void PlayReservedAnimation()
            {
                f_PlayReservedAnimation(this);
            }

            bool IsEndReservedAnimation()
            {
                return f_IsEndReservedAnimation(this);
            }

            bool IsCurrentAnimation(const char* animationName)
            {
                return f_IsCurrentAnimation(this, animationName);
            }
        };
    }
}