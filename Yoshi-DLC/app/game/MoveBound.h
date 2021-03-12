#pragma once

namespace app
{
	namespace game
	{
		class MoveBound : public MoveController
		{
		public:
			INSERT_PADDING(0x80); // Fields

		private:
			inline static FUNCTION_PTR(int, __thiscall, f_Update, ASLR(0x004C5E40), MoveController* This, const fnd::SUpdateInfo& updateInfo);
			inline static FUNCTION_PTR(int, __thiscall, f_OnEnter, ASLR(0x004C5E20), MoveController* This);
			inline static FUNCTION_PTR(MoveBound*, __thiscall, __dt, ASLR(0x004C5400), MoveBound* This, int a2);

		public:
			void* GetRuntimeTypeInfo() override { return (void*)ASLR(0x00F6BDAC); }
			int Update(const fnd::SUpdateInfo& updateInfo) override { return f_Update(this, updateInfo); }
			int OnEnter() override { return f_OnEnter(this); }
			~MoveBound() override
			{
				__dt(this, 0);
				FORCE_RET;
			}

			class Listener
			{
			public:
				virtual void OnBound(csl::math::Plane& const a1) {};
			};
		};
	}
}