#pragma once

namespace app
{
	class GameDocument
	{
	public: 
		inline static FUNCTION_PTR(int*, __thiscall, GetServiceByClass, ASLR(0x0090B2E0), GameDocument* This, void* Class);
		inline static FUNCTION_PTR(void, __thiscall, AddGameObject, ASLR(0x0090B3C0), GameDocument& This, void* obj);
	};

	static GameDocument** Document = (GameDocument**)ASLR(0x00FEFEF4);
}