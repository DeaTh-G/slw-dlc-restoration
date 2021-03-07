#pragma once

namespace app
{
	class GameObject3D : public GameObject
	{
	public:
		fnd::GOCTransform Transform;
		char Padding[100];
		fnd::GOCTransform* pTransform;
	};
}