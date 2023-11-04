#include "pch.h"
#include "ObjEggBlock.h"

void slw_dlc_restoration::ObjEggBlock::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	float angle = IdleMotor.Unk5 * sinf(IdleMotor.Unk4 * (IdleMotor.ElapsedTime + IdleMotor.Unk1));

	switch ((EggBlockState)State)
	{
	case EggBlockState::eEggBlockState_Idle:
	{
		IdleMotor.ElapsedTime += in_rUpdateInfo.DeltaTime;

		csl::math::Quaternion rotation{ Eigen::AngleAxisf(angle, csl::math::Vector3::UnitZ()) };
		Frame.SetLocalRotation(rotation);
		if (auto* pCollider = GetComponent<app::game::GOCCollider>())
			if (auto* pShape = pCollider->GetShapeById(2))
				pShape->SetLocalRotation(rotation);

		break;
	}
	case EggBlockState::eEggBlockState_Damage:
	{
		float scalar = sinf(DamageMotor.Unk4 * (DamageMotor.ElapsedTime + DamageMotor.Unk1));
		csl::math::Vector3 position{ PopEggParameter.Direction * PopEggParameter.Unk2 * csl::math::Clamp(fabs(scalar), 0.0f, 1.0f) };
		Frame.SetLocalTranslation(position);

		DoCheckPopEgg();

		if (DamageMotor.ElapsedTime > DamageMotor.Unk3 * 0.5f)
		{
			State = EggBlockState::eEggBlockState_Idle;
			InitMotorParam(1.8f, 0.0f, 8.0f, &IdleMotor);
		}

		DamageMotor.ElapsedTime += in_rUpdateInfo.DeltaTime;

		angle = csl::math::Lerp(angle, DamageMotor.Unk5 * scalar, csl::math::Clamp(DamageMotor.ElapsedTime / ((DamageMotor.Unk3 * 0.5f) * 0.1f), 0.0f, 1.0f));

		csl::math::Quaternion rotation{ Eigen::AngleAxisf(angle, csl::math::Vector3::UnitZ()) };
		Frame.SetLocalRotation(rotation);
		if (auto* pCollider = GetComponent<app::game::GOCCollider>())
			if (auto* pShape = pCollider->GetShapeById(2))
				pShape->SetLocalRotation(rotation);

		break;
	}
	default:
	{
		csl::math::Quaternion rotation{ Eigen::AngleAxisf(angle, csl::math::Vector3::UnitZ()) };
		Frame.SetLocalRotation(rotation);
		if (auto* pCollider = GetComponent<app::game::GOCCollider>())
			if (auto* pShape = pCollider->GetShapeById(2))
				pShape->SetLocalRotation(rotation);

		break;
	}
	}
}

bool slw_dlc_restoration::ObjEggBlock::ProcessMessage(app::fnd::Message& in_rMessage)
{
	if (PreProcessMessage(in_rMessage))
		return true;

	switch (in_rMessage.GetType())
	{
	case app::xgame::MsgDamage::MessageID:		return ProcMsgDamage(static_cast<app::xgame::MsgDamage&>(in_rMessage));
	default:									return app::ObjEggBlock::ProcessMessage(in_rMessage);
	}
}

bool slw_dlc_restoration::ObjEggBlock::ProcMsgDamage(app::xgame::MsgDamage& in_rMessage)
{
	if (State == EggBlockState::eEggBlockState_Damage)
		return true;

	auto* pShape = in_rMessage.SenderShape.Get();
	if (!pShape)
		return true;

	auto* pTransform = GetComponent<app::fnd::GOCTransform>();
	if (!pTransform)
		return true;

	auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), app::ObjUtil::GetPlayerNo(*GetDocument(), in_rMessage.Sender));
	if (!pPlayerInfo)
		return true;

	csl::math::Matrix34 objectMtx = pTransform->Frame.Unk3.Mtx;
	csl::math::Matrix34Inverse(objectMtx, &objectMtx);

	csl::math::Vector3 localPosition{ objectMtx * csl::math::Vector4(pPlayerInfo->Unk15, 1.0f) };
	csl::math::Vector3 playerNormal{ objectMtx * csl::math::Vector4(pPlayerInfo->Velocity, 1.0f) };

	if (!app::math::Vector3NormalizeIfNotZero(playerNormal, &playerNormal))
		return true;

	if (pShape->GetID() == 1 && localPosition.y() > 4.5f * 2.0f &&
		playerNormal.dot(csl::math::Vector3(0.0f, -1.0f, 0.0f)) >= 0.0f &&
		app::AttackType::IsDamaged(in_rMessage.AttackType, 10))
	{
		InitMotorParam(0.8f, 0.0f, 0.0f, &DamageMotor);
		InitPopEggParam({ 0.0f, -1.0f, 0.0f }, 5.0f, 50.0f, &PopEggParameter);
	}
	else if (localPosition.y() < 0.0f &&
		playerNormal.dot(csl::math::Vector3(0.0f, 1.0f, 0.0f)) >= 0.0f &&
		(app::AttackType::And(in_rMessage.AttackType, 32) || app::AttackType::And(in_rMessage.AttackType, 64)))
	{
		if (!app::math::Vector3NormalizeIfNotZero(localPosition, &localPosition))
			return true;

		if (csl::math::Vector3(0.0f, -1.0f, 0.0f).dot(localPosition) >= MATHF_PI / 2.25)
			return true;

		csl::math::Vector3 normal = { csl::math::Vector3(0.0f, 1.0f, 0.0f).cross(localPosition) };
		if (!app::math::Vector3NormalizeIfNotZero(normal, &normal))
			return true;

		float dot = -csl::math::Vector3(0.0f, 0.0f, 1.0f).dot(normal);
		dot = csl::math::Clamp(dot, 0.5f, 0.0f);

		InitMotorParam(1.0f, dot, 30.0f, &DamageMotor);
		InitPopEggParam({ 0.0f, 1.0f, 0.0f }, 10.0f, 40.0f, &PopEggParameter);
	}
	else
	{
		return true;
	}

	if (auto* pSound = GetComponent<app::game::GOCSound>())
		pSound->Play(ms_pHitSoundName, 0.0f);

	in_rMessage.SetReply(pTransform->Frame.Unk3.GetTranslation(), false);

	State = EggBlockState::eEggBlockState_Damage;
	HitPlayerNo = app::ObjUtil::GetPlayerNo(*GetDocument(), in_rMessage.Sender);

	return true;
}

void slw_dlc_restoration::ObjEggBlock::DoCheckPopEgg()
{
	if (PopEggNum <= 1 || PopEggParameter.Unk4 || DamageMotor.ElapsedTime <= DamageMotor.Unk3 * 0.25f)
		return;

	csl::math::Quaternion frameRot = Frame.Unk3.Mtx.GetRotation();
	csl::math::Vector3 framePos = Frame.Unk3.Mtx.GetTransVector();

	csl::math::Vector3 directionOffset{ app::math::Vector3Rotate(frameRot, PopEggParameter.Direction) };
	csl::math::Vector3 verticalOffset{ app::math::Vector3Rotate(frameRot, { 0.0f, 1.0f, 0.0f }) };

	csl::math::Vector3 popPos{ framePos + verticalOffset * 4.5f + directionOffset * 4.5f };

	if (auto* pTransform = GetComponent<app::fnd::GOCTransform>())
	{
		csl::math::Quaternion objectRot = pTransform->Frame.Unk3.GetRotationQuaternion();
		float random = floorf(SonicUSA::System::Random::GetInstance()->genrand_float32() * 100.0f);

		size_t limit{};
		size_t type{};
		for (size_t i = 0; i < 4; i++)
		{
			limit += ms_EggRates[i];
			if (random > limit)
			{
				type++;
				continue;
			}

			uint randomDistance = SonicUSA::System::Random::GetInstance()->genrand_float32() * 3.0f;
			if (random >= INT_MAX - 47)
				continue;

			slw_dlc_restoration::egg::DroppedEggCInfo createInfo{};
			createInfo.Mtx = app::math::Matrix34AffineTransformation(popPos, objectRot);;
			createInfo.DropDirection = { directionOffset * (PopEggRandomAddSpeed / 3.0f * randomDistance + PopEggParameter.Unk3) };
			createInfo.PathEvaluator = PathEvaluator;
			createInfo.Type = type;
			createInfo.ZIndex = PopEggNum;
			createInfo.PlayerNo = HitPlayerNo;

			egg::CreateDroppedEgg(*GetDocument(), createInfo);

			--PopEggNum;
			PopEggParameter.Unk4 = 1;
			SetExtUserData(eExtUserDataType_High, PopEggNum);

			if (auto* pSound = GetComponent<app::game::GOCSound>())
				pSound->Play3D(ms_pEggAppearSoundName, 0.0f);

			return;
		}
	}
}