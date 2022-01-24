#pragma once

namespace app
{
    namespace game
    {
        class alignas(16) MovePopup : public MoveController
        {
        public:
            struct Description
            {
                float field_00{};
                float field_04{};
                float field_08{};
                int field_0C{};
            };

            csl::math::Vector3 Position{};
            csl::math::Quaternion Rotation{};
            float field_30{};
            float field_34{};
            float field_38{};
            int field_3C{};
            float field_40{};
            float field_44{};
            float field_48{};
            float field_4C{};

        protected:
            void* GetRuntimeTypeInfo() override { return (void*)ASLR(0x00FD7BF8); }

        private:
            inline static FUNCTION_PTR(void, __thiscall, __ct, ASLR(0x004C7D50), MovePopup* This);
            inline static FUNCTION_PTR(void, __thiscall, __dt, ASLR(0x006D9470), MovePopup* This, int a2);
            inline static FUNCTION_PTR(int, __thiscall, f_Update, ASLR(0x004C7DF0), MovePopup* This, const fnd::SUpdateInfo& updateInfo);
            inline static FUNCTION_PTR(int, __thiscall, f_OnEnter, ASLR(0x004C7D30), MovePopup* This);
            inline static FUNCTION_PTR(void, __thiscall, f_Setup, ASLR(0x004C7DC0), MovePopup* This, Description* description);
            inline static FUNCTION_PTR(int*, __thiscall, f_SetBaseTransform, ASLR(0x004C7FF0), MovePopup* This, csl::math::Vector3* a2, csl::math::Quaternion* a3);

        public:
            int Update(const fnd::SUpdateInfo& updateInfo) override
            {
                return f_Update(this, updateInfo);
            }
            int OnEnter() override { return f_OnEnter(this); }
            void Destructor(size_t deletingFlags) override
            {
                __dt(this, deletingFlags);
            }

            MovePopup()
            {
                __ct(this);
            }

            void Setup(Description* description)
            {
                f_Setup(this, description);
            }

            int* SetBaseTransform(csl::math::Vector3* a1, csl::math::Quaternion* a2)
            {
                return f_SetBaseTransform(this, a1, a2);
            }
        };
    }
}