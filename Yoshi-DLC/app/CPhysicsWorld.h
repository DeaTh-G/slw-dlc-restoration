#pragma once

namespace app
{
	class CPhysicsWorld
	{
	private:
		inline static FUNCTION_PTR(void, __thiscall, f_AddRaycastJob, ASLR(0x00442C10), CPhysicsWorld* This, game::PhysicsRaycastJob* raycastJob);

	public:
		void AddRaycastJob(game::PhysicsRaycastJob* raycastJob) { f_AddRaycastJob(this, raycastJob); }
	};
}