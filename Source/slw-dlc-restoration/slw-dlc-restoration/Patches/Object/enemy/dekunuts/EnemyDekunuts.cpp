#include "pch.h"
#include "EnemyDekunuts.h"

slw_dlc_restoration::EnemyDekunuts::EnemyDekunuts() : app::EnemyDekunuts()
{
	States[1] = { "Appear", (app::ut::internal::StateCreate_t)&app::ut::StateCreatorFuncTemplate<EnemyDekunuts::State::Appear>::Create, -1 };
	States[3] = { "Attack", (app::ut::internal::StateCreate_t)&app::ut::StateCreatorFuncTemplate<EnemyDekunuts::State::Attack>::Create, -1 };
}

bool slw_dlc_restoration::EnemyDekunuts::ProcessMessage(app::fnd::Message& in_rMessage)
{
	if (PreProcessMessage(in_rMessage))
		return true;

	switch (in_rMessage.GetType())
	{
	case app::xgame::MsgLeaveEventCollision::MessageID:		return ProcMsgLeaveEventCollision(static_cast<app::xgame::MsgLeaveEventCollision&>(in_rMessage));
	case app::xgame::MsgStayTrigger::MessageID:				return ProcMsgStayTrigger(static_cast<app::xgame::MsgStayTrigger&>(in_rMessage));
	default:												return app::EnemyDekunuts::ProcessMessage(in_rMessage);
	}
}

bool slw_dlc_restoration::EnemyDekunuts::ProcMsgLeaveEventCollision(app::xgame::MsgLeaveEventCollision& in_rMessage)
{
	if (app::ObjUtil::CheckShapeUserID(in_rMessage.Self, 0) && PlayerNo == app::ObjUtil::GetPlayerNo(*GetDocument(), in_rMessage.Sender))
		PlayerNo = static_cast<uint>(-1);

	return app::EnemyDekunuts::ProcMsgLeaveEventCollision(in_rMessage);
}

bool slw_dlc_restoration::EnemyDekunuts::ProcMsgStayTrigger(app::xgame::MsgStayTrigger& in_rMessage)
{
	if (app::ObjUtil::CheckShapeUserID(in_rMessage.Unk1, 0) && PlayerNo == static_cast<uint>(-1))
		PlayerNo = app::ObjUtil::GetPlayerNo(*GetDocument(), in_rMessage.Sender);

	return app::EnemyDekunuts::ProcMsgStayTrigger(in_rMessage);
}

void slw_dlc_restoration::EnemyDekunuts::RotateTarget(float in_unk1, float in_unk2)
{
	auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), PlayerNo);
	if (!pPlayerInfo)
		return;

	csl::math::Vector3 playerDirection{ LookAtMatrix * csl::math::Vector4(pPlayerInfo->Unk15, 1.0f) };
	playerDirection = { playerDirection - csl::math::Vector3(csl::math::Vector3::UnitY() * playerDirection.dot(csl::math::Vector3::UnitY())) };
	if (!app::math::Vector3NormalizeIfNotZero(playerDirection, &playerDirection))
		return;

	csl::math::Vector3 forwardVec{ app::math::Vector3RotateY(RotationAngle, { csl::math::Vector3::UnitZ() }) };
	float distance = acosf(csl::math::Clamp(forwardVec.dot(playerDirection), -1.0f, 1.0f));
	if (in_unk1 > 0.0f)
		distance = csl::math::Min(distance, in_unk1);

	if (in_unk2 > 1.0f)
		distance *= in_unk2;

	RotationAngle += csl::math::Select(csl::math::Vector3::UnitY().cross(forwardVec).dot(playerDirection), fabs(distance), csl::math::Nabs(fabs(distance)));
	Frame.SetLocalRotation({ Eigen::AngleAxisf(RotationAngle, Eigen::Vector3f::UnitY()) });
}

bool slw_dlc_restoration::EnemyDekunuts::State::Appear::Step(context_type& in_rObj, float in_deltaTime)
{
	auto& object = static_cast<EnemyDekunuts&>(in_rObj);
	
	ElapsedTime += in_deltaTime;
	object.RotateTarget(-1.0f, csl::math::Clamp(ElapsedTime * 2.0f, 0.0f, 1.0f));

	if (ElapsedTime > 0.5f)
		ChangeState(object, 3);

	return true;
}

bool slw_dlc_restoration::EnemyDekunuts::State::Attack::Step(context_type& in_rObj, float in_deltaTime)
{
	auto& object = static_cast<EnemyDekunuts&>(in_rObj);

	if (object.Flags.test(FLAG_IS_DAMAGE_RECEIVED))
	{
		ChangeState(object, 4);
		return false;
	}
	else if (object.Flags.test(FLAG_IS_READY_TO_ATTACK))
	{
		object.RotateTarget(MATHF_PI / 1.125 * in_deltaTime, -1.0f);

		auto* pEnemyTargetGoc = object.GetComponent<app::GOCEnemyTarget>();
		if (pEnemyTargetGoc->IsFindTarget() && !object.IsCurrentAnimation(5))
		{
			RemainingTime -= in_deltaTime;
			if (RemainingTime < 0.0f)
			{
				object.SetAnimation(5);
				RemainingTime = 2.0f;
			}
		}

		return true;
	}
	else
	{
		ChangeState(object, 2);
		return true;
	}

	return true;
}