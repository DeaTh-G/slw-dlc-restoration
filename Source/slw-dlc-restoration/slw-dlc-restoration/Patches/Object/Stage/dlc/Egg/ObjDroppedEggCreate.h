#pragma once

namespace slw_dlc_restoration
{
	class ObjDroppedEgg;
}

namespace slw_dlc_restoration::egg
{
	extern ObjDroppedEgg* CreateDroppedEgg(app::GameDocument& in_rDocument, const slw_dlc_restoration::egg::DroppedEggCInfo& in_rInfo);
}