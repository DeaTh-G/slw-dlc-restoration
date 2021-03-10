#pragma once

namespace app
{
	class CSetObjectListener : public GameObject3D
	{
	public:
		char field_031C[0x78];

		inline static FUNCTION_PTR(void, __thiscall, SetStatusRetire, ASLR(0x00844790), GameObject* This);
		inline static FUNCTION_PTR(void, __thiscall, SetStatusTemporaryRetire, ASLR(0x008447B0), GameObject* This, float a2);
		inline static FUNCTION_PTR(bool, __thiscall, f_ProcessMessage, ASLR(0x00844A30), CSetObjectListener* This, fnd::Message* message);
		inline static FUNCTION_PTR(GameObject*, __thiscall, __ct, ASLR(0x008448A0), GameObject* This);
		inline static FUNCTION_PTR(GameObject*, __thiscall, GetParentObject, ASLR(0x00844AE0), GameObject* This);
		inline static FUNCTION_PTR(uint32_t, __thiscall, GetExtUserData, ASLR(0x00844830), GameObject* This, int a2);
		inline static FUNCTION_PTR(int, __thiscall, SetExtUserData, ASLR(0x00844850), GameObject* This, int a2, int a3);
	
	protected:
		bool ProcessMessage(fnd::Message& message) override { return f_ProcessMessage(this, &message); }
		virtual void OnInitializedSetObject() {};
		virtual void OnPutSetObject(CSetAdapter& adapter) {}
		virtual void OnSetEditorParamChanged(CSetAdapter& adapter) {};

	public:
		CSetObjectListener() { __ct(this); }
		virtual unsigned int GetAbsordGuidePathID() { return 0; }
		virtual double GetAbsorbPathPosition() { return -1; }
		virtual csl::math::Vector3 GetAbsorbPathOffset() { return { 0, 0, 0 }; }
		virtual void SetAppearance(float opacity) {}
	};
}