#pragma once

namespace slw_dlc_restoration
{
	namespace egg
	{
		struct DroppedEggCInfo : public app::egg::DroppedEggCInfo
		{
			uint PlayerNo{};
		};
	}

	class ObjDroppedEgg : public app::ObjDroppedEgg
	{
	public:
		typedef TiFsmState_t(app::ObjDroppedEgg::* BaseState)(const TiFsmEvent_t&);

		uint PlayerNo{};

		ObjDroppedEgg(const slw_dlc_restoration::egg::DroppedEggCInfo* in_pInfo);
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		bool ProcessMessage(app::fnd::Message& in_rMessage) override;
		bool ProcMsgHitEventCollision(app::xgame::MsgHitEventCollision& in_rMessage);
		TiFsmState_t StateWait(const TiFsmEvent_t& in_rEvent);
	};
}