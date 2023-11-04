#include "pch.h"
#include "ObjDroppedEgg.h"

slw_dlc_restoration::ObjDroppedEgg::ObjDroppedEgg(const slw_dlc_restoration::egg::DroppedEggCInfo* in_pInfo) : app::ObjDroppedEgg(in_pInfo)
{
	PlayerNo = in_pInfo->PlayerNo;
}

void slw_dlc_restoration::ObjDroppedEgg::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	if (FSM_STATE() == &app::ObjDroppedEgg::StateWait)
		ChangeState(static_cast<BaseState>(&ObjDroppedEgg::StateWait));

	DispatchFSM(app::TiFsmBasicEvent<app::ObjDroppedEgg>::CreateUpdate(in_rUpdateInfo.DeltaTime));
}

bool slw_dlc_restoration::ObjDroppedEgg::ProcessMessage(app::fnd::Message& in_rMessage)
{
	if (PreProcessMessage(in_rMessage))
		return true;

	switch (in_rMessage.GetType())
	{
	case app::xgame::MsgHitEventCollision::MessageID:		return ProcMsgHitEventCollision(static_cast<app::xgame::MsgHitEventCollision&>(in_rMessage));
	default:												return app::ObjDroppedEgg::ProcessMessage(in_rMessage);
	}
}

bool slw_dlc_restoration::ObjDroppedEgg::ProcMsgHitEventCollision(app::xgame::MsgHitEventCollision& in_rMessage)
{
	if (auto* pTransform = GetComponent<app::fnd::GOCTransform>())
	{
		slw_dlc_restoration::egg::EggCInfo createInfo{};
		createInfo.Mtx = { pTransform->Frame.Unk3.Mtx * csl::math::Matrix34(0.0f, -3.0f, 0.0f) };
		createInfo.Type = Type;
		createInfo.PlayerNo = app::ObjUtil::GetPlayerNo(*GetDocument(), in_rMessage.Sender);

		if (createInfo.PlayerNo >= 0)
			slw_dlc_restoration::egg::CreateEgg(*GetDocument(), createInfo);

		Kill();
	}

	return true;
}

app::TTinyFsm<app::ObjDroppedEgg>::TiFsmState_t slw_dlc_restoration::ObjDroppedEgg::StateWait(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_ENTER:
	{
		ElapsedTime = 0.0f;

		return {};
	}
	case TiFSM_SIGNAL_UPDATE:
	{
		UpdateFollow();

		ElapsedTime += in_rEvent.getFloat();
		if (auto* pVisualModel = GetComponent<app::fnd::GOCVisualModel>())
		{
			pVisualModel->SetLocalScale(app::egg::CalcSlipperyScale(ElapsedTime, 1.6f, 0.1f, 0.1f));

			auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), PlayerNo);
			if (!pPlayerInfo)
				return {};

			if (auto* pTransform = GetComponent<app::fnd::GOCTransform>())
			{
				csl::math::Vector3 distance{ pPlayerInfo->Position - pTransform->Transform.Position };
				if (distance.squaredNorm() > 1000000.0f)
					Kill();
			}
		}

		return {};
	}
	default:
		return FSM_TOP();
	}
}
