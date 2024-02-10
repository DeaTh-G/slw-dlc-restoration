#include "pch.h"
#include "MoveObjGolonRock.h"

slw_dlc_restoration::game::MoveObjGolonRock::MoveObjGolonRock() : app::game::MoveObjGolonRock()
{

}

void slw_dlc_restoration::game::MoveObjGolonRock::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	auto* pContextParam = GetOwnerMovement()->GetContextParam();

	switch ((Mode)MovementMode)
	{
	case Mode::eMode_Move:
	{
		csl::math::Vector3 rotationDir{ UpdateRotDirPathToPos(in_rUpdateInfo.DeltaTime) };
		pContextParam->Position = { UpdateMovePathPos(in_rUpdateInfo.DeltaTime) + rotationDir * OffsetMagnitude };
		pContextParam->Rotation = { app::math::Matrix34OrthonormalDirection(PathEvaluator.GetTangent(PathEvaluator.Distance), { -rotationDir }) };

		Direction = { (OffsetMagnitude + YOffset) * Unk3, 0.0f, Speed * in_rUpdateInfo.DeltaTime };

		UpdateLocalRotRad(in_rUpdateInfo.DeltaTime);

		if (!IsCheckFall || !CheckFall(rotationDir, in_rUpdateInfo.DeltaTime))
		{
			if (IsPassOverPlayer() && pOwner && PassPlayerMemFunc)
				(pOwner->*PassPlayerMemFunc)();
		}
		else
		{
			if (pOwner && MoveEndMemFunc)
				(pOwner->*MoveEndMemFunc)();
		}

		break;
	}
	default:
	{
		app::game::MoveObjGolonRock::Update(in_rUpdateInfo);
		break;
	}
	}
}

bool slw_dlc_restoration::game::MoveObjGolonRock::IsPassOverPlayer() const
{
	auto* pDocument = GetOwnerMovement()->pActiveObject->GetDocument();
	int numPlayers = pDocument->GetService<app::CLevelInfo>()->GetNumPlayers();

	csl::ut::ObjectMoveArray<bool> arePlayersPassed{ pDocument->GetAllocator() };

	for (int i = 0; i < numPlayers; i++)
	{
		auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*pDocument, i);
		if (!pPlayerInfo)
		{
			arePlayersPassed.push_back(false);
			continue;
		}

		csl::math::Vector3 point{}, normal{}, tangent{};
		PathEvaluator.GetPNT(PathEvaluator.Distance, &point, &normal, &tangent);

		csl::math::Vector3 distance{ pPlayerInfo->Position - point };
		distance = { distance - normal * distance.dot(normal) };
		tangent = { tangent * csl::math::Select(Speed, fabs(1.0f), csl::math::Nabs(-1.0f)) };

		if (tangent.dot(distance) >= 0.0f)
		{
			arePlayersPassed.push_back(false);
			continue;
		}

		if (distance.squaredNorm() > 90000.0f)
		{
			arePlayersPassed.push_back(true);
			continue;
		}

		arePlayersPassed.push_back(false);
		continue;
	}

	for (auto isPassed : arePlayersPassed)
	{
		if (!isPassed)
			return false;
	}
	
	return true;
}