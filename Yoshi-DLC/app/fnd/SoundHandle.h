#pragma once

namespace app
{
	namespace fnd
	{
		class SoundHandle
		{
		private:
			inline static FUNCTION_PTR(void, __thiscall, f_Stop, ASLR(0x004A1FE0), SoundHandle* This, float fade);

		public:
			void Stop(float fade) { f_Stop(this, fade); }
		};
	}
}