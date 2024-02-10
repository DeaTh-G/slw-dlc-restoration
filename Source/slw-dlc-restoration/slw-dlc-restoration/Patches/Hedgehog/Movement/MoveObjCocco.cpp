#include "pch.h"
#include "MoveObjCocco.h"

slw_dlc_restoration::MoveObjCocco::MoveObjCocco() : app::MoveObjCocco()
{

}

void slw_dlc_restoration::MoveObjCocco::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	if (Type == MoveType::eMoveType_None)
		return;

	auto* pMovementGoc = GetOwnerMovement();
	auto* pContextParam = pMovementGoc->GetContextParam();

	csl::math::Vector3 position{ pContextParam->Position };
	csl::math::Vector3 upVector{ app::math::Vector3Rotate(pContextParam->Rotation, { csl::math::Vector3::UnitY() }) };
	csl::math::Vector3 leftVector{ app::math::Vector3Rotate(pContextParam->Rotation, { csl::math::Vector3::UnitZ() }) };
	csl::math::Vector3 target{};
	csl::math::Vector3 destination{};
	csl::math::Vector3 downVector{};

	switch ((MoveType)Type)
	{
	case MoveType::eMoveType_TargetPoint:
	{
		target = { TargetPosition };
		destination = { target - pContextParam->Position };

		break;
	}
	case MoveType::eMoveType_RelativeTargetPoint:
	{
		if (auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*pMovementGoc->pActiveObject->GetDocument(), PlayerNo))
		{
			target = { pPlayerInfo->Position + TargetPosition };
			destination = { target - pContextParam->Position };
		}

		break;
	}
	case MoveType::eMoveType_TargetDirection:
	case MoveType::eMoveType_TargetDirectionJump:
	{
		target = { pContextParam->Position };
		destination = { TargetPosition };

		break;
	}
	case MoveType::eMoveType_TargetPlayer:
	{
		target = { pContextParam->Position };

		auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*pMovementGoc->pActiveObject->GetDocument(), PlayerNo);
		if (!pPlayerInfo)
		{
			destination = { TargetPosition };
			break;
		}

		if (pContextParam->Velocity.squaredNorm() <= 40000.0f)
		{
			destination = { pPlayerInfo->Position - pContextParam->Position };
			break;
		}
		else
		{
			csl::math::Vector3 playerVelocity{};
			app::math::Vector3Normalize(pPlayerInfo->Velocity, &playerVelocity);
			destination = { pPlayerInfo->Position + playerVelocity * 100.0f - pContextParam->Position };

			break;
		}

		break;
	}
	case MoveType::eMoveType_None:
	default:
	{
		break;
	}
	}

	destination -= csl::math::Vector3(upVector * destination.dot(upVector));
	app::math::Vector3NormalizeZero(destination, &destination);

	float scalar = fabs(acosf(csl::math::Clamp(destination.dot(leftVector), -1.0f, 1.0f)));
	if (scalar > 0.000001f)
	{
		float angle = MATHF_PI * 4.0f * in_rUpdateInfo.DeltaTime;
		if (angle >= scalar)
		{
			if (Type == MoveType::eMoveType_TargetPlayer)
			{
				TargetPosition = destination;
				Type = MoveType::eMoveType_TargetDirection;
			}
		}
		else
		{
			csl::math::Vector3 frontVector{ app::math::Vector3Rotate(pContextParam->Rotation, { csl::math::Vector3::UnitX() }) };
			destination = app::math::Vector3Rotate({ Eigen::AngleAxisf(csl::math::Select(frontVector.dot(destination), fabs(angle), csl::math::Nabs(angle)), upVector) }, leftVector);
		}
	}

	csl::math::Vector3 velocity{ upVector * pContextParam->Velocity.dot(upVector) };

	if (CurrentState == State::eState_Unk1)
	{
		if (Type != MoveType::eMoveType_RelativeTargetPoint)
			downVector = { -upVector * 100.0f };
		else
			downVector = { -upVector * 2.0f };
	}

	pContextParam->Velocity = { destination * Speed + velocity + downVector * in_rUpdateInfo.DeltaTime };
	pContextParam->Position += csl::math::Vector3(pContextParam->Velocity * in_rUpdateInfo.DeltaTime);
	TargetDirection = destination;

	csl::math::Vector3 normalizedDir{ TargetDirection - upVector * TargetDirection.dot(upVector) };
	app::math::Vector3NormalizeZero(normalizedDir, &normalizedDir);

	csl::math::Vector3 front{ upVector.cross(normalizedDir) };
	csl::math::Vector3 left{ front.cross(upVector) };

	app::math::Vector3NormalizeZero(front, &front);
	csl::math::Matrix34 rotationMtx{};
	rotationMtx.SetColumn(0, front);
	rotationMtx.SetColumn(1, upVector);
	rotationMtx.SetColumn(2, left);
	pContextParam->Rotation = { rotationMtx };

	switch ((State)CurrentState)
	{
	case State::eState_Unk0:
	{
		csl::math::Vector3 from{ pContextParam->Position + upVector * 5.0f };
		csl::math::Vector3 to{ pContextParam->Position - upVector * 0.5f };

		app::game::PhysicsRaycastOutput output{};
		if (app::ObjUtil::RaycastNearestCollision(&output, *pMovementGoc->pActiveObject->GetDocument(), from, to, 0xC996))
		{
			if ((output.Attribute & 0x10) == 0)
			{
				pContextParam->Velocity -= csl::math::Vector3(upVector * pContextParam->Velocity.dot(upVector));
				pContextParam->Position = output.HitPoint;
			}
		}
		else
		{
			CurrentState = State::eState_Unk1;
		}

		break;
	}
	case State::eState_Unk1:
	{
		csl::math::Vector3 to{ pContextParam->Position };

		csl::math::Vector3 offsetPosition{ position - pContextParam->Position };
		float length = app::math::Vector3NormalizeWithLength(offsetPosition, &offsetPosition);

		csl::math::Vector3 from{ pContextParam->Position + offsetPosition * (length + 0.1f) };

		app::game::PhysicsRaycastOutput output{};
		if (app::ObjUtil::RaycastNearestCollision(&output, *pMovementGoc->pActiveObject->GetDocument(), from, to, 0xC996)
			&& (output.Attribute & 0x10) == 0)
		{
			CurrentState = State::eState_Unk0;
			pContextParam->Position = output.HitPoint;
		}

		break;
	}
	default:
	{
		break;
	}
	}

	if (Type == MoveType::eMoveType_None)
		return;

	if (Type <= MoveType::eMoveType_RelativeTargetPoint)
	{
		csl::math::Vector3 distance{ target - position };
		distance -= csl::math::Vector3(upVector * distance.dot(upVector));

		csl::math::Vector3 newDistance{ target - pContextParam->Position };
		newDistance -= csl::math::Vector3(upVector * newDistance.dot(upVector));

		if (distance.dot(newDistance) <= 0.0f || newDistance.norm() <= 5.0f)
		{
			pMovementGoc->DisableMovementFlag(0);
			if (Callback.pOwner && Callback.NotifyStopMemFunc)
				(Callback.pOwner->*Callback.NotifyStopMemFunc)();
		}
	}
	else if (Type == MoveType::eMoveType_TargetDirectionJump && CurrentState == State::eState_Unk0)
	{
		pMovementGoc->DisableMovementFlag(0);
		if (Callback.pOwner && Callback.NotifyStopMemFunc)
			(Callback.pOwner->*Callback.NotifyStopMemFunc)();
	}
}

void slw_dlc_restoration::MoveObjCocco::SetRelativeTargetPoint(const csl::math::Vector3& in_rTargetPoint, float in_speed, uint in_playerNo)
{
	app::MoveObjCocco::SetRelativeTargetPoint(in_rTargetPoint, in_speed);

	PlayerNo = in_playerNo;
}

void slw_dlc_restoration::MoveObjCocco::SetTargetPlayer(float in_speed, float in_velocity, uint in_playerNo)
{
	auto* pMovementGoc = GetOwnerMovement();
	auto* pContextParam = pMovementGoc->GetContextParam();

	csl::math::Vector3 direction{ app::math::Vector3Rotate(pContextParam->Rotation, {csl::math::Vector3::UnitY()}) };

	auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*pMovementGoc->pActiveObject->GetDocument(), in_playerNo);
	if (!pPlayerInfo)
		return;

	csl::math::Vector3 offset{ pPlayerInfo->Position - pContextParam->Position };
	offset -= csl::math::Vector3(direction * offset.dot(direction));
	app::math::Vector3NormalizeZero(offset, &offset);

	Type = MoveType::eMoveType_TargetPlayer;
	TargetPosition = offset;
	Speed = in_speed;
	CurrentState = State::eState_Unk1;

	PlayerNo = in_playerNo;

	pContextParam->Velocity = { 0.0f, in_velocity, 0.0f };
	pMovementGoc->EnableMovementFlag(0);
}