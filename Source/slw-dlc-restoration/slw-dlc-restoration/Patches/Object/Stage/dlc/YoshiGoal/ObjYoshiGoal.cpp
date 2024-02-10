#include "pch.h"
#include "ObjYoshiGoal.h"

slw_dlc_restoration::ObjYoshiGoal::ObjYoshiGoal() : app::ObjYoshiGoal()
{
	FSM_INIT(static_cast<BaseState>(&ObjYoshiGoal::StateWait));
}

void slw_dlc_restoration::ObjYoshiGoal::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	if (FSM_STATE() == &app::ObjYoshiGoal::StateWaitYoshiExtrication)
		ChangeState(static_cast<BaseState>(&ObjYoshiGoal::StateWaitYoshiExtrication));

	app::ObjYoshiGoal::Update(in_rUpdateInfo);
}

app::TTinyFsm<app::ObjYoshiGoal>::TiFsmState_t slw_dlc_restoration::ObjYoshiGoal::StateWait(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_ENTER:
	{
		ElapsedTime = 0.05f;

		return {};
	}
	case TiFSM_SIGNAL_UPDATE:
	{
		UpdateSelectModel(in_rEvent.getFloat());

		return {};
	}
	case TiFSM_SIGNAL_MESSAGE:
	{
		if (in_rEvent.getMessage().GetType() != app::xgame::MsgHitEventCollision::MessageID)
			return FSM_TOP();

		uint playerNo = app::ObjUtil::GetPlayerNo(*GetDocument(), static_cast<app::xgame::MsgHitEventCollision&>(in_rEvent.getMessage()).Sender);
		if (playerNo < 0)
		{
			in_rEvent.getMessage().Handled = true;
			return{};
		}

		auto* pParam = GetAdapter()->GetData<app::SYoshiGoalParam>();

		PlayerActorID = static_cast<app::xgame::MsgHitEventCollision&>(in_rEvent.getMessage()).Sender;
		app::xgame::MsgPLHoldOn msgHold{};
		msgHold.Unk1 = 1;
		SendMessageImm(PlayerActorID, msgHold);

		csl::math::Vector3 locatorPos{};
		csl::math::Quaternion locatorRot{};
		if (app::ObjUtil::GetSetObjectTransform(*GetDocument(), { pParam->Locator }, &locatorPos, &locatorRot))
		{
			app::xgame::MsgPLJumpToTargetPosition msgJump{};
			msgJump.TargetPosition = locatorPos;
			msgJump.TargetRotation = locatorRot;
			msgJump.Unk1 = 100.0f;
			msgJump.Unk2 = 200.0f;
			msgJump.Unk3 = true;
			SendMessageImm(PlayerActorID, msgJump);
		}

		app::xgame::MsgCameraOn msgCamera{ 0.5f, false, 3001, 1, playerNo, false };
		if (app::ObjUtil::SendMessageImmToSetObject(*this, { pParam->FixCamera }, msgCamera, false))
			Camera.Value = pParam->FixCamera;

		if (auto* pEggManager = GetDocument()->GetService<slw_dlc_restoration::EggManager>())
			pEggManager->StartExtrication(playerNo);

		app::xgame::MsgStopGameTimer msgStopTimer{};
		app::ObjUtil::SendMessageImmToGameActor(*this, msgStopTimer);

		app::xgame::MsgDisableItemBag msgDisableItemBag{};
		app::ObjUtil::SendMessageImmToGameActor(*this, msgDisableItemBag);

		for (size_t i = 0; i < 3; i++)
		{
			app::xgame::MsgStopBgm msgStopBgm{};
			msgStopBgm.Unk2 = i;

			app::ObjUtil::SendMessageImmToGameActor(*this, msgStopBgm);
		}

		ChangeState(&ObjYoshiGoal::StateWaitStartRoulette);

		return {};
	}
	default:
		return FSM_TOP();
	}

	return {};
}

app::TTinyFsm<app::ObjYoshiGoal>::TiFsmState_t slw_dlc_restoration::ObjYoshiGoal::StateWaitYoshiExtrication(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_UPDATE:
	{
		auto* pEggManager = GetDocument()->GetService<slw_dlc_restoration::EggManager>();
		if (!pEggManager)
			break;

		if (!pEggManager->IsEndExtrication(app::ObjUtil::GetPlayerNo(*GetDocument(), PlayerActorID)))
			break;

		if (strcmp(GetDocument()->pGameMode->GetName(), "GameModeStageBattle") == 0)
		{
			app::SGoalRingBattleTargetParam* pBattleTargetParam{};
			for (auto& packs : m_pSetObjectManager->pActorManager->Packs)
			{
				for (auto& actor : packs)
				{
					if (strcmp(actor.GetObjectResource()->Class.GetName(), "GoalRingBattleTarget") == 0)
					{
						pBattleTargetParam = reinterpret_cast<app::SGoalRingBattleTargetParam*>(&actor.GetObjectResource()->pParam);
						break;
					}
				}
			}

			csl::math::Vector3 positionP1{ 502.8f, 103.0f, 0.0f };
			csl::math::Vector3 positionP2{ 520.2f, 103.0f, 0.0f };
			if (pBattleTargetParam)
			{
				app::ObjUtil::GetSetObjectTransform(*GetDocument(), { pBattleTargetParam->OnePTarget }, &positionP1, nullptr);
				app::ObjUtil::GetSetObjectTransform(*GetDocument(), { pBattleTargetParam->TwoPTarget }, &positionP2, nullptr);
			}

			app::xgame::MsgCameraOff msgCamera{ 1.0f, false, false, 1, (uint)app::ObjUtil::GetPlayerNo(*GetDocument(), PlayerActorID) };
			app::ObjUtil::SendMessageImmToSetObject(*this, Camera, msgCamera, false);

			app::xgame::MsgGoalForBattle msgGoal{};
			msgGoal.WinnerPlayerNo = app::ObjUtil::GetPlayerNo(*GetDocument(), PlayerActorID);
			msgGoal.TrsMatrixP1.SetTransVector(positionP1);
			msgGoal.TrsMatrixP2.SetTransVector(positionP2);

			SendMessageImmToGame(msgGoal);
		}
		else
		{
			app::xgame::MsgPlayerReachGoal msg{};
			SendMessageImm(PlayerActorID, msg);
		}

		DoCheckPlayResultBGM();

		ChangeState(&ObjYoshiGoal::StateResult);

		break;
	}
	default:
		break;
	}

	return FSM_TOP();
}