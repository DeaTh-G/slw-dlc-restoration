#include "pch.h"
#include "ObjCrayPipe.h"

void slw_dlc_restoration::ObjCrayPipe::AddCallback(app::GameDocument* in_pDocument)
{
	app::ObjCrayPipe::AddCallback(in_pDocument);

	NumPlayers = GetDocument()->GetService<app::CLevelInfo>()->GetNumPlayers();
}

void slw_dlc_restoration::ObjCrayPipe::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	if (FSM_STATE() == &app::ObjCrayPipe::StatePipeIn)
		ChangeState(static_cast<BaseState>(&ObjCrayPipe::StatePipeIn));

	DispatchFSM(app::TiFsmBasicEvent<app::ObjCrayPipe>::CreateUpdate(in_rUpdateInfo.DeltaTime));
}

app::TTinyFsm<app::ObjCrayPipe>::TiFsmState_t slw_dlc_restoration::ObjCrayPipe::StatePipeIn(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_UPDATE:
	{
		DeltaTime = in_rEvent.getFloat();

		if (!IsPlayerMovedSideWays)
		{
			ElapsedTime += DeltaTime;

			if (20.0f <= ElapsedTime * 15.0f)
				ChangeState(&ObjCrayPipe::StateWarp);
		}

		return {};
	}
	case TiFSM_SIGNAL_ENTER:
	{
		ElapsedTime = 0.0f;

		app::xgame::MsgStopGameTimer msg{};
		app::ObjUtil::SendMessageImmToGameActor(*this, msg);

		for (size_t i = 0; i < NumPlayers; i++)
		{
			auto pEggManager = GetDocument()->GetService<slw_dlc_restoration::EggManager>();
			if (pEggManager)
				pEggManager->SetForceSpaceShrink(true, i);
		}

		return {};
	}
	case TiFSM_SIGNAL_MESSAGE:
	{
		if (in_rEvent.getMessage().GetType() != app::xgame::MsgGetExternalMovePosition::MessageID)
			return FSM_TOP();

		auto pPlayerInfo = app::ObjUtil::GetPlayerInformation(*GetDocument(), PlayerNo);
		if (!pPlayerInfo)
		{
			in_rEvent.getMessage().Handled = true;
			return {};
		}

		if (IsPlayerMovedSideWays)
		{
			csl::math::Vector3 objectPosition{};
			csl::math::Matrix34 objectMatrix = GetComponent<app::fnd::GOCTransform>()->Frame.Unk3.Mtx;
			csl::math::Vector3 objectYAxis = { objectMatrix.GetColumn(1) };

			if (IsDirectionDown)
			{
				objectPosition = objectMatrix.GetTransVector();
			}
			else
			{
				objectPosition = { objectMatrix * csl::math::Vector4(0.0f, -9.0f, 0.0f, 1.0f) };
			}

			csl::math::Vector3 posOffset{ pPlayerInfo->Position - objectPosition };
			csl::math::Vector3 posOffsetNorm{};
			app::math::Vector3NormalizeZero(posOffset, &posOffsetNorm);

			float similarity = posOffset.dot(objectYAxis);
			float minLength = csl::math::Vector3Length({ posOffset - objectYAxis * similarity });
			float yLength = csl::math::Min(30.0f * DeltaTime, minLength);

			PlayerMatrix.SetTransVector({ objectPosition + posOffsetNorm * (minLength - yLength) });

			if (fabs(minLength - yLength) <= 0.000001f)
			{
				if (auto* pSound = GetComponent<app::game::GOCSound>()) pSound->Play3D(ms_pSoundName, *csl::math::Vector3::Zero, 0.0f);

				app::xgame::MsgPLVisibleItemEffect msg{};
				msg.IsVisible = false;
				app::ObjUtil::SendMessageImmToPlayer(*this, PlayerNo, msg);

				IsPlayerMovedSideWays = false;
			}
		}

		float scalar = IsDirectionDown ? -1.0f : 1.0f;

		csl::math::Vector3 yPositionOffset{ 0.0f, csl::math::Min(ElapsedTime * 15.0f, 20.0f) * scalar, 0.0f };
		Eigen::Matrix4f playerMatrixOffset = PlayerMatrix + app::math::Matrix34AffineTransformation(yPositionOffset, csl::math::Quaternion::Identity);

		static_cast<app::xgame::MsgGetExternalMovePosition&>(in_rEvent.getMessage()).pTrsMatrix->SetTransVector(
			static_cast<csl::math::Matrix34*>(&playerMatrixOffset)->GetTransVector());

		in_rEvent.getMessage().Handled = true;
		return {};
	}
	default:
		return FSM_TOP();
	}
}

slw_dlc_restoration::ObjCrayPipeExit::ObjCrayPipeExit() : app::ObjCrayPipeExit()
{
	FSM_INIT(static_cast<BaseState>(&ObjCrayPipeExit::StateIdle));
}

void slw_dlc_restoration::ObjCrayPipeExit::AddCallback(app::GameDocument* in_pDocument)
{
	app::ObjCrayPipeExit::AddCallback(in_pDocument);
	NumPlayers = GetDocument()->GetService<app::CLevelInfo>()->GetNumPlayers();
}

app::TTinyFsm<app::ObjCrayPipeExit>::TiFsmState_t slw_dlc_restoration::ObjCrayPipeExit::StateIdle(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_UPDATE:
	{
		if (!IsPlayerFound)
			return {};

		for (size_t i = 0; i < NumPlayers; i++)
		{
			if (auto* pTransform = GetComponent<app::fnd::GOCTransform>())
			{
				csl::math::Matrix34 objectMatrix = pTransform->Frame.Unk3.Mtx;
				float scalar = IsDirectionUp ? -1.0f : 1.0f;

				objectMatrix.SetTransVector({ objectMatrix.GetTransVector() + csl::math::Vector3(0.0f, 20.f * scalar, 0.0f) });

				app::xgame::MsgCatchPlayer msgCatch{};
				msgCatch.Unk2 = objectMatrix;
				msgCatch.Unk3 = 18;
				if (app::ObjUtil::SendMessageImmToPlayer(*this, i, msgCatch) && msgCatch.Unk4)
					AllPlayersCaught = msgCatch.Unk4;
			}
		}

		if (AllPlayersCaught)
		{
			IsPlayerFound = false;
			ChangeState(static_cast<BaseState>(&ObjCrayPipeExit::StatePipeOut));
		}

		return {};
	}
	case TiFSM_SIGNAL_MESSAGE:
	{
		if (in_rEvent.getMessage().GetType() != app::xgame::MsgHitEventCollision::MessageID)
			return FSM_TOP();

		if (GetExtUserData(eExtUserDataType_High) == 1)
		{
			PlayerNo = app::ObjUtil::GetPlayerNo(*GetDocument(), static_cast<app::xgame::MsgHitEventCollision&>(in_rEvent.getMessage()).Sender);
			if (PlayerNo >= 0)
				IsPlayerFound = true;
		}

		in_rEvent.getMessage().Handled = true;

		return {};
	}
	default:
		return FSM_TOP();
	}
}

app::TTinyFsm<app::ObjCrayPipeExit>::TiFsmState_t slw_dlc_restoration::ObjCrayPipeExit::StatePipeOut(const TiFsmEvent_t& in_rEvent)
{
	switch (in_rEvent.getSignal())
	{
	case TiFSM_SIGNAL_ENTER:
	{
		SetExtUserData(eExtUserDataType_High, 0);

		if (auto* pCollider = GetComponent<app::game::GOCCollider>())
			pCollider->SetEnable(false);

		ElapsedTime = 0.0f;

		return {};
	}
	case TiFSM_SIGNAL_LEAVE:
	{
		app::xgame::MsgResumeGameTimer msg{};
		app::ObjUtil::SendMessageImmToGameActor(*this, msg);

		for (size_t i = 0; i < NumPlayers; i++)
		{
			auto pEggManager = GetDocument()->GetService<slw_dlc_restoration::EggManager>();
			if (pEggManager)
				pEggManager->SetForceSpaceShrink(false, i);
		}

		return {};
	}
	case TiFSM_SIGNAL_UPDATE:
	{
		if (ElapsedTime < 0.5f && 0.5f <= ElapsedTime + in_rEvent.getFloat())
		{
			if (auto* pSound = GetComponent<app::game::GOCSound>())
				pSound->Play3D(ms_pSoundName, 0.0f);
		}

		ElapsedTime += in_rEvent.getFloat();

		if (IsPlayerMoveFinished)
		{
			IsPlayerMoveFinished = false;

			for (size_t i = 0; i < NumPlayers; i++)
			{
				app::xgame::MsgCatchEndPlayer msgCatchEnd{};
				app::ObjUtil::SendMessageImmToPlayer(*this, i, msgCatchEnd);

				app::xgame::MsgPLVisibleItemEffect msgItemEffect{};
				msgItemEffect.IsVisible = true;
				app::ObjUtil::SendMessageImmToPlayer(*this, i, msgItemEffect);
			}

			ChangeState(static_cast<BaseState>(&ObjCrayPipeExit::StateIdle));
		}

		return {};
	}
	case TiFSM_SIGNAL_MESSAGE:
	{
		if (in_rEvent.getMessage().GetType() != app::xgame::MsgGetExternalMovePosition::MessageID)
			return FSM_TOP();

		if (auto* pTransform = GetComponent<app::fnd::GOCTransform>())
		{
			csl::math::Matrix34 objectMatrix = pTransform->Frame.Unk3.Mtx;

			float scalar = IsDirectionUp ? -1.0f : 1.0f;
			float offset = IsDirectionUp ? 0.0f : -9.0f;

			csl::math::Vector3 yPositionOffset{ 0.0f, (20.0f - csl::math::Min(csl::math::Max(ElapsedTime - 0.5f, 0.0f) * 15.0f, 20.0f)) * scalar + offset, 0.0f };
			Eigen::Matrix4f playerMatrixOffset = objectMatrix + app::math::Matrix34AffineTransformation(yPositionOffset, csl::math::Quaternion::Identity);

			static_cast<app::xgame::MsgGetExternalMovePosition&>(in_rEvent.getMessage()).pTrsMatrix->SetTransVector(
				static_cast<csl::math::Matrix34*>(&playerMatrixOffset)->GetTransVector());

			if (fabs((20.0f - csl::math::Min(csl::math::Max(ElapsedTime - 0.5f, 0.0f) * 15.0f, 20.0f)) * scalar) <= 0.000001f)
				IsPlayerMoveFinished = true;
		}

		return {};
	}
	default:
		return FSM_TOP();
	}
}