#pragma once

namespace slw_dlc_restoration::game
{
	class MoveObjGolonRock : public app::game::MoveObjGolonRock
	{
	public:
		MoveObjGolonRock();
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		bool IsPassOverPlayer() const;
	};
}