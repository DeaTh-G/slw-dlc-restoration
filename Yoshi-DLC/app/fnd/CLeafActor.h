#pragma once

namespace app
{
	namespace fnd
	{
		class CLeafActor : public CActor
		{
		private:
			inline static FUNCTION_PTR(size_t, __thiscall, f_ForEach, ASLR(0x0048D3F0), CActor* This, int& traverser);
			inline static FUNCTION_PTR(bool, __thiscall, f_ActorProc, ASLR(0x0048D410), CActor* This, int id, void* data);

		public:
			size_t ForEach(int& traverser) override { return f_ForEach(this, traverser); }
			bool ActorProc(int id, void* data) override { return f_ActorProc(this, id, data); }
		};
	}
}
