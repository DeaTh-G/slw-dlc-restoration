#pragma once

namespace app
{
	namespace game
	{
		class MoveController
		{
		public:
			INSERT_PADDING(0xC); // Fields

		private:
			inline static FUNCTION_PTR(MoveController*, __thiscall, __dt, ASLR(0x004C3D50), MoveController* This, int a2);
			
		protected:
			virtual void* GetRuntimeTypeInfo() { return (void*)ASLR(0x00FD7BF8); }
			virtual int Update(const fnd::SUpdateInfo& updateInfo) {};
			virtual int OnEnter() {};
			virtual void OnLeave() {};

		public:
			virtual ~MoveController()
			{
				__dt(this, 0);
				FORCE_RET;
			}
		};
	}
}