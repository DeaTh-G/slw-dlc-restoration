#pragma once

namespace slw_dlc_restoration
{
	class ObjZeldaGoal : public app::ObjZeldaGoal
	{
	public:
		typedef TiFsmState_t(app::ObjZeldaGoal::* BaseState)(const TiFsmEvent_t&);

	public:
		void AddCallback(app::GameDocument* in_pDocument) override;
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		TiFsmState_t StateFadeIn(const TiFsmEvent_t& in_rEvent);
	};
}