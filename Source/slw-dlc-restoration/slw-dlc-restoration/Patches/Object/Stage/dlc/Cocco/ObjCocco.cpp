#include "pch.h"
#include "ObjCocco.h"

slw_dlc_restoration::ObjCocco::ObjCocco() : app::ObjCocco()
{
	FSM_INIT(static_cast<BaseState>(&ObjCocco::StateIdle));
}

slw_dlc_restoration::ObjCocco::ObjCocco(const CInfo& in_rCreateInfo) : app::ObjCocco(static_cast<const app::ObjCocco::CInfo&>(in_rCreateInfo)), PlayerNo(in_rCreateInfo.PlayerNo)
{
	FSM_INIT(static_cast<BaseState>(&ObjCocco::StateAttackIn));
}

void slw_dlc_restoration::ObjCocco::AddCallback(app::GameDocument* in_pDocument)
{
	app::fnd::GOComponent::Create<app::fnd::GOCVisualModel>(*this);
	app::fnd::GOComponent::Create<app::game::GOCAnimationScript>(*this);
	app::fnd::GOComponent::Create<app::game::GOCCollider>(*this);
	app::fnd::GOComponent::Create<app::game::GOCEffect>(*this);
	app::fnd::GOComponent::Create<app::game::GOCSound>(*this);
	app::fnd::GOComponent::Create<app::game::GOCShadowSimple>(*this);
	app::fnd::GOComponent::Create<app::game::GOCMovementComplex>(*this);
	
	app::fnd::GOComponent::BeginSetup(*this);

	GetParentObject();

	MoveRange = GetSpawner()->MoveRange;

	auto* pInfo = app::ObjUtil::GetObjectInfo<app::ObjCoccoInfo>(*in_pDocument);

	if (Type == ActionType::eActionType_Attack)
		GetComponent<app::fnd::GOCTransform>()->SetLocalTranslationAndRotation(CreateInfo.Position, CreateInfo.Rotation);

	if (auto* pVisualGoc = GetComponent<app::fnd::GOCVisualModel>())
	{
		app::fnd::GOCVisualModel::Description description{};
		description.Model = pInfo->Model;
		description.Skeleton = pInfo->Skeleton;
		description.Unk2 |= 0x400000;

		pVisualGoc->Setup(description);

		if (auto* pAnimationGoc = GetComponent<app::game::GOCAnimationScript>())
		{
			pAnimationGoc->Setup({ &pInfo->AnimationContainer });
			pVisualGoc->AttachAnimation(pAnimationGoc);
			pAnimationGoc->SetAnimation(ms_pIdleAnimationName);

			pAnimationGoc->RegisterCallback(0, app::animation::CreateAnimCallback<ObjCocco>(this, &app::ObjCocco::SoundCallback, GetAllocator()));
		}
	}

	if (auto* pColliderGoc = GetComponent<app::game::GOCCollider>())
	{
		pColliderGoc->Setup({ ms_ShapeCount });
		app::game::ColliSphereShapeCInfo collisionInfo{};
		collisionInfo.ShapeType = app::game::CollisionShapeType::ShapeType::eShapeType_Sphere;
		collisionInfo.MotionType = app::game::PhysicsMotionType::MotionType::eMotionType_Value2;
		collisionInfo.Unk2 = 1;
		collisionInfo.Radius = ms_CollisionRadius;
		app::ObjUtil::SetupCollisionFilter(app::ObjUtil::eFilter_Default, collisionInfo);
		if (Type == ActionType::eActionType_Idle)
			collisionInfo.Unk3 = 0x20000;

		collisionInfo.SetLocalPosition(ms_CollisionOffset);
		pColliderGoc->CreateShape(collisionInfo);
	}

	app::game::GOCSound::SimpleSetup(this, 0, 0);
	if (GetComponent<app::game::GOCEffect>())
		app::game::GOCEffect::SimpleSetup(this, 1, true);

	if (auto* pShadowGoc = GetComponent<app::game::GOCShadowSimple>())
	{
		app::game::ShadowSphereShapeCInfo shadowInfo{ ms_ShadowRadius };
		shadowInfo.Unk2 = 1;
		shadowInfo.ShadowQuality = 2;

		app::game::GOCShadowSimple::Description description{ &shadowInfo };
		pShadowGoc->Setup(description);
		pShadowGoc->SetLocalOffsetPosition(ms_ShadowOffset);
	}

	if (auto* pMovementGoc = GetComponent<app::game::GOCMovementComplex>())
	{
		if (pMovementController = pMovementGoc->SetMoveController<slw_dlc_restoration::MoveObjCocco>())
		{
			pMovementController->SetNotifyStopCallback(&ObjCocco::NotifyStopCallback, this);
		}
	}

	if (GetExtUserData(CSetObjectListener::EExtUserDataType::eExtUserDataType_High) == 1)
	{
		GetComponent<app::fnd::GOCVisualModel>()->SetVisible(false);
		GetComponent<app::game::GOCShadowSimple>()->SetVisible(false);
		GetComponent<app::game::GOCCollider>()->SetEnable(false);
		Sleep();
	}

	SetEnableAttack(true);

	app::fnd::GOComponent::EndSetup(*this);

	InitFSM();
}

void slw_dlc_restoration::ObjCocco::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	if (FSM_STATE() == &app::ObjCocco::StateIdle)
		ChangeState(static_cast<BaseState>(&ObjCocco::StateIdle));
	else if (FSM_STATE() == &app::ObjCocco::StateAttackIn)
		ChangeState(static_cast<BaseState>(&ObjCocco::StateAttackIn));
	else if (FSM_STATE() == &app::ObjCocco::StateAttackOut)
		ChangeState(static_cast<BaseState>(&ObjCocco::StateAttackOut));

	DispatchFSM(TiFsmEvent_t::CreateUpdate(in_rUpdateInfo.DeltaTime));

	if (!Flags.test(1) || Type != ActionType::eActionType_Idle)
		return;

	float time = ElapsedTime;
	ElapsedTime += in_rUpdateInfo.DeltaTime;
	if (ElapsedTime > 7.0f)
	{
		SetStatusRetire();
		Kill();
	}

	for (auto& cocco : SubCoccos)
		if (cocco.IsValid())
			SubCoccos.erase(&cocco);

	if ((int)(time / 0.7f) < (int)(ElapsedTime / 0.7f))
		CreateAttackers();
}

app::TTinyFsm<app::ObjCocco>::TiFsmState_t slw_dlc_restoration::ObjCocco::StateIdle(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_MESSAGE:
	{
		switch (in_rEvent.getMessage().GetType())
		{
		case app::xgame::MsgDamage::MessageID:
		{
			app::xgame::MsgDamage& message = static_cast<app::xgame::MsgDamage&>(in_rEvent.getMessage());

			DamageJump(message.Unk3);
			HealthPoint -= message.Damage;
			PlayerNo = message.PlayerNo;

			message.Handled = true;

			break;
		}
		case app::xgame::MsgKick::MessageID:
		{
			app::xgame::MsgKick& message = static_cast<app::xgame::MsgKick&>(in_rEvent.getMessage());

			DamageJump(message.Unk3);
			HealthPoint--;
			PlayerNo = message.PlayerNo;

			message.Handled = true;

			break;
		}
		default:
			break;
		}

		return {};
	}
	default:
		break;
	}

	return app::ObjCocco::StateIdle(in_rEvent);
}

app::TTinyFsm<app::ObjCocco>::TiFsmState_t slw_dlc_restoration::ObjCocco::StateAttackIn(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_ENTER:
	{
		SetTargetPlayer();
		Flags.set(1);
		GetComponent<app::game::GOCAnimationScript>()->ChangeAnimation(ms_pFlyAnimationName);

		return {};
	}
	case TiFSM_SIGNAL_UPDATE:
	{
		auto* pAnimationGoc = GetComponent<app::game::GOCAnimationScript>();

		if (CryTime > 0.0f)
		{
			CryTime -= in_rEvent.getFloat();
		}
		else
		{
			GetComponent<app::game::GOCSound>()->Play3D(ms_pAttackSoundName, 0.0f);
			CryTime += 2.0f;
		}

		if (Type == ActionType::eActionType_Idle)
		{
			auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), PlayerNo);
			if (pPlayerInfo)
			{
				if (fabs(pPlayerInfo->Position.z() - GetComponent<app::fnd::GOCTransform>()->Frame.Unk3.GetTranslation().z() > 100.0f))
					Kill();
			}
		}

		if (Flags.test(0))
		{
			csl::math::Matrix34 transformMtx{ GetComponent<app::fnd::GOCTransform>()->Frame.Unk3.Mtx };

			Flags.reset(0);

			csl::math::Vector3 upVector{ transformMtx.GetColumn(1) };
			csl::math::Vector3 from{ transformMtx.GetTransVector() + upVector * 5.0f };
			csl::math::Vector3 to{ from + csl::math::Vector3(-from * 50.0f) };

			app::game::PhysicsRaycastOutput output{};
			if (app::ObjUtil::RaycastNearestCollision(&output, *GetDocument(), from, to, 0xC996))
			{
				pAnimationGoc->ChangeAnimation(ms_pAttack2AnimationName);
				ChangeState(static_cast<BaseState>(&ObjCocco::StateAttackOut));

				return {};
			}

			pMovementController->SetTargetDirection(transformMtx.GetColumn(2), 70.0f);
		}

		return {};
	}
	default:
		break;
	}

	return app::ObjCocco::StateAttackIn(in_rEvent);
}

app::TTinyFsm<app::ObjCocco>::TiFsmState_t slw_dlc_restoration::ObjCocco::StateAttackOut(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_ENTER:
	{
		auto* pMoveController = static_cast<slw_dlc_restoration::MoveObjCocco*>(pMovementController);

		if (auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), PlayerNo))
		{
			if (pow(150.0f, 2.0f) >= pPlayerInfo->Velocity.squaredNorm())
				pMoveController->SetTargetPlayer(70.0f, 20.0f, PlayerNo);
			else
				pMoveController->SetTargetPlayer(220.0f, 20.0f, PlayerNo);
		}

		if (Flags.test(2))
		{
			Flags.set(1);

			if (Type == ActionType::eActionType_Idle)
			{
				auto* pParam = GetSpawner();
				for (auto coccoId : pParam->CoccoList)
				{
					if (auto* pCocco = static_cast<ObjCocco*>(app::ObjUtil::GetGameObjectHandle(*GetDocument(), { coccoId }).Get()))
						pCocco->SetEnableAttack(false);
				}
			}
		}

		RunAwayTime = 0.0f;

		return {};
	}
	case TiFSM_SIGNAL_UPDATE:
	{
		auto* pAnimationGoc = GetComponent<app::game::GOCAnimationScript>();

		RunAwayTime += in_rEvent.getFloat();

		if (CryTime > 0.0f)
		{
			CryTime -= in_rEvent.getFloat();
		}
		else
		{
			GetComponent<app::game::GOCSound>()->Play3D(ms_pAttackSoundName, 0.0f);
			CryTime += 2.0f;
		}

		if (!Flags.test(0))
		{
			if (RunAwayTime <= 0.5f || IsInCamera())
				return {};
		}

		Flags.reset(0);

		if (Type != ActionType::eActionType_Idle)
			Kill();
		else
			ChangeState(&ObjCocco::StateEnd);

		return {};
	}
	default:
		break;
	}

	return app::ObjCocco::StateAttackOut(in_rEvent);
}

void slw_dlc_restoration::ObjCocco::SetTargetPlayer()
{
	auto* pMoveController = static_cast<slw_dlc_restoration::MoveObjCocco*>(pMovementController);

	csl::math::Matrix34 transformMtx{ GetComponent<app::fnd::GOCTransform>()->Frame.Unk3.Mtx };
	csl::math::Vector3 upVector{ transformMtx.GetColumn(1) };
	csl::math::Vector3 frontVector{ transformMtx.GetColumn(2) };

	float angle = SonicUSA::System::Random::GetInstance()->genrand_float32() * MATHF_PI / 4.5f + (MATHF_PI / 6.0f);
	csl::math::Quaternion direction{};
	if (CreateInfo.Unk1)
	{
		if ((CreateInfo.Unk1 & 1) == 0)
			direction = { Eigen::AngleAxisf(angle, upVector) };
	}
	else
	{
		float random = powf(SonicUSA::System::Random::GetInstance()->genrand_float32(), 2.0f);
		if (random >= 0.0f)
		{
			direction = { Eigen::AngleAxisf(angle, upVector) };
		}
		else
		{
			if (!(int)random)
				direction = { Eigen::AngleAxisf(angle, upVector) };
		}
	}

	direction = { Eigen::AngleAxisf(-angle, upVector) };

	csl::math::Vector3 targetDirection{ app::math::Vector3Rotate(direction, csl::math::Vector3(frontVector * (SonicUSA::System::Random::GetInstance()->genrand_float32() * 30.0f + 20.0f))) };

	auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), PlayerNo);
	if (!pPlayerInfo)
		return;

	if (powf(150.0f, 2.0f) >= pPlayerInfo->Velocity.squaredNorm())
	{
		if (powf(50.0f, 2.0f) >= pPlayerInfo->Velocity.squaredNorm())
			pMoveController->SetRelativeTargetPoint(targetDirection, SonicUSA::System::Random::GetInstance()->genrand_float32() * 10.0f + 50.0f, PlayerNo);
		else
			pMoveController->SetRelativeTargetPoint(targetDirection, SonicUSA::System::Random::GetInstance()->genrand_float32() * 10.0f + 175.0f, PlayerNo);
	}
	else
	{
		pMoveController->SetRelativeTargetPoint(targetDirection, SonicUSA::System::Random::GetInstance()->genrand_float32() * 10.0f + 295.0f, PlayerNo);
	}
}

bool slw_dlc_restoration::ObjCocco::IsInCamera() const
{
	auto* pCamera = GetDocument()->pWorld->GetCamera(0);

	csl::math::Vector3 ndc{};
	return pCamera->TransformNDC(ndc, GetComponent<app::fnd::GOCTransform>()->Frame.Unk3.GetTranslation()) && fabs(ndc.x()) < 1.1f && fabs(ndc.y()) < 1.1f;
}

void slw_dlc_restoration::ObjCocco::CreateAttackers()
{
	auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), PlayerNo);
	if (!pPlayerInfo)
		return;

	csl::math::Matrix34 cameraInvMtx{};
	if (!strcmp(GetDocument()->GetGameMode()->GetName(), "GameModeStageBattle"))
	{
		auto* pPlayer = static_cast<app::Player::CPlayer*>(pMessageManager->GetActor(app::ObjUtil::GetPlayerActorID(*GetDocument(), PlayerNo)));
		csl::math::Matrix34Inverse(pPlayer->rpPhysics->CameraViewMtx, &cameraInvMtx);
	}
	else
	{
		cameraInvMtx = GetDocument()->pWorld->GetCamera(0)->GetInvViewMatrix();
	}

	csl::math::Vector3 rightVector{ -cameraInvMtx.GetColumn(0) };
	csl::math::Vector3 downVector{ -cameraInvMtx.GetColumn(1) };
	csl::math::Vector3 frontVector{ cameraInvMtx.GetColumn(2) };
	csl::math::Vector3 position{ cameraInvMtx.GetTransVector() };

	csl::math::Vector3 rayBase{ csl::math::Vector3(position.x(), pPlayerInfo->Position.y(), position.z()) + frontVector * 10.0f };

	csl::math::Vector3 offset{ pPlayerInfo->Position - position };
	offset -= csl::math::Vector3(csl::math::Vector3::UnitY() * offset.dot(csl::math::Vector3::UnitY()));

	csl::math::Quaternion rotation{};
	if (app::math::Vector3NormalizeIfNotZero(offset, &offset))
	{
		csl::math::Matrix34 rotationMtx{};
		rotationMtx.SetColumn(0, { csl::math::Vector3::UnitY().cross(offset) });
		rotationMtx.SetColumn(1, { csl::math::Vector3::UnitY() });
		rotationMtx.SetColumn(2, offset);

		rotation = { rotationMtx };
	}
	else
	{
		rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	}

	if (!IS_GLOBAL_1_ENABLED)
	{
		IS_GLOBAL_1_ENABLED = true;
		GLOBAL_1_VALUE = 20.0f;
	}

	if (!IS_GLOBAL_2_ENABLED)
	{
		IS_GLOBAL_2_ENABLED = true;
		GLOBAL_2_VALUE = 40.0f;
	}

	for (int i = 0; i < 2; i++)
	{
		float scalar = SonicUSA::System::Random::GetInstance()->genrand_float32() * 20.0f + 10.0f;
		if (i == 0)
			scalar *= ((i + 1) / 2);
		else
			scalar *= ((i + 1) / -2);

		csl::math::Vector3 randomBase{ rayBase + rightVector * scalar };
		csl::math::Vector3 from{ randomBase - downVector * GLOBAL_1_VALUE };
		csl::math::Vector3 to{ from + downVector * GLOBAL_2_VALUE };

		app::game::PhysicsRaycastOutput output{};
		if (!app::ObjUtil::RaycastNearestCollision(&output, *GetDocument(), from, to, 0xC996))
			continue;

		CInfo createInfo{};
		createInfo.Position = { output.HitPoint - downVector * 10.0f };
		createInfo.Rotation = rotation;
		createInfo.pSpawner = GetSpawner();
		createInfo.Unk1 = i;
		createInfo.PlayerNo = PlayerNo;

		auto* pCocco = CreateAttacker(createInfo);
		GetDocument()->AddGameObject(pCocco);
		SubCoccos.push_back({ pCocco });
	}
}