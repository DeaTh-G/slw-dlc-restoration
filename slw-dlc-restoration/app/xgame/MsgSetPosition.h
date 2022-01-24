#pragma once

namespace app
{
    namespace xgame
    {
        __declspec(align(16)) struct MsgSetPosition
        {
        public:
            fnd::Message Base;
            int field_18;
            int field_1C;
            csl::math::Vector3 Position;
        };
    }

}