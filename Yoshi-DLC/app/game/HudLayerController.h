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
                int AnimationID;
                bool IsLooping;
                INSERT_PADDING(3);
                float field_0C;
                bool field_10;

                PlayInfo()
                {
                    __ct(this);
                }
            };

        private:
            inline static FUNCTION_PTR(void, __thiscall, f_PlayAnimation, ASLR(0x004C0F50), HudLayerController* This, const char* animationName, bool isLooping, int a3);
            inline static FUNCTION_PTR(void, __thiscall, f_PlayInfoAnimation, ASLR(0x004C0DB0), HudLayerController* This, const PlayInfo& playInfo);
            inline static FUNCTION_PTR(void, __thiscall, f_ReserveAnimation, ASLR(0x004C10B0), HudLayerController* This, const char* animationName, int a2, int a3);
            inline static FUNCTION_PTR(void, __thiscall, f_PlayReservedAnimation, ASLR(0x004C0ED0), HudLayerController* This);
            inline static FUNCTION_PTR(bool, __thiscall, f_IsEndReservedAnimation, ASLR(0x004C0D90), HudLayerController* This);
            inline static FUNCTION_PTR(bool, __thiscall, f_IsCurrentAnimation, ASLR(0x004C0E30), HudLayerController* This, const char* animationName);
            inline static FUNCTION_PTR(void, __thiscall, f_SetVisible, ASLR(0x004C0E60), HudLayerController* This, bool isVisible);
            inline static FUNCTION_PTR(char, __thiscall, f_GetFrame, ASLR(0x004C0E20), HudLayerController* This);

        public:
            void ReserveAnimation(const char* animationName, int a2, int a3)
            {
                f_ReserveAnimation(this, animationName, a2, a3);
            }

            void PlayReservedAnimation()
            {
                f_PlayReservedAnimation(this);
            }

            void PlayAnimation(const char* animationName, bool isLooping, int a3)
            {
                f_PlayAnimation(this, animationName, isLooping, a3);
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

            char GetFrame()
            {
                return f_GetFrame(this);
            }
        };
    }
}