#include "pch.h"
#include "EnemyStalBaby.h"

slw_dlc_restoration::EnemyStalBaby::EnemyStalBaby() : app::EnemyStalBaby()
{
	States[7] = { "Dead", (app::ut::internal::StateCreate_t)&app::ut::StateCreatorFuncTemplate<EnemyStalBaby::State::Dead>::Create, -1 };
}

bool slw_dlc_restoration::EnemyStalBaby::ProcessMessage(app::fnd::Message& in_rMessage)
{
	if (PreProcessMessage(in_rMessage))
		return true;

	switch (in_rMessage.GetType())
	{
	case app::xgame::MsgDamage::MessageID:		return ProcMsgDamage(static_cast<app::xgame::MsgDamage&>(in_rMessage));
	case app::xgame::MsgKick::MessageID:		return ProcMsgKick(static_cast<app::xgame::MsgKick&>(in_rMessage));
	default:									return app::EnemyStalBaby::ProcessMessage(in_rMessage);
	}
}

bool slw_dlc_restoration::EnemyStalBaby::ProcMsgDamage(app::xgame::MsgDamage& in_rMessage)
{
	bool result = app::EnemyStalBaby::ProcMsgDamage(in_rMessage);

	if ((app::EnemyUtil::IsFatalDamage(in_rMessage) || GetComponent<app::GOCEnemyHsm>()->GetCurrentStateID() == 5) || in_rMessage.SenderType == 3)
		KillingPlayerNo = in_rMessage.PlayerNo;

	return result;
}

bool slw_dlc_restoration::EnemyStalBaby::ProcMsgKick(app::xgame::MsgKick& in_rMessage)
{
	bool result = app::EnemyStalBaby::ProcMsgKick(in_rMessage);

	KillingPlayerNo = in_rMessage.PlayerNo;

	return result;
}

bool slw_dlc_restoration::EnemyStalBaby::State::Dead::Step(context_type& in_rObj, float in_deltaTime)
{
	auto& object = static_cast<EnemyStalBaby&>(in_rObj);

	if (!IsDeadNoHead)
	{
		if (pAnimationGoc->IsFinished())
			object.OnDead();

		return false;
	}

	app::math::Transform transform{};
	if (!object.GetComponent<app::fnd::GOCVisualModel>()->GetNodeTransform(0, ms_pHeadNodeName, &transform))
		return false;

	auto* pInfo = app::ObjUtil::GetObjectInfo<app::EnemyStalBabyInfo>(*object.GetDocument());

	app::EnemyBlowOffObjectCInfo createInfo{};
	createInfo.Model = pInfo->HeadModel;
	createInfo.TrsMatrix = transform.GetTransformMatrix();
	createInfo.Unk6 = object.KickPosition;
	createInfo.CollisionRadius = 4.0f;
	createInfo.PlayerNo = object.KillingPlayerNo;
	createInfo.Size = 5;
	object.CreateEnemyBlowOffObject(createInfo);

	IsDeadNoHead = false;

	return false;
}