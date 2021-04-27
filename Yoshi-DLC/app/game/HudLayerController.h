#pragma once

namespace app
{
    namespace game
    {
        class HudLayerController
        {
        public:
            struct alignas(4) PlayInfo
            {
                inline static FUNCTION_PTR(void, __thiscall, __ct, ASLR(0x004C0D60), PlayInfo* This);

                const char* AnimationName;
                int field_04;
                int field_08;
                float field_0C;
                bool field_10;

                PlayInfo()
                {
                    __ct(this);
                }
            };

        private:
            inline static FUNCTION_PTR(void, __thiscall, f_PlayAnimation, ASLR(0x004C0F50), HudLayerController* This, const char* animationName, int a2, int a3);
            inline static FUNCTION_PTR(void, __thiscall, f_PlayInfoAnimation, ASLR(0x004C0DB0), HudLayerController* This, const PlayInfo& playInfo);
            inline static FUNCTION_PTR(void, __thiscall, f_ReserveAnimation, ASLR(0x004C10B0), HudLayerController* This, const char* animationName, int a2, int a3);
            inline static FUNCTION_PTR(void, __thiscall, f_PlayReservedAnimation, ASLR(0x004C0ED0), HudLayerController* This);
            inline static FUNCTION_PTR(bool, __thiscall, f_IsEndReservedAnimation, ASLR(0x004C0D90), HudLayerController* This);
            inline static FUNCTION_PTR(bool, __thiscall, f_IsCurrentAnimation, ASLR(0x004C0E30), HudLayerController* This, const char* animationName);
            inline static FUNCTION_PTR(void, __thiscall, f_SetVisible, ASLR(0x004C0E60), HudLayerController* This, bool isVisible);

        public:
            void ReserveAnimation(const char* animationName, int a2, int a3)
            {
                f_ReserveAnimation(this, animationName, a2, a3);
            }

            void PlayReservedAnimation()
            {
                f_PlayReservedAnimation(this);
            }

            void PlayAnimation(const char* animationName, int a2, int a3)
            {
                f_PlayAnimation(this, animationName, a2, a3);
            }

            void PlayAnimation(const PlayInfo& playInfo)
            {
                f_PlayInfoAnimation(this, playInfo);
            }

            void SetVisible(bool isVisible)
            {
                f_SetVisible(this, isVisible);
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