#include "pch.h"
#include "ObjLoftBird.h"

void slw_dlc_restoration::ObjLoftBird::RemoveCallback(app::GameDocument* in_pDocument)
{
	for (auto& light : PointLights)
	{
		if (!light.IsValid())
			continue;
		
		app::xgame::MsgKill msg{};
		SendMessageImmToGameObject(light.Get(), msg);
	}
}