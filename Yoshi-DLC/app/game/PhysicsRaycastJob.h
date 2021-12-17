#pragma once

namespace app
{
	namespace game
	{
		struct PhysicsRaycastCommand
		{
			csl::math::Vector3 field_00{};
			csl::math::Vector3 field_10{};
			int field_20{};
			int field_24{};
			int field_28{};
			int field_2C{};
			int field_30{};
			int field_34{};
			int field_38{};
			int field_3C{};
		};

		struct PhysicsRaycastOutput
		{
			int Data[16]{};
		};

		class alignas(16) PhysicsRaycastJob : public PhysicsQueryJob
		{
		private:
			inline static FUNCTION_PTR(void, __thiscall, __ct, ASLR(0x004DB3D0), PhysicsRaycastJob* This);
			inline static FUNCTION_PTR(void, __thiscall, f_Add, ASLR(0x004DB340), PhysicsRaycastJob* This, csl::math::Vector3* a2, csl::math::Vector3* a3, int a4, int a5, int a6);
			inline static FUNCTION_PTR(void, __thiscall, f_Clear, ASLR(0x004DB3B0), PhysicsRaycastJob* This);

		public:
			Type Type{};
			csl::ut::InplaceMoveArrayAligned16<PhysicsRaycastCommand, 1> Commands{};
			int field_60{};
			INSERT_PADDING(12);
			PhysicsRaycastOutput Output[2]{};

			~PhysicsRaycastJob() { Destructor(0); };
			PhysicsRaycastJob() { __ct(this); }
			void Add(csl::math::Vector3* a2, csl::math::Vector3* a3, int a4, int a5, int a6) { f_Add(this, a2, a3, a4, a5, a6); }
			void Clear() { f_Clear(this); }
		};
	}
}