#pragma once

namespace slw_dlc_restoration
{
	class ObjEventDrowning : public app::ObjEventDrowning
	{
	public:
		void AddCallback(app::GameDocument* in_pDocument) override;
		bool ProcessMessage(app::fnd::Message& in_rMessage) override;
		bool ProcMsgHitEventCollision(app::xgame::MsgHitEventCollision& in_rMessage);
	};
}