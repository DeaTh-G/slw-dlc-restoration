#pragma once

namespace app
{
    namespace xgame
    {
        __declspec(align(16)) struct MsgCatchEndPlayer
        {
        public:
            fnd::Message Base;
            bool field_18;
            int8_t field_19;
            int8_t field_20;
            int8_t field_21;
        };
    }

}