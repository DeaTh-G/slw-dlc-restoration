#include "pch.h"
#include "ObjZeldaGoal.h"

void slw_dlc_restoration::ObjZeldaGoal::AddCallback(app::GameDocument* in_pDocument)
{
	app::ObjZeldaGoal::AddCallback(in_pDocument);

	if (!CONFIGURATION.ZeldaTweaks.ScaleObjectsWithPlayer)
		return;

	auto* pPlayer = static_cast<app::Player::CPlayer*>(pMessageManager->GetActor(app::ObjUtil::GetPlayerActorID(*in_pDocument, 0)));
	auto playerScale = pPlayer->GetPlayerGOC<app::Player::CVisualGOC>()->GetHumanVisual()->pGocHolder->GetUnit(0).rpModel->Transform.Scale;
	
	GetComponent<app::fnd::GOCVisualModel>()->SetLocalScale(playerScale);
}

void slw_dlc_restoration::ObjZeldaGoal::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	if (FSM_STATE() == &app::ObjZeldaGoal::StateFadeIn)
		ChangeState(static_cast<BaseState>(&ObjZeldaGoal::StateFadeIn));

	app::ObjZeldaGoal::Update(in_rUpdateInfo);
}

slw_dlc_restoration::ObjZeldaGoal::TiFsmState_t slw_dlc_restoration::ObjZeldaGoal::StateFadeIn(const TiFsmEvent_t& in_rEvent)
{
	if (in_rEvent.getSignal() != TiFSM_SIGNAL_UPDATE)
		return app::ObjZeldaGoal::StateFadeIn(in_rEvent);

	if (!app::CFadeInOutManager::GetInstance()->IsFinished(3))
		return {};

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

		csl::math::Vector3 positionP1{ 398.265f, 304.489f, -858.605f };
		csl::math::Vector3 positionP2{ 397.58f, 304.94f, -858.991f };
		if (pBattleTargetParam)
		{
			app::ObjUtil::GetSetObjectTransform(*GetDocument(), { pBattleTargetParam->OnePTarget }, &positionP1, nullptr);
			app::ObjUtil::GetSetObjectTransform(*GetDocument(), { pBattleTargetParam->TwoPTarget }, &positionP2, nullptr);
		}

		app::xgame::MsgGoalForBattle msgGoal{};
		msgGoal.WinnerPlayerNo = PlayerNo;
		msgGoal.TrsMatrixP1.SetTransVector(positionP1);
		msgGoal.TrsMatrixP2.SetTransVector(positionP2);

		SendMessageImmToGame(msgGoal);
	}
	else
	{
		app::xgame::MsgPlayerReachGoal msg{};
		app::ObjUtil::SendMessageImmToPlayer(*this, PlayerNo, msg);
	}

	GetComponent<app::game::GOCAnimationScript>()->ChangeAnimation(ms_pResultAnimationName);

	if (CONFIGURATION.ZeldaTweaks.UseUnusedGoalAnimation || (strcmp(GetDocument()->pGameMode->GetName(), "GameModeStageBattle") == 0 && CONFIGURATION.GlobalTweaks.FixMultiplayerBugs))
		ChangeState(&ObjZeldaGoal::StateEndUp);
	else
		ChangeState(&ObjZeldaGoal::StateIdle);

	return {};
}