#pragma once

namespace app
{
    namespace Effect
    {
        class CEffectHandle
        {
        private:
            inline static FUNCTION_PTR(void, __thiscall, f_Stop, ASLR(0x0047EEA0), CEffectHandle* This, int a2);

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

            void Stop(int a1)
            {
                f_Stop(this, a1);
            }
        };
    }
}