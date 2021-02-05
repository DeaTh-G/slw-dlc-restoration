#pragma once

namespace app
{
	class GameDocument
	{
	public: 
		inline static FUNCTION_PTR(int*, __thiscall, GetServiceByClass, ASLR(0x0090B2E0), GameDocument* This, void* Class);
	};

	static GameDocument** Document = (GameDocument**)ASLR(0x00FEFEF4);
}