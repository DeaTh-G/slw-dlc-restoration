#pragma once

namespace slw_dlc_restoration
{
	class ObjChangeTopViewCollision : public app::ObjChangeTopViewCollision
	{
		bool ProcessMessage(app::fnd::Message& in_rMessage) override;
		bool ProcMsgHitEventCollision(app::xgame::MsgHitEventCollision& in_rMessage);
		bool ProcMsgLeaveEventCollision(app::xgame::MsgLeaveEventCollision& in_rMessage);
	};
}