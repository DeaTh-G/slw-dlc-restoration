#pragma once

namespace app
{
    namespace xgame
    {
        __declspec(align(16)) struct MsgResumeGameTimer
        {
        public:
            fnd::Message Base;
        };
    }

}