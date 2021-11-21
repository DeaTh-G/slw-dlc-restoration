#pragma once

namespace app
{
    class EnemyBase : public CSetObjectListener
    {
    private:
        inline static FUNCTION_PTR(EnemyBase*, __thiscall, __ct, ASLR(0x00671FA0), EnemyBase* This);
        inline static FUNCTION_PTR(EnemyBase*, __thiscall, __dt, ASLR(0x00672140), EnemyBase* This, int a2);
        inline static FUNCTION_PTR(bool, __thiscall, f_ProcessMessage, ASLR(0x006724B0), CActor* This, fnd::Message& message);
        inline static FUNCTION_PTR(fnd::HFrame*, __thiscall, f_GetCenterPositionFrame, ASLR(0x00672080), EnemyBase* This);
        inline static FUNCTION_PTR(int*, __thiscall, f_SendTouchDamage, ASLR(0x00672180), EnemyBase* This, xgame::MsgDamage& message);

    protected:
        bool ProcessMessage(fnd::Message& message) override { return f_ProcessMessage(this, message); }

    public:
        inline static FUNCTION_PTR(int*, __thiscall, CreateCenterPositionFrame, ASLR(0x00672040), EnemyBase* This, csl::math::Vector3* position);
        inline static FUNCTION_PTR(int*, __thiscall, ProcMission, ASLR(0x00671E20), EnemyBase* This, fnd::Message* message);
        inline static FUNCTION_PTR(int*, __thiscall, CreateEnemyBlowOffObject, ASLR(0x006720D0), EnemyBase* This, EnemyBlowOffObjectCInfo* blowOffObject);

        fnd::HFrame Parent;
        INSERT_PADDING(32);

        EnemyBase() : CSetObjectListener(1) { __ct(this); }
        void Destructor(size_t deletingFlags) override
        {
            __dt(this, deletingFlags);
        }

        fnd::HFrame* GetCenterPositionFrame() { return f_GetCenterPositionFrame(this); }
        int* SendTouchDamage(xgame::MsgDamage& message) { return f_SendTouchDamage(this, message); }
    };
}