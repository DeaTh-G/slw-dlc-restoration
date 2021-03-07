#pragma once

namespace app
{
	GameObject* Create_Small_ObjYoshiJumpBoard()
	{
		GameObject* object = GameObject::New(0x410);
		if (!object)
			return 0;
		((ObjYoshiJumpBoard*)object)->__ct(1);
		return object;
	}

	fnd::ReferencedObject* createObjInfo_ObjYoshiJumpBoardSmall(csl::fnd::IAllocator* a1)
	{
		fnd::ReferencedObject* object = fnd::ReferencedObject::New(0x54, a1);
		if (!object)
			return 0;
		((ObjYoshiJumpBoardInfo*)object)->__ct();
		return object;
	}
}