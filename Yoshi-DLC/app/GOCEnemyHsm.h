#pragma once

namespace app
{
    class GOCEnemyHsm
    {
    public:
        struct StateDesc
        {
            int ID;
            ut::internal::StateDescImpl* Impl;
        };

        struct Description
        {
            StateDesc* Descriptors;
            int DescriptorCount;
            int field_08;
        };

        inline static FUNCTION_PTR(int, __thiscall, Setup, ASLR(0x00724030), int* This, Description* description);
        inline static FUNCTION_PTR(int, __thiscall, ChangeState, ASLR(0x007240C0), int* This, int stateID);
        inline static FUNCTION_PTR(int, __thiscall, Dispatch, ASLR(0x00724120), int* This, fnd::Message* message);
        inline static FUNCTION_PTR(int, __thiscall, SetEnableUpdate, ASLR(0x00724100), int* This, bool isEnable);
        inline static FUNCTION_PTR(int, __thiscall, GetCurrentStateID, ASLR(0x00723F50), int* This);
    };
}