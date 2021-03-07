#pragma once

namespace app
{
	class CObjInfo : public fnd::ReferencedObject
	{
	public:
		inline static FUNCTION_PTR(int*, __thiscall, __ct, ASLR(0x00840360), CObjInfo* This);

		int field_0C;

		CObjInfo()
		{
			__ct(this);
		}
	};
}