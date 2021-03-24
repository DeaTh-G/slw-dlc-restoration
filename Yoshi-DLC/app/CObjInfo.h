#pragma once

namespace app
{
    class CObjInfo : public fnd::ReferencedObject
    {
    public:
        inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x00840360), CObjInfo* This);

        int field_0C{};

        CObjInfo()
        {
            __ct(this);
        }

        virtual void Load(int& loader) {}
        virtual void Initialize(GameDocument& document) {}
        virtual void Finalize() {}
        virtual const char* GetInfoName() = 0;
        virtual void RegistCallback(int& container) {}
    };
}