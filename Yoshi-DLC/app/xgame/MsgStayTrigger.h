#pragma once

namespace app
{
    namespace xgame
    {
        __declspec(align(16)) struct MsgStayTrigger
        {
        public:
            fnd::Message Base;
            int field_18;
            int field_1C;
            int field_20;
        };
    }

}