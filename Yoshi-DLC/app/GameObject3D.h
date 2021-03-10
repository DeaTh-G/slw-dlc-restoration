#pragma once

namespace app
{
	class GameObject3D : public GameObject
	{
	private:
		inline static FUNCTION_PTR(bool, __thiscall, f_ProcessMessage, ASLR(0x0090BD70), GameObject3D* This, fnd::Message& message);
		inline static FUNCTION_PTR(void, __thiscall, __ct, ASLR(0x0090BC90), GameObject3D* This);

	public:
		fnd::GOCTransform Transform;
		char Padding[100];
		fnd::GOCTransform* pTransform;

		GameObject3D()
		{
			__ct(this);
		}

		bool ProcessMessage(fnd::Message& message) override { return f_ProcessMessage(this, message); }
	};
}