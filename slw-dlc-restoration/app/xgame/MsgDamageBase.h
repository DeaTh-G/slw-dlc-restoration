#pragma once

namespace app
{
    namespace xgame
    {
        class MsgDamageBase : public fnd::Message
        {
        public:
            fnd::HandleBase field_18{};
            fnd::HandleBase field_20{};
            int field_28;
            int field_2C;
            csl::math::Vector3 field_30;
            csl::math::Vector3 field_40;
            int field_50;
            int field_54;
            int field_58;
            int field_5C;

            MsgDamageBase() : Message() {}
        };
    }
}