#pragma once

namespace app
{
    namespace Effect
    {
        class CEffectHandle
        {
        private:
            inline static FUNCTION_PTR(bool, __thiscall, f_IsValid, ASLR(0x0047EE50), CEffectHandle* This);
            inline static FUNCTION_PTR(void, __thiscall, f_Stop, ASLR(0x0047EEA0), CEffectHandle* This, int a2);
            inline static FUNCTION_PTR(void, __thiscall, f_SetVisible, ASLR(0x0047EF60), CEffectHandle* This, bool isVisible);

        public:
            void* field_00;
            int field_04;
            int field_08;

            CEffectHandle()
            {
                field_00 = (void*)ASLR(0x00D5D044);
                field_04 = 0;
                field_08 = 0;
            }

            bool IsValid() { return f_IsValid(this); }
            void Stop(int a1) { f_Stop(this, a1); }
            void SetVisible(bool isVisible) { f_SetVisible(this, isVisible); }
        };
    }
}