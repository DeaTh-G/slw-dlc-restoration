#pragma once

namespace app
{
	namespace animation
	{
		class AnimationListener : public AnimationObj
		{
		private:
			inline static FUNCTION_PTR(void, __thiscall, __ct, ASLR(0x0040B1B0), AnimationListener* This);

		protected:
			void* animationManager{};
			virtual void OnEvent(int notifyTiming) {};

		public:
			int field_10;
			int field_14;
			int field_18;
			int field_1C;
			int field_20;
			int field_24;

			AnimationListener() { __ct(this); sizeof(AnimationListener); }
		};
	}
}