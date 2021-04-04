#pragma once

namespace app
{
    namespace game
    {
        class alignas(16) MoveBound : public MoveController
        {
        public:
            struct Description
            {
                csl::math::Vector3 field_00;
                float field_10;
                float field_14;
                float field_18;
                float field_1C;
                float field_20;
                float field_24;
                int field_28;
                float field_2C;
                int field_30;
                int field_34;
                PathEvaluator field_38;
                short field_48;
                short field_4A;
                int field_4C;
            };

            int field_10;
            int field_14;
            int field_18;
            int field_1C;
            int field_20;
            int field_24;
            int field_28;
            int field_2C;
            Description Descriptor;
            int field_80;
            int field_84;
            int field_88;
            int field_8C;

            class Listener
            {
            protected:
                virtual void OnBound(csl::math::Plane& const a1) {};
            };

        private:
            inline static FUNCTION_PTR(int, __thiscall, f_Update, ASLR(0x004C5E40), MoveController* This, const fnd::SUpdateInfo& updateInfo);
            inline static FUNCTION_PTR(int, __thiscall, f_OnEnter, ASLR(0x004C5E20), MoveController* This);
            inline static FUNCTION_PTR(MoveBound*, __thiscall, __dt, ASLR(0x004C5400), MoveBound* This, int a2);
            inline static FUNCTION_PTR(MoveBound*, __thiscall, __ct, ASLR(0x004C5860), MoveBound* This);
            inline static FUNCTION_PTR(void, __thiscall, f_ResetListener, ASLR(0x004C53A0), MoveBound* This, Listener* a2);

        protected:
            void* GetRuntimeTypeInfo() override { return (void*)ASLR(0x00F6BDAC); }

        public:
            inline static FUNCTION_PTR(void, __thiscall, Setup, ASLR(0x004C5930), MoveBound* This, Description* a2);

            int Update(const fnd::SUpdateInfo& updateInfo) override
            {
                return f_Update(this, updateInfo);
            }
            int OnEnter() override { return f_OnEnter(this); }
            ~MoveBound() override
            {
                __dt(this, 0);
            }

            MoveBound()
            {
                __ct(this);
            }

            void ResetListener(Listener* a2)
            {
                f_ResetListener(this, a2);
            }
        };
    }
}