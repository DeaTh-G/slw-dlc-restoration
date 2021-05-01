#pragma once

namespace app
{
    namespace xgame
    {
        class MsgLoftBirdCollision : public fnd::Message
        {
        public:
            int field_18{};
            int field_1C{};

            MsgLoftBirdCollision() : Message()
            {
                Type = fnd::PROC_MSG_LOFT_BIRD_COLLISION;
            }

            MsgLoftBirdCollision(int a1) : MsgLoftBirdCollision()
            {
                if (a1 < 4)
                    field_18 = a1;
            }
        };
    }

}