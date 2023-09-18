#pragma once

namespace slw_dlc_restoration
{
	class ObjGolonRockGenerator : public app::ObjGolonRockGenerator
	{
	public:
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
	};
}