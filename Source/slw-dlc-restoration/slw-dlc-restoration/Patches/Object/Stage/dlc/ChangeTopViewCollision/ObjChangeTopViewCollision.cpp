#include "pch.h"
#include "ObjChangeTopViewCollision.h"

bool slw_dlc_restoration::ObjChangeTopViewCollision::ProcessMessage(app::fnd::Message& in_rMessage)
{
	if (PreProcessMessage(in_rMessage))
		return true;

	switch (in_rMessage.GetType())
	{
	case app::xgame::MsgHitEventCollision::MessageID:			return ProcMsgHitEventCollision(static_cast<app::xgame::MsgHitEventCollision&>(in_rMessage));
	case app::xgame::MsgLeaveEventCollision::MessageID:			return ProcMsgLeaveEventCollision(static_cast<app::xgame::MsgLeaveEventCollision&>(in_rMessage));
	default:													return app::ObjChangeTopViewCollision::ProcessMessage(in_rMessage);
	}
}

bool slw_dlc_restoration::ObjChangeTopViewCollision::ProcMsgHitEventCollision(app::xgame::MsgHitEventCollision& in_rMessage)
{
	app::xgame::MsgPLChangeAreaTopView msg{};
	msg.IsTopView = true;

	app::ObjUtil::SendMessageImmToPlayer(*this, app::ObjUtil::GetPlayerNo(*GetDocument(), in_rMessage.Sender), msg);
	return true;
}

bool slw_dlc_restoration::ObjChangeTopViewCollision::ProcMsgLeaveEventCollision(app::xgame::MsgLeaveEventCollision& in_rMessage)
{
	app::xgame::MsgPLChangeAreaTopView msg{};
	msg.IsTopView = false;

	app::ObjUtil::SendMessageImmToPlayer(*this, app::ObjUtil::GetPlayerNo(*GetDocument(), in_rMessage.Sender), msg);
	return true;
}