#pragma once

namespace app
{
    namespace xgame
    {
        class MsgHitTriggerBase : public fnd::Message
        {
        private:
            inline static FUNCTION_PTR(csl::math::Vector3*, __thiscall, f_GetContactPointNormal, ASLR(0x00534520), MsgHitTriggerBase* This, csl::math::Vector3* result);

        public:
            int field_18;
            int* field_1C;
            int ActorID{};

            csl::math::Vector3* GetContactPointNormal(csl::math::Vector3* result) { return f_GetContactPointNormal(this, result); }
        };
    }
}