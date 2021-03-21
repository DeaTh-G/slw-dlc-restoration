#pragma once

namespace app
{
	class EnemyBase : public CSetObjectListener
	{
	private:
		inline static FUNCTION_PTR(EnemyBase*, __thiscall, __ct, ASLR(0x00671FA0), EnemyBase* This);
		inline static FUNCTION_PTR(EnemyBase*, __thiscall, __dt, ASLR(0x00672140), EnemyBase* This, int a2);
		inline static FUNCTION_PTR(bool, __thiscall, f_ProcessMessage, ASLR(0x006724B0), CActor* This, fnd::MessageNew* message);

	protected:
		bool ProcessMessage(fnd::MessageNew& message) override { return f_ProcessMessage(this, &message); }

	public:
		inline static FUNCTION_PTR(int*, __thiscall, CreateCenterPositionFrame, ASLR(0x00672040), GameObject* This, csl::math::Vector3* position);
		inline static FUNCTION_PTR(fnd::HFrame*, __thiscall, GetCenterPositionFrame, ASLR(0x00672080), GameObject* This);
		inline static FUNCTION_PTR(int*, __thiscall, SendTouchDamage, ASLR(0x00672180), GameObject* This, xgame::MsgDamage& message);

		fnd::HFrame Parent;
		char asd[32];

		EnemyBase() { __ct(this); }
		~EnemyBase() override
		{
			sizeof(EnemyBase);
			__dt(this, 0);
			FORCE_RET;
		}
	};

	class EnmBase
	{
	public:

		inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x00671FA0), GameObject* This);
		inline static FUNCTION_PTR(bool, __thiscall, ProcMsg, ASLR(0x006724B0), int* This, fnd::Message* message);
		inline static FUNCTION_PTR(int*, __thiscall, CreateCenterPositionFrame, ASLR(0x00672040), GameObject* This, csl::math::Vector3* position);
		inline static FUNCTION_PTR(fnd::HFrame*, __thiscall, GetCenterPositionFrame, ASLR(0x00672080), GameObject* This);
		inline static FUNCTION_PTR(int*, __thiscall, SendTouchDamage, ASLR(0x00672180), int* This, fnd::Message* message);
		inline static FUNCTION_PTR(int*, __thiscall, CreateEnemyBlowOffObject, ASLR(0x006720D0), GameObject* This, EnemyBlowOffObjectCInfo* blowOffObject);
		inline static FUNCTION_PTR(int*, __thiscall, ProcMission, ASLR(0x00671E20), GameObject* This, fnd::Message* message);
	};
}