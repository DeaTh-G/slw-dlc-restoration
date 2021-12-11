#pragma once

namespace app
{
	namespace game
	{
		struct PhysicsRaycastOutput
		{
			int Data[16]{};
		};

		class PhysicsRaycastJob : public PhysicsQueryJob
		{
		private:
			inline static FUNCTION_PTR(void, __thiscall, f_Add, ASLR(0x004DB340), PhysicsRaycastJob* This, csl::math::Vector3* a2, csl::math::Vector3* a3, int a4, int a5, int a6);
			inline static FUNCTION_PTR(void, __thiscall, f_Clear, ASLR(0x004DB3B0), PhysicsRaycastJob* This);

		public:
			Type Type{};
			int Commands[20]{};
			int field_60{};
			INSERT_PADDING(12);
			PhysicsRaycastOutput Output[2]{};

			void Add(csl::math::Vector3* a2, csl::math::Vector3* a3, int a4, int a5, int a6) { f_Add(this, a2, a3, a4, a5, a6); }
			void Clear() { f_Clear(this); }
		};
	}
}