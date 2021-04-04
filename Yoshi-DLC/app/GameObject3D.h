#pragma once

namespace app
{
    class GameObject3D : public GameObject
    {
    private:
        inline static FUNCTION_PTR(bool, __thiscall, f_ProcessMessage, ASLR(0x0090BD70), CActor* This, fnd::Message& message);
        inline static FUNCTION_PTR(void, __thiscall, __ct, ASLR(0x0090BC90), GameObject3D* This);
        inline static FUNCTION_PTR(void, __thiscall, __dt, ASLR(0x0090BD30), GameObject3D* This, int a2);

    public:
        fnd::GOCTransform Transform{};
        char Padding[100]{};
        fnd::GOCTransform* pTransform = new fnd::GOCTransform();

        GameObject3D()
        {
            __ct(this);
        }

        void Destructor(size_t deletingFlags) override
        {
            __dt(this, deletingFlags);
        }

        bool ProcessMessage(fnd::Message& message) override { return f_ProcessMessage(this, message); }
    };
}