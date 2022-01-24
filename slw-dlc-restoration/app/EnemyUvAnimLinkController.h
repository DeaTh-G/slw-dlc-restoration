#pragma once

namespace app
{
    class EnemyUvAnimLinkController
    {
    public:
        int field_00{};
        int field_04{};
        int field_08{};
        int field_0C{};
        int field_10{};
        int field_14{};
        int field_18{};
        int field_1C{};
        int field_20{};
        int field_24{};
        int field_28{};

        struct Description
        {
            int field_00{};
            int field_04{};
            int field_08{};
            int field_0C[2]{};
            int field_14{};

            Description(int a1, int a2)
            {
                field_00 = a1;
                field_04 = a2;
            }
        };

    private:
        inline static FUNCTION_PTR(EnemyUvAnimLinkController*, __thiscall, __ct, ASLR(0x0072D6C0), EnemyUvAnimLinkController* This);
        inline static FUNCTION_PTR(EnemyUvAnimLinkController*, __thiscall, __dt, ASLR(0x0072D6F0), EnemyUvAnimLinkController* This);
        inline static FUNCTION_PTR(void*, __thiscall, f_Setup, ASLR(0x0072D780), EnemyUvAnimLinkController* This, int a2, int a3, int a4, int a5, int a6, char a7, csl::fnd::IAllocator* pAllocator);
        inline static FUNCTION_PTR(void*, __thiscall, f_Add, ASLR(0x0072D2A0), EnemyUvAnimLinkController* This, int* a2, const char* name, int a4);
        inline static FUNCTION_PTR(void, __thiscall, f_ChangeCallback, ASLR(0x0072D4D0), EnemyUvAnimLinkController* This, int* a2, int a3, int a4);

    public:
        EnemyUvAnimLinkController()
        {
            __ct(this);
        }

        ~EnemyUvAnimLinkController()
        {
            __dt(this);
        }

        void* Setup(Description description, csl::fnd::IAllocator* pAllocator)
        {
            return f_Setup(this, description.field_00, description.field_04, 0, description.field_0C[0], description.field_0C[1], 0, pAllocator);
        }

        void* Add(int* a2, const char* name, int a4)
        {
            return f_Add(this, a2, name, a4);
        }

        void ChangeCallback(int* a1, int a2, int a3) { f_ChangeCallback(this, a1, a2, a3); }
    };
}