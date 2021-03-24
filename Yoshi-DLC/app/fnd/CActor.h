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
			inline static FUNCTION_PTR(char, __thiscall, f_SendMessageImm, ASLR(0x0049A470), CActor* This, int a2, fnd::MessageNew* msg);

		public:
			inline static FUNCTION_PTR(char, __thiscall, SendMessageImmOld, ASLR(0x0049A470), int* This, int a2, void* msg);
			int field_04[6]{};

			virtual ~CActor()
			{
				__dt(this);
				FORCE_RET;
			}

			char SendMessageImm(int actorID, fnd::MessageNew* msg)
			{
				return f_SendMessageImm(this, actorID, msg);
			}

			virtual size_t ForEach(int& traverser) = 0;
		
		protected:
			virtual bool PreProcessMessage(MessageNew& message) { return false; }
			virtual bool ProcessMessage(MessageNew& message) { return PreProcessMessage(message); }
			virtual void Update(const SUpdateInfo& updateInfo) { };
			virtual bool ActorProc(int id, void* data) = 0;
		};
	}
}
