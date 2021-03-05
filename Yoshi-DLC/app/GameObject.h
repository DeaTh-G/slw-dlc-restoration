#pragma once

namespace app
{
	class GameObject
	{
	public:
		inline static FUNCTION_PTR(GameObject*, __cdecl, New, ASLR(0x0049CAD0), size_t size);
		inline static FUNCTION_PTR(void, __thiscall, SetObjectCategory, ASLR(0x0049CAB0), GameObject* This, char category);
		inline static FUNCTION_PTR(int*, __thiscall, GetGOC, ASLR(0x0049D430), GameObject* This, char* component);
		inline static FUNCTION_PTR(void, __thiscall, Kill, ASLR(0x0049CCA0), GameObject* This);
		inline static FUNCTION_PTR(void, __thiscall, SetStatusRetire, ASLR(0x00844790), GameObject* This);
		inline static FUNCTION_PTR(int*, __cdecl, NewGameObject, ASLR(0x0049CAD0), size_t size);
		inline static FUNCTION_PTR(bool, __thiscall, IsKilled, ASLR(0x0079A150), GameObject* size);
		inline static FUNCTION_PTR(csl::fnd::IAllocator*, __cdecl, GetAllocator, ASLR(0x0049CC90));
	
		OBJECT_FUNCTION(void, Sleep, ASLR(0x0049CCB0));

		int Data[0x300];

		void* operator new(size_t size)
		{
			return NewGameObject(size);
		}
	};

	class GameObjectHandleBase
	{
	public:
		inline static FUNCTION_PTR(GameObject*, __thiscall, __ct, ASLR(0x0049D490), void* This, GameObject* a1);
	};
}