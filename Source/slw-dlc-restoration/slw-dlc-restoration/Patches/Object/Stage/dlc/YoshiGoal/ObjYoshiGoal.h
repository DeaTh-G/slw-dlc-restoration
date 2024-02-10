#pragma once

namespace slw_dlc_restoration
{
	class ObjYoshiGoal : public app::ObjYoshiGoal
	{
	public:
		typedef TiFsmState_t(app::ObjYoshiGoal::* BaseState)(const TiFsmEvent_t&);

	public:
		ObjYoshiGoal();
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		TiFsmState_t StateWait(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StateWaitYoshiExtrication(const TiFsmEvent_t& in_rEvent);
	};
}