#pragma once

namespace app
{
	class CPhysicsWorld
	{
	private:
		inline static FUNCTION_PTR(int*, __cdecl, staticClass, ASLR(0x00442B60));
		inline static FUNCTION_PTR(void, __thiscall, f_AddRaycastJob, ASLR(0x00442C10), CPhysicsWorld* This, game::PhysicsRaycastJob* raycastJob);

	public:
		static void* GetService(GameDocument* gameDocument)
		{
			void* physicsWorld = staticClass();
			void* service = GameDocument::GetServiceByClass(gameDocument, physicsWorld);
			return service;
		}

		void AddRaycastJob(game::PhysicsRaycastJob* raycastJob) { f_AddRaycastJob(this, raycastJob); }
	};
}