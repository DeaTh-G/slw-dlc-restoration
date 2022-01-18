#pragma once

namespace app
{
    namespace xgame
    {
        class MsgAppeareKnockback : public fnd::Message
        {
        public:
            int field_18{};
            int field_1C{};
            csl::math::Vector3 field_20{};
            int field_30{};
            int field_34{};
            int field_38{};
            int field_3C{};

            MsgAppeareKnockback() : Message()
            {
                Type = fnd::PROC_MSG_APPEARE_KNOCKBACK;
            }

            MsgAppeareKnockback(csl::math::Vector3 a1) : MsgAppeareKnockback()
            {
                field_20 = a1;
            }
        };
    }
}