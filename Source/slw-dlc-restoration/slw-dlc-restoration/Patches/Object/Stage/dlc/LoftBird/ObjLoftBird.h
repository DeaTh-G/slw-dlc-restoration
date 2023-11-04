#pragma once

namespace slw_dlc_restoration
{
	class ObjLoftBird : public app::ObjLoftBird
	{
	public:
		void RemoveCallback(app::GameDocument* in_pDocument) override;
	};
}