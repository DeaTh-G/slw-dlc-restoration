#pragma once

namespace app
{
	namespace fnd
	{
		struct SUpdateInfo
		{
			float deltaTime;
			unsigned int frame;
		};

		class CActor : public csl::ut::NonCopyable
		{
		private:
			inline static FUNCTION_PTR(CActor*, __thiscall, __dt, ASLR(0x0049A350), CActor* This);

		public:
			inline static FUNCTION_PTR(char, __thiscall, SendMessageImm, ASLR(0x0049A470), int* This, int a2, void* msg);
			int field_04[6];

			virtual ~CActor()
			{
				__dt(this);
				FORCE_RET;
			}

			virtual size_t ForEach(int& traverser) = 0;
		
		protected:
			virtual bool PreProcessMessage(Message& message) { return false; }
			virtual bool ProcessMessage(Message& message) { return PreProcessMessage(message); }
			virtual void Update(const SUpdateInfo& updateInfo) { };
			virtual bool ActorProc(int id, void* data) = 0;
		};
	}
}
