#include "pch.h"
#include "ObjGossipStone.h"

slw_dlc_restoration::ObjGossipStone::ObjGossipStone() : app::ObjGossipStone()
{
	FSM_INIT(static_cast<BaseState>(&ObjGossipStone::StateIdle));
}

bool slw_dlc_restoration::ObjGossipStone::ProcessMessage(app::fnd::Message& in_rMessage)
{
	if (PreProcessMessage(in_rMessage))
		return true;

	switch (in_rMessage.GetType())
	{
	case app::xgame::MsgDamage::MessageID:					return ProcMsgDamage(static_cast<app::xgame::MsgDamage&>(in_rMessage));
	case app::xgame::MsgHitEventCollision::MessageID:		return ProcMsgHitEventCollision(static_cast<app::xgame::MsgHitEventCollision&>(in_rMessage));
	default:												return app::ObjGossipStone::ProcessMessage(in_rMessage);
	}
}

bool slw_dlc_restoration::ObjGossipStone::ProcMsgDamage(app::xgame::MsgDamage& in_rMessage)
{
	auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), app::ObjUtil::GetPlayerNo(*GetDocument(), in_rMessage.Sender));
	if (!pPlayerInfo)
		return false;

	if (pPlayerInfo->PixieNo == static_cast<app::Game::EPhantomType>(-1))
	{
		if (in_rMessage.Unk3.dot(GetComponent<app::fnd::GOCTransform>()->Frame.Unk3.Mtx.GetColumn(1)) >= 0.9f)
			return false;
	}

	DispatchFSM(TiFsmEvent_t::CreateMessage(in_rMessage));

	in_rMessage.SetReply(in_rMessage.Unk2, false, { -in_rMessage.Unk3 });
	in_rMessage.ReplyStatus.set(0);

	if (pPlayerInfo->PixieNo != static_cast<app::Game::EPhantomType>(-1))
		in_rMessage.ReplyStatus.set(5);

	return true;
}

bool slw_dlc_restoration::ObjGossipStone::ProcMsgHitEventCollision(app::xgame::MsgHitEventCollision& in_rMessage)
{
	if (app::ObjUtil::CheckShapeUserID(in_rMessage.pSelf, 0))
		return OnSnapshot();

	if (!app::ObjUtil::CheckShapeUserID(in_rMessage.pSelf, 1))
		return false;

	auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), app::ObjUtil::GetPlayerNo(*GetDocument(), in_rMessage.Sender));
	if (!pPlayerInfo || pPlayerInfo->IsOnGround)
		return false;

	csl::math::Vector3 upVector = GetComponent<app::fnd::GOCTransform>()->Frame.Unk3.Mtx.GetColumn(1);
	csl::math::Vector3 contactNormal{ -(*in_rMessage.GetContactPointNormal()) };

	csl::math::Vector3 velocity{ contactNormal - csl::math::Vector3(upVector * upVector.dot(contactNormal)) };
	app::math::Vector3NormalizeZero(velocity, &velocity);

	app::xgame::MsgAppeareKnockback msg{};
	msg.Velocity = { csl::math::Vector3(upVector * upVector.dot(contactNormal)) + csl::math::Vector3(velocity * 30.0f * 2.0f) };
	SendMessageImm(in_rMessage.Sender, msg);

	DispatchFSM(TiFsmEvent_t::CreateMessage(in_rMessage));
	return true;
}

void slw_dlc_restoration::ObjGossipStone::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	if (FSM_STATE() == &app::ObjGossipStone::StateIdle)
		ChangeState(static_cast<BaseState>(&ObjGossipStone::StateIdle));
	else if (FSM_STATE() == &app::ObjGossipStone::StateShake)
		ChangeState(static_cast<BaseState>(&ObjGossipStone::StateShake));

	DispatchFSM(TiFsmEvent_t::CreateUpdate(in_rUpdateInfo.DeltaTime));
}

app::TTinyFsm<app::ObjGossipStone>::TiFsmState_t slw_dlc_restoration::ObjGossipStone::StateIdle(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_MESSAGE:
	{
		auto& message = in_rEvent.getMessage();
		if (message.GetType() != app::xgame::MsgDamage::MessageID)
			break;

		auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), app::ObjUtil::GetPlayerNo(*GetDocument(), static_cast<app::xgame::MsgDamage&>(message).Sender));

		if (static_cast<app::xgame::MsgDamage&>(message).Bonus.Unk1 == 3 && pPlayerInfo && pPlayerInfo->PixieNo == app::Game::EPhantomType::ePhantom_Bomb)
			ChangeState(&ObjGossipStone::StateCountdown);
		else
			ChangeState(static_cast<BaseState>(&ObjGossipStone::StateShake));

		message.Handled = true;
		return {};
	}
	}

	return app::ObjGossipStone::StateIdle(in_rEvent);
}

app::TTinyFsm<app::ObjGossipStone>::TiFsmState_t slw_dlc_restoration::ObjGossipStone::StateShake(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_MESSAGE:
	{
		auto& message = in_rEvent.getMessage();
		if (message.GetType() != app::xgame::MsgDamage::MessageID)
			break;

		auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), app::ObjUtil::GetPlayerNo(*GetDocument(), static_cast<app::xgame::MsgDamage&>(message).Sender));

		if (static_cast<app::xgame::MsgDamage&>(message).Bonus.Unk1 == 3 && pPlayerInfo && pPlayerInfo->PixieNo == app::Game::EPhantomType::ePhantom_Bomb)
		{
			IsDamaged = true;
		}
		else
		{
			if (!IsDamaged)
			{
				GetComponent<app::game::GOCAnimationSimple>()->SetAnimation(ms_pAnimationName);
				GetComponent<app::game::GOCSound>()->Play3D(ms_pBibeSoundName, 0.0f);
			}
		}

		message.Handled = true;
		return {};
	}
	}

	return app::ObjGossipStone::StateShake(in_rEvent);
}