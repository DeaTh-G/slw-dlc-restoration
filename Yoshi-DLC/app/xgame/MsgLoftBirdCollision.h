#pragma once

namespace app
{
    enum class ObjLoftBirdCollisionType : char
    {
        FLAP_WING,
        LOFTBIRD_CRY,
        EVENT_ONE,
        EVENT_TWO
    };

    namespace xgame
    {
        class MsgLoftBirdCollision : public fnd::Message
        {
        public:
            ObjLoftBirdCollisionType EventType{};
            int field_1C{};

            MsgLoftBirdCollision() : Message()
            {
                Type = fnd::PROC_MSG_LOFT_BIRD_COLLISION;
            }

            MsgLoftBirdCollision(ObjLoftBirdCollisionType eventType) : MsgLoftBirdCollision()
            {
                if (eventType <= ObjLoftBirdCollisionType::EVENT_TWO)
                    EventType = eventType;
            }
        };
    }
}