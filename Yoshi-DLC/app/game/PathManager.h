#pragma once

namespace app
{
	namespace game
	{
		class PathManager
		{
		public:
			inline static FUNCTION_PTR(void*, __cdecl, staticClass, ASLR(0x004D9720));
			inline static FUNCTION_PTR(int, __thiscall, CastRay, ASLR(0x004D9730), void* This);

			static void* GetService(GameDocument* gameDocument)
			{
				void* managerClass = staticClass();
				void* service = GameDocument::GetServiceByClass(gameDocument, managerClass);
				return service;
			}
		};
	}
}