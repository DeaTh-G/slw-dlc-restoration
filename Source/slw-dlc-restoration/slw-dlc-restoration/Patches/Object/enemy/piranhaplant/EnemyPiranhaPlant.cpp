#include "pch.h"
#include "EnemyPiranhaPlant.h"

bool slw_dlc_restoration::EnemyPiranhaPlant::ProcessMessage(app::fnd::Message& in_rMessage)
{
	if (PreProcessMessage(in_rMessage))
		return true;

	switch (in_rMessage.GetType())
	{
	case app::xgame::MsgDamage::MessageID:		return ProcMsgDamage(static_cast<app::xgame::MsgDamage&>(in_rMessage));
	default:									return app::EnemyPiranhaPlant::ProcessMessage(in_rMessage);
	}
}

bool slw_dlc_restoration::EnemyPiranhaPlant::ProcMsgDamage(app::xgame::MsgDamage& in_rMessage)
{
	if (in_rMessage.SenderType == 3)
	{
		GetComponent<app::GOCEnemyHsm>()->Dispatch(in_rMessage);
	}
	else if (in_rMessage.Bonus.Unk1 == 3)
	{
		auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), app::ObjUtil::GetPlayerNo(*GetDocument(), in_rMessage.PlayerNo));
		if (!pPlayerInfo || pPlayerInfo->PixieNo == app::Game::EPhantomType::ePhantom_Bomb)
			return false;

		GetComponent<app::GOCEnemyHsm>()->Dispatch(in_rMessage);
	}

	return true;
}