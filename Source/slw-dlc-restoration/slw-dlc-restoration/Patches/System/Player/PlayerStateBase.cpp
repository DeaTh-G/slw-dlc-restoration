#include "pch.h"
#include "PlayerStateBase.h"

bool slw_dlc_restoration::Player::CStateBase::ProcMsgTakeObject(app::Player::CStateGOC& in_rStateGoc, app::xgame::MsgTakeObject& in_rMessage)
{
	if (app::Player::StateUtil::IsDead(in_rStateGoc) || in_rStateGoc.GetBlackBoard()->Unk1[3].test(26))
		return true;

	bool isVariantValid = in_rMessage.ItemLevel != -1;
	auto* pPlugin = in_rStateGoc.GetStatePlugin<app::Player::PluginStateCheckEnableItem>();

	switch (in_rMessage.Type)
	{
	case app::xgame::MsgTakeObject::EType::eType_Ring:
	case app::xgame::MsgTakeObject::EType::eType_DroppedRing:
	{
		if (in_rMessage.IsValidUserID() && in_rMessage.UserID != 6)
			break;

		if (isVariantValid && pPlugin && pPlugin->IsItemDisable(8))
		{
			CallErrSE(in_rStateGoc, isVariantValid);
			break;
		}

		if (in_rMessage.ItemLevel == -1)
		{
			app::Player::StateUtil::AddRingNum(in_rStateGoc, 1);
			if (in_rMessage.Type == app::xgame::MsgTakeObject::EType::eType_DroppedRing)
				app::Player::StateUtil::SendMissionGetRing(in_rStateGoc, 1);

			in_rMessage.Taken = true;
			break;
		}

		in_rStateGoc.PlaySE("obj_release_ring", false);
		app::Player::StateUtil::AddRingNum(in_rStateGoc, ms_ReleaseBoxRingCount[in_rMessage.ItemLevel]);

		// Up until this point the execution is the same as the PC port's original code. The change here is that
		// PluginStateHeartLife is used to get the value of the currently available hearts and maximum number of hearts
		// to be able to send them as part of the following message to the game.
		int numHearts{};
		int maxNumHearts{};
		auto* pHeartPlugin = in_rStateGoc.GetStatePlugin<app::Player::PluginStateHeartLife>();
		if (pHeartPlugin)
		{
			numHearts = pHeartPlugin->GetNumHearts();
			maxNumHearts = pHeartPlugin->GetMaxNumHearts();
		}

		app::xgame::MsgPLSendGameInfo msg{ (app::Game::EUser)in_rStateGoc.GetPlayerNo(), in_rStateGoc.GetPhysics()->GetHorzVelocityLength(), app::Player::StateUtil::GetRingNum(in_rStateGoc), numHearts, maxNumHearts };
		in_rStateGoc.SendMessageImmToGame(msg);

		in_rMessage.Taken = true;
		break;
	}
	case app::xgame::MsgTakeObject::EType::eType_YoshiOneUp:
	{
		app::xgame::MsgExtendPlayer msg{ (app::Game::EUser)in_rStateGoc.GetToHudPlayerNumber(), 1 };
		if (in_rMessage.ItemLevel == -1)
		{
			app::Player::StateUtil::SendMission(in_rStateGoc, 2);
			msg.LifeCount = 1;
		}
		else
		{
			msg.LifeCount = ms_ReleaseBoxLivesCount[msg.LifeCount];
		}

		in_rStateGoc.SendMessageImmToGame(msg);
		in_rMessage.Taken = true;
		break;
	}
	case app::xgame::MsgTakeObject::EType::eType_YoshiCoin:
	{
		if (!in_rMessage.IsValidUserID() || in_rMessage.UserID == 6)
		{
			app::Player::StateUtil::AddRingNum(in_rStateGoc, 1);
			in_rMessage.Taken = true;
		}

		break;
	}
	case app::xgame::MsgTakeObject::EType::eType_ZeldaOneUp:
	{
		app::xgame::MsgExtendPlayer msg{ (app::Game::EUser)in_rStateGoc.GetToHudPlayerNumber(), 1 };
		msg.LifeCount = 10;

		for (size_t i = 0; i < 10; i++)
			app::Player::StateUtil::SendMission(in_rStateGoc, 2);

		in_rStateGoc.SendMessageImmToGame(msg);

		in_rMessage.Taken = true;
		break;
	}
	}

	return true;
}

bool slw_dlc_restoration::Player::CStateBase::ProcMsgPlayerReachGoal(app::Player::CStateGOC& in_rStateGoc, app::xgame::MsgPlayerReachGoal& in_rMessage)
{
	// Due to The Legend of Zelda DLC modifying the PanCamera boolean on MsgGoal the below modification has to be made
	// to the MsgGoal message sent by the game itself. This is done to avoid having to use a mid-asm hook or a complete
	// replacement of the game's already existing function, from which the latter could result in future mod incompatibility.
	if (in_rStateGoc.GetLevelInfo()->GetDLCStageIndex() != 3)
		return false;

	for (auto& pMessage : in_rStateGoc.pPlayer->pMessageManager->Messages)
		if (pMessage->GetType() == app::xgame::MsgGoal::MessageID)
			static_cast<app::xgame::MsgGoal*>(pMessage)->PanCamera = false;

	return true;
}

bool slw_dlc_restoration::Player::CStateBase::CheckHitDamage(app::Player::CStateGOC& in_rStateGoc, bool* out_pDamaged) const
{
	if (app::Player::StateUtil::IsDead(in_rStateGoc) || app::Player::StateUtil::IsDisableTime(in_rStateGoc, 3) || app::Player::StateUtil::IsInvincible(in_rStateGoc))
		return false;

	auto* pBlackboard = in_rStateGoc.GetBlackBoard();
	if (!pBlackboard->Unk1[3].test(21))
	{
		if (app::Player::StateUtil::IsBarrier(in_rStateGoc) | app::Player::StateUtil::IsHeartLife(in_rStateGoc) || (!CONFIGURATION.ZeldaTweaks.IsAlwaysHeartLife && in_rStateGoc.GetRingNum() > 0) || app::Player::StateUtil::IsNowPhantom(in_rStateGoc))
		{
			*out_pDamaged = false;
			return true;
		}
		else
		{
			app::Player::StateUtil::ReduceHeartLife(in_rStateGoc);
			*out_pDamaged = true;
			return true;
		}
	}
	else
	{
		in_rStateGoc.ChangeState(125);
		return false;
	}
}

HOOK(const bool, __fastcall, CheckHitDamageHook, ASLR(0x00894640), slw_dlc_restoration::Player::CStateBase* in_pThis, void* in_edx, app::Player::CStateGOC& in_rStateGoc, bool* out_pDamaged)
{
	return in_pThis->CheckHitDamage(in_rStateGoc, out_pDamaged);
}

HOOK(bool, __fastcall, CStateBaseProcessMessageHook, ASLR(0x008981A0), slw_dlc_restoration::Player::CStateBase* in_pThis, void* in_edx, app::Player::CStateGOC& in_rStateGoc, app::fnd::Message& in_rMessage)
{
	bool result = false;

	// Outside of the 3 restored app::xgame::MsgTakeObject types for the DLCs, eType_Ring and eType_DroppedRing require
	// modifications to be done to how their code executes due to app::Player::PluginStateHeartLife missing from the game.
	// This means that when the app::xgame::MsgPLSendGameInfo message is sent from these cases, the player's heart count and
	// maximum heart count is sent as 0 as part of the original code, while it should be taking the value from PluginStateHeartLife.
	// Not running the original function in these two cases and running our own implementation gets around this.
	if (in_rMessage.GetType() != app::xgame::MsgTakeObject::MessageID ||
		(static_cast<app::xgame::MsgTakeObject&>(in_rMessage).Type != app::xgame::MsgTakeObject::EType::eType_Ring &&
			static_cast<app::xgame::MsgTakeObject&>(in_rMessage).Type != app::xgame::MsgTakeObject::EType::eType_DroppedRing))
	{
		result = originalCStateBaseProcessMessageHook(in_pThis, in_edx, in_rStateGoc, in_rMessage);
	}
    
    switch (in_rMessage.GetType())
    {
    case app::xgame::MsgTakeObject::MessageID:						return in_pThis->ProcMsgTakeObject(in_rStateGoc, static_cast<app::xgame::MsgTakeObject&>(in_rMessage));
    case app::xgame::MsgPlayerReachGoal::MessageID:					return in_pThis->ProcMsgPlayerReachGoal(in_rStateGoc, static_cast<app::xgame::MsgPlayerReachGoal&>(in_rMessage));
	case app::xgame::MsgDlcZeldaHeartAllRecovery::MessageID:		return in_pThis->ProcMsgDlcZeldaHeartAllRecovery(in_rStateGoc, static_cast<app::xgame::MsgDlcZeldaHeartAllRecovery&>(in_rMessage));
    }

	return result;
}

void slw_dlc_restoration::Player::CStateBase::InstallHooks()
{
	// General Hooks
	INSTALL_HOOK(CStateBaseProcessMessageHook);

	// The Legend of Zelda Zone DLC Hooks
	INSTALL_HOOK(CheckHitDamageHook);
}

HOOK(bool, __fastcall, CStateSonicBaseProcessMessageHook, ASLR(0x00898960), slw_dlc_restoration::Player::CStateSonicBase* in_pThis, void* in_edx, app::Player::CStateGOC& in_rStateGoc, app::fnd::Message& in_rMessage)
{
	switch (in_rMessage.GetType())
	{
	case app::xgame::MsgDlcZeldaTakeHeart::MessageID:				return in_pThis->ProcMsgDlcZeldaTakeHeart(in_rStateGoc, static_cast<app::xgame::MsgDlcZeldaTakeHeart&>(in_rMessage));
	case app::xgame::MsgDlcZeldaTakeHeartContainer::MessageID:		return in_pThis->ProcMsgDlcZeldaTakeHeartContainer(in_rStateGoc, static_cast<app::xgame::MsgDlcZeldaTakeHeartContainer&>(in_rMessage));
	default:														return originalCStateSonicBaseProcessMessageHook(in_pThis, in_edx, in_rStateGoc, in_rMessage);
	}

	return true;
}

void slw_dlc_restoration::Player::CStateSonicBase::InstallHooks()
{
	// The Legend of Zelda Zone DLC Hooks
	INSTALL_HOOK(CStateSonicBaseProcessMessageHook);
}