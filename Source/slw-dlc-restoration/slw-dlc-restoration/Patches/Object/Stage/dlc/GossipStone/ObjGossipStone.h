#pragma once

namespace slw_dlc_restoration
{
	class ObjGossipStone : public app::ObjGossipStone
	{
	public:
		typedef TiFsmState_t(app::ObjGossipStone::*BaseState)(const TiFsmEvent_t&);

	public:
		ObjGossipStone();
		bool ProcessMessage(app::fnd::Message& in_rMessage) override;
		bool ProcMsgDamage(app::xgame::MsgDamage& in_rMessage);
		bool ProcMsgHitEventCollision(app::xgame::MsgHitEventCollision& in_rMessage);
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		TiFsmState_t StateIdle(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StateShake(const TiFsmEvent_t& in_rEvent);
	};
}