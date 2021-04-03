#pragma once

namespace app
{
    class EnemyUvAnimLinkController
    {
    public:
        struct Description
        {
            int field_00;
            int field_04;
            int field_08;
            int field_0C;
            int field_10;
            int field_14;
            int field_18;
            int field_1C;
            int field_20;
            int field_24;
            int field_28;
        };

        struct UvBase
        {
            csl::math::Vector3 UVVector{};
            int field_10[2]{};
        };

    private:
        inline static FUNCTION_PTR(Description*, __thiscall, __ct, ASLR(0x0072D6C0), Description* This);
        inline static FUNCTION_PTR(void*, __thiscall, f_Setup, ASLR(0x0072D780), Description* This, float a2, float a3, float a4, float a5, int* a6, char a7, csl::fnd::IAllocator* pAllocator);
        inline static FUNCTION_PTR(void*, __thiscall, f_Add, ASLR(0x0072D6C0), Description* This, int* a2, const char* name, int a4);

    public:
        EnemyUvAnimLinkController(Description* This)
        {
            __ct(This);
        }

        static void* Setup(Description* This, UvBase* a2, csl::fnd::IAllocator* pAllocator)
        {
            return f_Setup(This, a2->UVVector.X, a2->UVVector.Y, a2->UVVector.Z, 0, a2->field_10, 0, pAllocator);
        }

        static void* Add(Description* This, int* a2, const char* name, int a4)
        {
            return f_Add(This, a2, name, a4);
        }
    };
}