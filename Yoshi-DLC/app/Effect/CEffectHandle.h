#pragma once

namespace app
{
    namespace Effect
    {
        class CEffectHandle
        {
        public:
            int field_00;
            int field_04;
            void* field_08;

            CEffectHandle()
            {
                field_00 = 0;
                field_04 = 0;
                field_08 = (void*)ASLR(0x00D5D044);
            }
        };
    }
}