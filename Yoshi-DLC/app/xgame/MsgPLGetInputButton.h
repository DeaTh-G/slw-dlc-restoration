#pragma once

namespace app
{
    namespace xgame
    {
        struct alignas(16) MsgPLGetInputButton
        {
        public:
            fnd::Message Base;
            int field_18;
            int field_1C;
            bool field_20;

            inline static FUNCTION_PTR(MsgPLGetInputButton*, __thiscall, __ct, ASLR(0x00537630), MsgPLGetInputButton* This, int a2, int a3);
        };
    }

}