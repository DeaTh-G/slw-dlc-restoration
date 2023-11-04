#include "pch.h"
#include "ObjTreasureBox.h"

void slw_dlc_restoration::ObjTreasureBox::AddCallback(app::GameDocument* in_pDocument)
{
	app::ObjTreasureBox::AddCallback(in_pDocument);
	if (!CONFIGURATION.ZeldaTweaks.ScaleObjectsWithPlayer)
		return;

	auto* pPlayer = static_cast<app::Player::CPlayer*>(pMessageManager->GetActor(app::ObjUtil::GetPlayerActorID(*in_pDocument, 0)));
	auto playerScale = pPlayer->GetPlayerGOC<app::Player::CVisualGOC>()->GetHumanVisual()->pGocHolder->GetUnit(0).rpModel->Transform.Scale;

	if (auto* pVisualContainerGoc = GetComponent<app::fnd::GOCVisualContainer>())
	{
		static_cast<app::fnd::GOCVisualTransformed*>(pVisualContainerGoc->Visuals[0])->SetLocalScale(playerScale);
		
		static_cast<app::fnd::GOCVisualTransformed*>(pVisualContainerGoc->Visuals[1])->SetLocalScale({ ms_ItemScale * playerScale.y() });
		static_cast<app::fnd::GOCVisualTransformed*>(pVisualContainerGoc->Visuals[1])->SetLocalTranslation({ ms_PositionOffset.x(), ms_PositionOffset.y() * playerScale.y(), ms_PositionOffset.z() * playerScale.z() });
	}

	if (auto* pColliderGoc = GetComponent<app::game::GOCCollider>())
	{
		auto* pInteractCollider = static_cast<app::game::ColliBoxShapeBase*>(pColliderGoc->GetShapeById(0));
		pInteractCollider->Size = { ms_InteractCollisionSize * playerScale.y() };
		pInteractCollider->SetLocalTranslation({ 0.0f, 10.0f * playerScale.y(), 0.0f });

		auto* pCollider = static_cast<app::game::ColliBoxShapeBase*>(pColliderGoc->GetShapeById(1));
		pCollider->Size = { ms_CollisionSize * playerScale.y() };
		pCollider->SetLocalTranslation({ 0.0f, 8.6f * playerScale.y(), 0.0f });
	}
}

void slw_dlc_restoration::ObjTreasureBox::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	if (FSM_STATE() == &app::ObjTreasureBox::StateWait)
		ChangeState(static_cast<BaseState>(&ObjTreasureBox::StateWait));
	else if (FSM_STATE() == &app::ObjTreasureBox::StateOpen_End)
		ChangeState(static_cast<BaseState>(&ObjTreasureBox::StateOpen_End));

	DispatchFSM(TiFsmEvent_t::CreateUpdate(in_rUpdateInfo.DeltaTime));
}

bool slw_dlc_restoration::ObjTreasureBox::IsPlayerPhantom(uint in_playerNo)
{
	auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), in_playerNo);
	return pPlayerInfo && pPlayerInfo->PixieNo != static_cast<app::Game::EPhantomType>(-1);
}

void slw_dlc_restoration::ObjTreasureBox::PushCamera()
{
	if (!CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
	{
		app::ObjTreasureBox::PushCamera();
		return;
	}

	if (rpCamera)
		return;

	if (rpCamera = new app::Camera::TreasureBoxCamera())
	{
		rpCamera->SetCameraParameter(CameraPosition, GetComponent<app::fnd::GOCTransform>()->Frame.Unk3.Mtx.GetColumn(1), CameraLookAt);
		rpCamera->SetFovy(35.0f);
	}

	app::xgame::MsgPushCameraController msg{ rpCamera, 0.0f, false, 4000, 1, true };
	app::ObjUtil::SendMessageImmToCamera(*this, PlayerNo, msg);
}

void slw_dlc_restoration::ObjTreasureBox::PopCamera()
{
	if (!CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
	{
		app::ObjTreasureBox::PopCamera();
		return;
	}

	if (!rpCamera)
		return;

	app::xgame::MsgPopCameraController msg{ rpCamera, 0.0f, false, 1, true };
	app::ObjUtil::SendMessageImmToCamera(*this, PlayerNo, msg);
}

slw_dlc_restoration::ObjTreasureBox::TiFsmState_t slw_dlc_restoration::ObjTreasureBox::StateWait(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_UPDATE:
	{
		if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
			break;

		return app::ObjTreasureBox::StateWait(in_rEvent);
	}
	case TiFSM_SIGNAL_MESSAGE:
	{
		if (in_rEvent.getMessage().GetType() != app::xgame::MsgHitEventCollision::MessageID)
			break;

		auto& message = static_cast<app::xgame::MsgHitEventCollision&>(in_rEvent.getMessage());

		InteractingPlayerNo = app::ObjUtil::GetPlayerNo(*GetDocument(), message.Sender);

		if (InteractingPlayerNo < 0)
		{
			message.Handled = true;
			break;
		}

		if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
		{
			if (IsPlayerPhantom(InteractingPlayerNo))
			{
				ChangeState(static_cast<BaseState>(&ObjTreasureBox::StateHitOff));
				break;
			}
		}

		app::xgame::MsgCatchPlayer catchMsg{};
		catchMsg.Unk2 = TransformMtx;
		catchMsg.Unk3 = 19;
		catchMsg.Unk4 = false;

		catchMsg.Unk1.set(5);
		if (!app::ObjUtil::SendMessageImmToPlayer(*this, InteractingPlayerNo, catchMsg) && !catchMsg.Unk4)
		{
			message.Handled = true;
			break;
		}

		PlayerNo = InteractingPlayerNo;

		PushCamera();

		app::xgame::MsgStopGameTimer stopTimerMsg{};
		app::ObjUtil::SendMessageImmToGameActor(*this, stopTimerMsg);

		app::xgame::MsgDlcZeldaNoticeStopEnemy stopEnemyMsg{};
		app::ObjUtil::SendMessageImmToGameActor(*this, stopEnemyMsg);

		GetComponent<app::game::GOCCollider>()->SetEnable(false);
		GetComponent<app::game::GOCAnimationSimple>()->SetAnimation(ms_pAnimationName);

		if (pLayerController && !CONFIGURATION.ZeldaTweaks.DisableChestLetterboxing)
		{
			pLayerController->ReserveAnimation(ms_pHudIntroAnimationName, app::game::HudPlayPolicy::eHudPlayPolicy_Once, false);
			pLayerController->ReserveAnimation(ms_pHudIdleAnimationName, app::game::HudPlayPolicy::eHudPlayPolicy_Loop, false);
			pLayerController->PlayReservedAnimation();
		}

		auto* pTransformGoc = GetComponent<app::fnd::GOCTransform>();
		auto* pEffectGoc = GetComponent<app::game::GOCEffect>();
		float originalScaling = pEffectGoc->GlobalScaling;
		
		auto* pPlayer = static_cast<app::Player::CPlayer*>(pMessageManager->GetActor(app::ObjUtil::GetPlayerActorID(*GetDocument(), 0)));
		auto playerScale = pPlayer->GetPlayerGOC<app::Player::CVisualGOC>()->GetHumanVisual()->pGocHolder->GetUnit(0).rpModel->Transform.Scale;
		
		app::game::EffectCreateInfo effectCreateInfo{};
		effectCreateInfo.Position = pEffectGoc->pFrame->Unk3.GetTranslation();
		effectCreateInfo.Rotation = pEffectGoc->pFrame->Unk3.GetRotationQuaternion();
		effectCreateInfo.pName = ms_pChestOpeningEffectName;
		effectCreateInfo.Unk1 = 1.0f;
		
		pEffectGoc->GlobalScaling = playerScale.y();
		pEffectGoc->CreateEffectEx(effectCreateInfo);
		pEffectGoc->GlobalScaling = originalScaling;

		GetComponent<app::game::GOCSound>()->Play(ms_pChestOpeningSoundName, 0.0f);

		app::xgame::MsgChangeBGMVolume changeBgmVolumeMsg{};
		changeBgmVolumeMsg.Unk2 = 0.2f;
		app::ObjUtil::SendMessageImmToGameActor(*this, changeBgmVolumeMsg);

		ChangeState(static_cast<BaseState>(&ObjTreasureBox::StateOpen_ControlCamera));

		message.Handled = true;

		return {};
	}
	default:
		break;
	}

	return FSM_TOP();
}

slw_dlc_restoration::ObjTreasureBox::TiFsmState_t slw_dlc_restoration::ObjTreasureBox::StateHitOff(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_UPDATE:
	{
		if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
		{
			if (!IsPlayerPhantom(InteractingPlayerNo))
			{
				ElapsedTime += in_rEvent.getFloat();
				if (ElapsedTime > 0.25f)
				{
					app::ObjUtil::SetEnableColliShape(GetComponent<app::game::GOCCollider>(), 0, true);
					ChangeState(static_cast<BaseState>(&ObjTreasureBox::StateWait));
					InteractingPlayerNo = static_cast<uint>(-1);
				}
			}
			else
			{
				ElapsedTime = 0.0f;
			}
		}

		break;
	}
	default:
		return app::ObjTreasureBox::StateHitOff(in_rEvent);
	}

	return FSM_TOP();
}

slw_dlc_restoration::ObjTreasureBox::TiFsmState_t slw_dlc_restoration::ObjTreasureBox::StateOpen_ControlCamera(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_UPDATE:
	{
		auto* pAnimationGoc = GetComponent<app::game::GOCAnimationSimple>();
		float frame = pAnimationGoc->GetFrame();
		if (frame >= 460.0f)
		{
			auto* pEffectGoc = GetComponent<app::game::GOCEffect>();
			float originalScaling = pEffectGoc->GlobalScaling;

			auto* pPlayer = static_cast<app::Player::CPlayer*>(pMessageManager->GetActor(app::ObjUtil::GetPlayerActorID(*GetDocument(), 0)));
			auto playerScale = pPlayer->GetPlayerGOC<app::Player::CVisualGOC>()->GetHumanVisual()->pGocHolder->GetUnit(0).rpModel->Transform.Scale;

			GetComponent<app::fnd::GOCVisualContainer>()->Visuals[1]->SetVisible(true);
			GetComponent<app::game::GOCSound>()->Play(ms_pItemGetSoundName, 0.0f);

			app::game::EffectCreateInfo effectInfo{};
			effectInfo.pName = ms_pItemGetTwinkleEffectName;
			effectInfo.Unk1 = ms_ItemGetTwinkleEffectScale * playerScale.y();
			effectInfo.Position = { ms_TwinklePositionOffset.x(), ms_TwinklePositionOffset.y() * playerScale.y(), ms_TwinklePositionOffset.z() * playerScale.z() };
			effectInfo.Unk3 = true;

			pEffectGoc->GlobalScaling = playerScale.y();
			pEffectGoc->CreateEffectEx(effectInfo);
			pEffectGoc->GlobalScaling = originalScaling;

			ChangeState(&ObjTreasureBox::StateOpen_WaitAnim);
			break;
		}

		return app::ObjTreasureBox::StateOpen_ControlCamera(in_rEvent);
	}
	default:
		return app::ObjTreasureBox::StateOpen_ControlCamera(in_rEvent);
	}

	return FSM_TOP();
}

slw_dlc_restoration::ObjTreasureBox::TiFsmState_t slw_dlc_restoration::ObjTreasureBox::StateOpen_End(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_UPDATE:
	{
		if (ElapsedTime <= 0.0f)
			break;

		switch (ItemType)
		{
		case app::ObjTreasureBox::eItemType_HeartContainer:
		{
			app::xgame::MsgDlcZeldaTakeHeartContainer msg{};
			app::ObjUtil::SendMessageImmToGameActor(*this, msg);

			if (auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), PlayerNo))
			{
				app::ObjZeldaOneUp::CInfo createInfo{};
				createInfo.PlayerNo = PlayerNo;
				createInfo.Position = pPlayerInfo->Position;
				createInfo.Rotation = pPlayerInfo->Rotation;
				app::ObjZeldaOneUp::Create(*GetDocument(), createInfo);
			}

			break;
		}
		case app::ObjTreasureBox::eItemType_RupeePurple:
		{
			app::xgame::MsgGetAnimal msg{ 50 };
			app::ObjUtil::SendMessageImmToGameActor(*this, msg);

			break;
		}
		case app::ObjTreasureBox::eItemType_RupeeGold:
		{
			if (CONFIGURATION.ZeldaTweaks.FixTreasureChestRupeeCount)
			{
				app::xgame::MsgGetAnimal msg{ 200 };
				app::ObjUtil::SendMessageImmToGameActor(*this, msg);
			}
			else
			{
				app::xgame::MsgGetAnimal msg{ 50 };
				app::ObjUtil::SendMessageImmToGameActor(*this, msg);
			}

			break;
		}
		default:
		{
			if (CONFIGURATION.ZeldaTweaks.FixTreasureChestRupeeCount)
			{
				app::xgame::MsgGetAnimal msg{ 1 };
				app::ObjUtil::SendMessageImmToGameActor(*this, msg);
			}
			
			break;
		}
		}

		auto* pVisualContainerGoc = GetComponent<app::fnd::GOCVisualContainer>();
		pVisualContainerGoc->Visuals[1]->SetVisible(false);
		pVisualContainerGoc->Visuals[0]->SetVisible(false);

		GetComponent<app::game::GOCEffect>()->CreateEffect(ms_pVanishEffectName);

		if (pLayerController && !CONFIGURATION.ZeldaTweaks.DisableChestLetterboxing)
		{
			pLayerController->ClearReservedAnimation();
			pLayerController->PlayAnimation(ms_pHudOutroAnimationName, app::game::HudPlayPolicy::eHudPlayPolicy_Once, false);
		}

		app::xgame::MsgDlcZeldaNoticeActiveEnemy activeEnemyMsg{};
		app::ObjUtil::SendMessageImmToGameActor(*this, activeEnemyMsg);

		app::xgame::MsgResumeGameTimer resumeTimerMsg{};
		app::ObjUtil::SendMessageImmToGameActor(*this, resumeTimerMsg);

		PopCamera();

		app::xgame::MsgCatchEndPlayer catchEndMsg{};
		catchEndMsg.Unk1 = false;

		if (!CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
			app::ObjUtil::SendMessageImmToPlayer(*this, catchEndMsg);
		else
			app::ObjUtil::SendMessageImmToPlayer(*this, PlayerNo, catchEndMsg);

		PlayerNo = static_cast<uint>(-1);

		ChangeState(static_cast<BaseState>(&ObjTreasureBox::StateOpened));

		break;
	}
	default:
		return app::ObjTreasureBox::StateOpen_End(in_rEvent);
	}

	return FSM_TOP();
}

slw_dlc_restoration::ObjTreasureBox::TiFsmState_t slw_dlc_restoration::ObjTreasureBox::StateOpened(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_UPDATE:
	{
		if (ElapsedTime > 0.0f)
		{
			ElapsedTime -= in_rEvent.getFloat();
			if (ElapsedTime >= 0.0f)
				break;

			app::xgame::MsgChangeBGMVolume msg{};
			msg.Unk1 = 1.0f;
			msg.Unk2 = 2.0f;
			app::ObjUtil::SendMessageImmToGameActor(*this, msg);
		}
		else
		{
			if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
			{
				if (pLayerController)
				{
					if (!pLayerController->IsEndAnimation())
						break;

					SetStatusRetire();
					Kill();
				}
				else
				{
					SetStatusRetire();
					Kill();
				}
			}
		}

		break;
	}
	default:
		return app::ObjTreasureBox::StateOpened(in_rEvent);
	}

	return FSM_TOP();
}