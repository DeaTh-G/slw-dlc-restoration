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
}