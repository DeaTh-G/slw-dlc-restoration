#pragma once

namespace app
{
    class CSetObjectListener : public GameObject3D
    {
    private:
        inline static FUNCTION_PTR(bool, __thiscall, f_ProcessMessage, ASLR(0x00844A30), CActor* This, fnd::Message* message);
        inline static FUNCTION_PTR(GameObject*, __thiscall, __ct, ASLR(0x008448A0), GameObject* This);
        inline static FUNCTION_PTR(CSetObjectListener*, __thiscall, __dt, ASLR(0x00844A70), CSetObjectListener* This, int a2);
        inline static FUNCTION_PTR(uint32_t, __thiscall, f_GetExtUserData, ASLR(0x00844830), GameObject* This, int a2);
        inline static FUNCTION_PTR(uint32_t, __thiscall, f_SetExtUserData, ASLR(0x00844850), GameObject* This, int a2, int a3);

    public:
        char field_031C[0x88]{};

        inline static FUNCTION_PTR(void, __thiscall, SetStatusRetire, ASLR(0x00844790), GameObject* This);
        inline static FUNCTION_PTR(void, __thiscall, SetStatusTemporaryRetire, ASLR(0x008447B0), GameObject* This, float a2);
        inline static FUNCTION_PTR(GameObject*, __thiscall, GetParentObject, ASLR(0x00844AE0), GameObject* This);

    protected:
        bool ProcessMessage(fnd::Message& message) override { return f_ProcessMessage(this, &message); }
        virtual void OnInitializedSetObject() {};
        virtual void OnPutSetObject(CSetAdapter& adapter) {}
        virtual void OnSetEditorParamChanged(CSetAdapter& adapter) {};

    public:
        CSetObjectListener() { __ct(this); }
        void Destructor(size_t deletingFlags) override
        {
            __dt(this, deletingFlags);
        }

        virtual unsigned int GetAbsordGuidePathID() { return 0; }
        virtual double GetAbsorbPathPosition() { return -1; }
        virtual csl::math::Vector3 GetAbsorbPathOffset() { return { 0, 0, 0 }; }
        virtual void SetAppearance(float opacity) {}

        uint32_t GetExtUserData(int a1) { return f_GetExtUserData(this, a1); }
        uint32_t SetExtUserData(int a1, int a2) { return f_SetExtUserData(this, a1, a2); }
    };
}