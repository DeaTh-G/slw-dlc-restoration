#pragma once

namespace app
{
	GameObject* Create_Big_ObjYoshiJumpBoard()
	{
		GameObject* object = GameObject::New(0x410);
		if (!object)
			return 0;
		((ObjYoshiJumpBoard*)object)->__ct(0);
		return object;
	}

	fnd::ReferencedObject* createObjInfo_ObjYoshiJumpBoardBig(csl::fnd::IAllocator* a1)
	{
		fnd::ReferencedObject* object = fnd::ReferencedObject::f_new(0x54, a1);
		if (!object)
			return 0;
		((ObjYoshiJumpBoardInfo*)object)->__ct();
		return object;
	}
}