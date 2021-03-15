#pragma once

namespace app
{
	class CSetObjectListener : public GameObject3D
	{
	private:
		/* After updating codebase update this to proper name*/
		inline static FUNCTION_PTR(bool, __thiscall, f_ProcMsg, ASLR(0x00844A30), CActor* This, fnd::MessageNew* message);
		inline static FUNCTION_PTR(CSetObjectListener*, __thiscall, __dt, ASLR(0x00844A70), CSetObjectListener* This, int a2);

	public:
		char field_031C[0x88];

		inline static FUNCTION_PTR(void, __thiscall, SetStatusRetire, ASLR(0x00844790), GameObject* This);
		inline static FUNCTION_PTR(void, __thiscall, SetStatusTemporaryRetire, ASLR(0x008447B0), GameObject* This, float a2);
		inline static FUNCTION_PTR(bool, __thiscall, f_ProcessMessage, ASLR(0x00844A30), CSetObjectListener* This, fnd::Message* message);
		inline static FUNCTION_PTR(GameObject*, __thiscall, __ct, ASLR(0x008448A0), GameObject* This);
		inline static FUNCTION_PTR(GameObject*, __thiscall, GetParentObject, ASLR(0x00844AE0), GameObject* This);
		inline static FUNCTION_PTR(uint32_t, __thiscall, GetExtUserData, ASLR(0x00844830), GameObject* This, int a2);
		inline static FUNCTION_PTR(int, __thiscall, SetExtUserData, ASLR(0x00844850), GameObject* This, int a2, int a3);
	
	protected:
		bool ProcessMessage(fnd::MessageNew& message) override { return f_ProcMsg(this, &message); }
		virtual void OnInitializedSetObject() {};
		virtual void OnPutSetObject(CSetAdapter& adapter) {}
		virtual void OnSetEditorParamChanged(CSetAdapter& adapter) {};

	public:
		CSetObjectListener() { __ct(this); }
		~CSetObjectListener() override
		{
			__dt(this, 0);
			FORCE_RET;
		}

		virtual unsigned int GetAbsordGuidePathID() { return 0; }
		virtual double GetAbsorbPathPosition() { return -1; }
		virtual csl::math::Vector3 GetAbsorbPathOffset() { return { 0, 0, 0 }; }
		virtual void SetAppearance(float opacity) {}
	};
}