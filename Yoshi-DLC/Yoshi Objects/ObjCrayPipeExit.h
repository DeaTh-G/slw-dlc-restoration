#pragma once
#include "../app/GameObject.h"
#include "../app/CSetObjectListener.h"
#include "../app/CSetAdapter.h"
#include "../app/GameDocument.h"
#include "../app/fnd/GOComponent.h"
#include "../LWVariables.h"
#include "../app/ObjUtil.h"
#include "../app/game/GOCSound.h"
#include "../app/game/GOCCollider.h"
#include "../app/fnd/Message.h"
#include "../app/xgame/MsgCatchEndPlayer.h"
#include "../app/xgame/MsgCatchPlayer.h"
#include "../app/xgame/MsgResumeGameTimer.h"

namespace app
{
	struct ObjCrayPipeExitData
	{
		int Direction;
	};

	class ObjCrayPipeExit
	{
	public:
		CSetObjectListener* __ct()
		{
			CSetObjectListener::__ct((GameObject*)this);
			*(int*)this = ASLR(0x00D947C0);
			*((int*)this + 2) = ASLR(0x00D947A4);

			/* OutwardsDirection */
			*((int*)(this + 0x3A4)) = false;

			*((int*)(this + 0x3A8)) = 0;

			/* IsNotMoving */
			*((bool*)(this + 0x3AC)) = true;

			/* IsInPosition */
			*((bool*)(this + 0x3AD)) = false;

			/* IsFinished */
			*((bool*)(this + 0x3AE)) = false;

			return (CSetObjectListener*)this;
		}
		void AddCallback(GameDocument* gameDocument)
		{
			game::CollisionObjCInfo collisionInfo;
			int unit = 1;

			fnd::GOComponent::Create((GameObject*)this, GOCCollider);
			fnd::GOComponent::Create((GameObject*)this, GOCSound);

			fnd::GOComponent::BeginSetup((GameObject*)this);
			ObjCrayPipeExitData* data = (ObjCrayPipeExitData*)CSetAdapter::GetData(*(int**)(this + 0x324));
			*((int*)(this + 0x3A4)) = data->Direction;

			game::GOCSound::SimpleSetup((GameObject*)this, 0, 0);

			/* Collider */
			int* gocCollider = app::GameObject::GetGOC((GameObject*)this, GOCColliderString);
			if (gocCollider)
			{
				game::GOCCollider::Setup(gocCollider, &unit);
				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.CollisionType |= 4;

				/* Collision Radius */
				collisionInfo.CollisionSize.X = 3;

				ObjUtil::SetupCollisionFilter(7, &collisionInfo);
				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

				uint32_t extUserData = CSetObjectListener::GetExtUserData((GameObject*)this, 0);
				if (!extUserData)
					game::GOCCollider::SetEnable(gocCollider, false);
			}

			fnd::GOComponent::EndSetup((GameObject*)this);
		}

		void Update(int* sUpdateInfo)
		{
			if (*((bool*)(this + 0x3A6)))
			{
				xgame::MsgCatchEndPlayer catchEndPlayerMessage;
				fnd::Message::__ct(&catchEndPlayerMessage.Base, fnd::PROC_MSG_CATCH_END_PLAYER);
				catchEndPlayerMessage.Base.field_00 = (int*)ASLR(0x00D67170);
				catchEndPlayerMessage.Base.field_12 = 0x0019;
				catchEndPlayerMessage.field_18 = false;
				catchEndPlayerMessage.field_19 = 0x0B;
				catchEndPlayerMessage.field_20 = 0x4C;
				catchEndPlayerMessage.field_21 = 0x00;
				ObjUtil::SendMessageImmToPlayer((GameObject*)(this - 8), (int*)&catchEndPlayerMessage);
				xgame::MsgExtendPlayer::__dt((int*)&catchEndPlayerMessage);

				xgame::MsgPLVisibleItemEffect playerVisibleItemEffectMessage;
				fnd::Message::__ct(&playerVisibleItemEffectMessage.Base, fnd::PROC_MSG_PL_VISIBLE_ITEM_EFFECT);
				playerVisibleItemEffectMessage.field_18 = 1;
				ObjUtil::SendMessageImmToPlayer((GameObject*)(this - 8), (int*)&playerVisibleItemEffectMessage);
				xgame::MsgExtendPlayer::__dt((int*)&playerVisibleItemEffectMessage);

				int* gocCollider = GameObject::GetGOC((GameObject*)(this - 8), GOCColliderString);
				if (gocCollider)
					game::GOCCollider::SetEnable(gocCollider, false);
				*((bool*)(this + 0x3A6)) = false;
			}
		}

		void StateIdle()
		{
			(*((int*)(this + 0x3A0)))++;

			if (*((int*)(this + 0x3A0)) >= 3)
			{
				xgame::MsgCatchPlayer catchPlayerMessage;
				fnd::Message::__ct(&catchPlayerMessage.Base, fnd::PROC_MSG_CATCH_PLAYER);
				catchPlayerMessage.field_18 = 0;
				catchPlayerMessage.field_60 = 0x12;
				catchPlayerMessage.field_64 = 0;

				ObjUtil::SendMessageImmToPlayer((GameObject*)(this - 8), (int*)&catchPlayerMessage);
			}
		}

		void StatePipeOut(xgame::MsgGetExternalMovePosition* message)
		{
			int* gocTransform = GameObject::GetGOC((GameObject*)(this - 8), GOCTransformString);
			if (gocTransform)
			{
				int* playerInfo = ObjUtil::GetPlayerInformation(*Document, 0);
				Vector3 playerPosition = *(Vector3*)(playerInfo + 4);
				Vector3 targetPosition = *(Vector3*)(gocTransform + 0x50);
				float verticalDestination = 0;

				if (*((bool*)(this + 0x39C)))
				{
					verticalDestination = targetPosition.Y + 14;

					if ((std::abs(playerPosition.Y - verticalDestination) < 23.65f))
						PipeOutMotion(playerPosition, message);
					else
						*((bool*)(this + 0x3A6)) = true;
				}
				else
				{
					verticalDestination = targetPosition.Y;
					if ((std::abs(playerPosition.Y - verticalDestination) > 0.35f))
						PipeOutMotion(playerPosition, message);
					else
						*((bool*)(this + 0x3A6)) = true;
				}
			}
		}

		bool ProcessMessage(fnd::Message* message)
		{
			switch (message->field_04)
			{
			case fnd::PROC_MSG_NOTIFY_OBJECT_EVENT:
			{
				ProcMsgNotifyObjectEvent((xgame::MsgNotifyObjectEvent*)message);
				return true;
			}
			case fnd::PROC_MSG_STAY_TRIGGER:
			{
				StateIdle();
				return true;
			}
			case fnd::PROC_MSG_GET_EXTERNAL_MOVE_POSITION:
			{
				int* gocTransform = GameObject::GetGOC((GameObject*)(this - 8), GOCTransformString);
				if (gocTransform)
				{
					int* playerInfo = ObjUtil::GetPlayerInformation(*Document, 0);
					Vector3 playerPosition = *(Vector3*)(playerInfo + 4);
					Vector3 targetPosition = *(Vector3*)(gocTransform + 0x50);
					float verticalDestination = 0;

					if (!*((bool*)(this + 0x3A6)))
					{
						if (!*((bool*)(this + 0x3A5)))
						{
							if (*((int*)(this + 0x39C)))
							{
								verticalDestination = targetPosition.Y += 2;
								if (std::abs(playerPosition.Y - verticalDestination) > 1.95f)
								{
									((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][0] = playerPosition.X;
									((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][1] = verticalDestination;
									((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][2] = playerPosition.Z;
								}
								else
								{
									*((bool*)(this + 0x3A5)) = true;
								}
							}
							else
							{
								verticalDestination = targetPosition.Y -= 12;
								if (std::abs(playerPosition.Y - verticalDestination) > 11.95f)
								{
									((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][0] = playerPosition.X;
									((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][1] = verticalDestination;
									((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][2] = playerPosition.Z;
								}
								else
								{
									*((bool*)(this + 0x3A5)) = true;
								}
							}
						}
						else
						{
							if (*((int*)(this + 0x3A0)) == 3)
							{
								xgame::MsgResumeGameTimer resumeGameTimerMessage;

								// Resuming Game Timer
								fnd::Message::__ct(&resumeGameTimerMessage.Base, fnd::PROC_MSG_RESUME_GAME_TIMER);
								if (ObjUtil::SendMessageImmToGameActor((GameObject*)(this - 8), (int*)&resumeGameTimerMessage))
								{
									void* eggManager = EggManager::GetService(*Document);
									if (eggManager)
										EggManager::SetForceSpaceShrink(eggManager, false);

									xgame::MsgExtendPlayer::__dt((int*)&resumeGameTimerMessage);
									(*((int*)(this + 0x3A0)))++;
								}
							}

							StatePipeOut((xgame::MsgGetExternalMovePosition*)message);
						}
					}
				}
				return true;
			}
			default:
				CSetObjectListener::ProcessMessage((int*)this, message);
				return true;
				break;
			}
		}

	private:
		void ProcMsgNotifyObjectEvent(xgame::MsgNotifyObjectEvent* message)
		{
			if (message->field_18 == 1)
				CSetObjectListener::SetExtUserData((GameObject*)(this - 8), 0, 2);
			else if (message->field_18 != 2)
				CSetObjectListener::SetExtUserData((GameObject*)(this - 8), 0, 1);
		}

		void PipeOutMotion(Vector3 playerPos, xgame::MsgGetExternalMovePosition* message)
		{
			if (*((bool*)(this + 0x39C)))
				playerPos.Y -= 0.3f;
			else
				playerPos.Y += 0.3f;

			((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][0] = playerPos.X;
			((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][1] = playerPos.Y;
			((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][2] = playerPos.Z;

			if (*((bool*)(this + 0x3A4)))
			{
				int* gocSound = GameObject::GetGOC((GameObject*)(this - 8), GOCSoundString);
				if (gocSound)
				{
					app::game::GOCSound::Play3D(gocSound, &deviceTag, "obj_yossypipe_in_out", 0);
					*((bool*)(this + 0x3A4)) = false;
				}
			}
		}
	};

	GameObject* create_ObjCrayPipeExit()
	{
		GameObject* object = GameObject::New(0x3B0);
		if (!object)
			return 0;
		((ObjCrayPipeExit*)object)->__ct();
		return object;
	}
}