#pragma once
#include "../framework.h"

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
	
		OBJECT_FUNCTION(void, Sleep, ASLR(0x0049CCB0));

		int Data[0x300];

		void* operator new(size_t size)
		{
			return NewGameObject(size);
		}
	};
}

//FUNCTION_PTR(int*, __thiscall, GameObjectHandleBaseCtor, ASLR(0x0049D490),void* This, GameObject* obj);

struct GameObjectHandleBase
{
	int* Unknown;
	int* Unknown2;

	GameObjectHandleBase(app::GameObject* obj)
	{
		Unknown = nullptr;
		Unknown2 = nullptr;
		//GameObjectHandleBaseCtor(this, obj);
	}
};
