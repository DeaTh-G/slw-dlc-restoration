#pragma once

namespace app
{
    class CObjInfoFileLoader
    {
    public:
        virtual void Load(const char* name, int a2) = 0;
        virtual void LoadEx(const char* name, const fnd::FileLoaderParam& params) = 0;
    };

    class CObjInfo : public fnd::ReferencedObject
    {
    public:
        inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x00840360), CObjInfo* This);

        int field_0C{};

        CObjInfo()
        {
            __ct(this);
        }

        virtual void Load(app::CObjInfoFileLoader& loader) {}
        virtual void Initialize(GameDocument& document) {}
        virtual void Finalize() {}
        virtual const char* GetInfoName() = 0;
        virtual void RegistCallback(int& container) {}
    };
}