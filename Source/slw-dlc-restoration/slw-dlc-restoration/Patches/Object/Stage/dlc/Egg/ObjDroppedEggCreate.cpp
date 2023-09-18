#include "pch.h"
#include "ObjDroppedEggCreate.h"

slw_dlc_restoration::ObjDroppedEgg* slw_dlc_restoration::egg::CreateDroppedEgg(app::GameDocument& in_rDocument, const slw_dlc_restoration::egg::DroppedEggCInfo& in_rInfo)
{
	auto* pObject = new slw_dlc_restoration::ObjDroppedEgg(&in_rInfo);
	if (!pObject)
		return { nullptr };

	in_rDocument.AddGameObject(pObject);
	return pObject;
}