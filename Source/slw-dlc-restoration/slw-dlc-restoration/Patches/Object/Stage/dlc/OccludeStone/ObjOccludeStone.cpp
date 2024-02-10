#include "pch.h"
#include "ObjOccludeStone.h"

bool slw_dlc_restoration::ObjOccludeStone::ProcessMessage(app::fnd::Message& in_rMessage)
{
	if (PreProcessMessage(in_rMessage))
		return true;

	switch (in_rMessage.GetType())
	{
	case app::xgame::MsgDamage::MessageID:		return ProcMsgDamage(static_cast<app::xgame::MsgDamage&>(in_rMessage));
	default:									return app::ObjOccludeStone::ProcessMessage(in_rMessage);
	}
}

bool slw_dlc_restoration::ObjOccludeStone::ProcMsgDamage(app::xgame::MsgDamage& in_rMessage)
{
	bool damageByObject{};
	if (in_rMessage.DefensePower == 3 && in_rMessage.SenderType == 3)
		damageByObject = true;

	auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), app::ObjUtil::GetPlayerNo(*GetDocument(), in_rMessage.Sender));

	if ((in_rMessage.Bonus.Unk1 != 3 || !pPlayerInfo || pPlayerInfo->PixieNo != app::Game::EPhantomType::ePhantom_Bomb) && !damageByObject)
		return true;

	auto* pInfo = app::ObjUtil::GetObjectInfo<app::ObjOccludeStoneInfo>(*GetDocument());
	auto& transform = GetComponent<app::fnd::GOCTransform>()->Frame.Unk3;
	csl::math::Vector3 direction = app::math::Vector3Rotate(transform.GetRotationQuaternion(), { csl::math::Vector3::UnitY() });

	app::debris::SRandomSpaceDebrisInfo debrisInfo{};
	debrisInfo.Transform.Position = { transform.GetTranslation() + (direction * 45.0f * 0.5f) };
	debrisInfo.Transform.SetFlag(1);
	debrisInfo.Transform.Rotation = transform.GetRotationQuaternion();
	debrisInfo.Transform.SetFlag(1);
	debrisInfo.Unk1 = 400.0f;
	debrisInfo.Unk2 = 2.0f;
	debrisInfo.Unk3 = 0.5f;
	debrisInfo.Unk4.set(0);
	debrisInfo.Unk5 = in_rMessage.Unk2;
	debrisInfo.Unk6 = direction;
	debrisInfo.Unk7 = 2.0f;
	debrisInfo.Unk8 = 1.0f;
	debrisInfo.Unk4.set(2);
	debrisInfo.Unk10 = 0.0f;
	debrisInfo.Unk11 = 10.0f;
	debrisInfo.Unk4.set(1);
	debrisInfo.Unk9 = 0.5f;
	debrisInfo.SetModelRes(pInfo->Debris, 24);
	debrisInfo.Unk14 = 3;
	debrisInfo.Unk15 = 22.5f;
	debrisInfo.Unk16 = 25.0f;
	debrisInfo.Unk17 = 15.0f;
	app::debrisUtil::CreateRandomSpaceDebris(*GetDocument(), debrisInfo);

	GetComponent<app::game::GOCEffect>()->CreateEffect(ms_pEffectName);
	GetComponent<app::game::GOCSound>()->Play(ms_pPuzzleClearSoundName, 0.0f);
	GetComponent<app::game::GOCSound>()->Play3D(ms_pRockBreakSoundName, 0.0f);
	SetStatusRetire();
	Kill();

	return true;
}