#pragma once

bool IsJumpBoardShadowOn;

namespace app
{
	struct ObjYoshiJumpBoardData
	{
		float FirstSpeed;
		float OutOfControl;
		float KeepVelocityDistance;
		bool IsPathChange;
		bool IsChangeCameraWhenPathChange;
		bool IsUpdateYaw;
		float OutOfParkour;
	};

	class ObjYoshiJumpBoardInfo
	{

	public:
		void Initialize(GameDocument* gameDocument)
		{
			int model = 0;
			int skeleton = 0;
			int animationScript = 0;

			int packFile;
			ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(gameDocument));
			ObjUtil::GetModelResource(&model, "zdlc02_obj_jumpboard", &packFile);
			ObjUtil::GetSkeletonResource(&skeleton, "zdlc02_obj_jumpboard", packFile);
			ObjUtil::GetAnimationScriptResource(&animationScript, "zdlc02_obj_jumpboard", packFile);

			if (model)
				*(int*)(this + 0x10) = model;
			
			if (skeleton)
				*(int*)(this + 0x14) = skeleton;

			if (animationScript)
				animation::AnimationResContainer::LoadFromBuffer((int*)(this + 0x18), &animationScript, packFile);
		}

		const char* GetInfoName()
		{
			return "ObjYoshiJumpBoardInfo";
		}
	};

	class ObjYoshiJumpBoard
	{
	public:
		CSetObjectListener* __ct(int jumpBoardType)
		{
			CSetObjectListener::__ct((GameObject*)this);
			*(int*)this = ASLR(0x00DE138C);
			*((int*)this + 2) = ASLR(0x00DE1370);

			/* Button Press Success */
			*((int*)(this + 0x3A0)) = 0;

			*((int*)(this + 0x3A4)) = jumpBoardType;

			/* StateType */
			*((int*)(this + 0x3A8)) = 0;

			/* MessageType */
			*((int*)(this + 0x3AC)) = 0;

			/* Default ShotInfo */
			*((int*)(this + 0x3B0)) = 0;

			/* ShotInfo */
			*((int*)(this + 0x3E0)) = 0;

			return (CSetObjectListener*)this;
		}

		void AddCallback(GameDocument* gameDocument)
		{
			// Collision Offset
			void* loc = _aligned_malloc(16, 16);
			Vector3* position = new (loc)Vector3(0, 7.375f, 0);

			game::ShadowSphereShapeCInfo shadowInfo;
			fnd::GOCVisualModel::Description visualDescriptor;
			game::CollisionObjCInfo collisionInfo;
			float jumpBoardScale = *(int*)(this + 0x3A4) ? 0.75f : 1.25f;
			float shadowSize = *(int*)(this + 0x3A4) ? 3.75f : 6.25f;
			int collisionUnit = 1;
			Vector3 objectScale;

			fnd::GOComponent::Create((GameObject*)this, GOCVisualModel);
			fnd::GOComponent::Create((GameObject*)this, GOCAnimationScript);
			fnd::GOComponent::Create((GameObject*)this, GOCCollider);
			
			if (IsJumpBoardShadowOn)
				fnd::GOComponent::Create((GameObject*)this, GOCShadowSimple);
			
			fnd::GOComponent::Create((GameObject*)this, GOCSound);

			ObjYoshiJumpBoardInfo* info = (ObjYoshiJumpBoardInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjYoshiJumpBoardInfo");
			ObjYoshiJumpBoardData* data = (ObjYoshiJumpBoardData*)CSetAdapter::GetData(*(int**)(this + 0x324));

			app::fnd::GOComponent::BeginSetup((GameObject*)this);

			int* gocVisual = GameObject::GetGOC((GameObject*)this, GOCVisual);
			if (gocVisual)
			{
				fnd::GOCVisualModel::Description::__ct(&visualDescriptor);
				visualDescriptor.Model = *(int*)(info + 0x10);
				visualDescriptor.Skeleton = *(int*)(info + 0x14);
				visualDescriptor.Animation |= 0x400000;
				fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

				objectScale.X = jumpBoardScale;
				objectScale.Y = jumpBoardScale;
				objectScale.Z = jumpBoardScale;
				fnd::GOCVisualTransformed::SetLocalScale(gocVisual, &objectScale);

				int* gocAnimation = GameObject::GetGOC((GameObject*)this, GOCAnimationString);
				if (gocAnimation)
				{
					int animation = (int)(info + 0x18);

					game::GOCAnimationScript::Setup(gocAnimation, &animation);
					fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
				}
			}

			int* gocCollider = GameObject::GetGOC((GameObject*)this, GOCColliderString);
			if (gocCollider)
			{
				game::GOCCollider::Setup(gocCollider, &collisionUnit);
				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.field_0C = 0;
				collisionInfo.CollisionType |= 1;
				collisionInfo.Data[0] = 2;
				collisionInfo.Data[1] = 2;
				collisionInfo.CollisionSize.X = *(int*)(this + 0x3A4) ? 2.5f : 5;
				collisionInfo.CollisionSize.Y = *(int*)(this + 0x3A4) ? 3.7f : 4.75f;
				collisionInfo.CollisionSize.Z = 0;

				game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, position);
				ObjUtil::SetupCollisionFilter(0, &collisionInfo);
				
				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
			}

			int* gocShadow = GameObject::GetGOC((GameObject*)this, GOCShadowString);
			if (gocShadow)
			{
				game::ShadowSphereShapeCInfo::__ct(&shadowInfo, shadowSize);
				shadowInfo.field_04 = 6;
				shadowInfo.field_11 = 0;

				game::ShadowSphereShapeCInfo* ppShadowInfo = &shadowInfo;
				game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
			}

			int* gocTransform = GameObject::GetGOC((GameObject*)this, GOCTransformString);
			if (gocTransform)
			{
				game::GOCLauncher::ShotInfo shotInfo;

				SetShotInfo(data->FirstSpeed, data->KeepVelocityDistance, data->OutOfControl,
					(Matrix34*)(gocTransform + 0xC), *(int*)(this + 0x3A4), &shotInfo);
				*((game::GOCLauncher::ShotInfo*)(this + 0x3E0)) = shotInfo;

				SetShotInfo(
					*(int*)(this + 0x3A4) ? 75 : 100, *(int*)(this + 0x3A4) ? 7.5f : 10,
					*(int*)(this + 0x3A4) ? 0.1 : 0.125, (Matrix34*)(gocTransform + 0xC),
					*(int*)(this + 0x3A4), &shotInfo);
				*((game::GOCLauncher::ShotInfo*)(this + 0x3B0)) = shotInfo;
			}

			app::game::GOCSound::SimpleSetup((GameObject*)this, 0, 0);

			fnd::GOComponent::EndSetup((GameObject*)this);
			*((int*)(this + 0x3A8)) = 1;
		}

		bool ProcessMessage(fnd::Message* message)
		{
			if (message->field_04 == fnd::PROC_MSG_HIT_EVENT_COLLISION)
			{
				*((fnd::MessageType*)(this + 0x3A4)) = message->field_04;
				*((int*)(this + 0x39C)) = ((int*)message)[8];
			}

			switch (message->field_04)
			{
			case fnd::PROC_MSG_GET_EXTERNAL_MOVE_POSITION:
			{
				int* gocAnimation = GameObject::GetGOC((GameObject*)(this - 8), GOCAnimationString);
				int* gocTransform = GameObject::GetGOC((GameObject*)(this - 8), GOCTransformString);

				if (app::game::GOCAnimationScript::GetFrame(gocAnimation) < 10.0 && *(int*)(this + 0x3A0) == 3)
				{
					xgame::MsgPLGetInputButton playerGetInputButtonMessage;

					xgame::MsgPLGetInputButton::__ct(&playerGetInputButtonMessage, 0, 0);
					if (ObjUtil::SendMessageImmToPlayer((GameObject*)(this - 8), (int*)&playerGetInputButtonMessage))
					{
						if (!*(int*)(this + 0x398))
							*(int*)(this + 0x398) = playerGetInputButtonMessage.field_20;
						
						if (playerGetInputButtonMessage.field_20 != 0)
							game::GOCAnimationScript::ChangeAnimation(gocAnimation, "JUMP_BIG");

						xgame::MsgExtendPlayer::__dt((int*)&playerGetInputButtonMessage);
					}

					int playerNo = ObjUtil::GetPlayerNo(*(int*)(this + 32), *(int*)(this + 0x39C));
					int* playerInfo = ObjUtil::GetPlayerInformation(*Document, playerNo);

					Vector3 playerPosition = *(Vector3*)(playerInfo + 4);
					Vector3 targetPosition = *(Vector3*)(gocTransform + 0x50);

					if ((std::abs(playerPosition.Y - targetPosition.Y)) > 0.55f)
						if (app::game::GOCAnimationScript::GetFrame(gocAnimation) < 8)
							playerPosition.Y -= 1.25f;
						else
							playerPosition.Y += 5.0f;

					((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][0] = playerPosition.X;
					((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][1] = playerPosition.Y;
					((xgame::MsgGetExternalMovePosition*)message)->Transform->data[3][2] = playerPosition.Z;
				}
				else
				{
					*(int*)(this + 0x3A0) = 4;
				}
			}
			default:
				CSetObjectListener::ProcessMessage((int*)this, message);
				return true;
				break;
			}
		}

		void Update(int* sUpdateInfo)
		{
			StateWait();
			StateExpantion();
		}

	private:
		void StateWait()
		{
			if (*((int*)(this + 0x3A0)) == 1)
			{
				if (*(fnd::MessageType*)(this + 0x3A4) == fnd::PROC_MSG_HIT_EVENT_COLLISION)
				{
					xgame::MsgCatchPlayer catchPlayerMessage;

					int playerNo = ObjUtil::GetPlayerNo(*(int*)(this + 32), *(int*)(this + 0x39C));
					int* playerInfo = ObjUtil::GetPlayerInformation(*Document, playerNo);
					((game::GOCLauncher::ShotInfo*)(this + 0x3D8))->StartingPosition = *(Vector3*)(playerInfo + 4);
					((game::GOCLauncher::ShotInfo*)(this + 0x3A8))->StartingPosition = *(Vector3*)(playerInfo + 4);

					fnd::Message::__ct(&catchPlayerMessage.Base, fnd::PROC_MSG_CATCH_PLAYER);
					catchPlayerMessage.field_18 = 0;
					catchPlayerMessage.field_60 = 0x12;
					catchPlayerMessage.field_64 = 0;
					if (ObjUtil::SendMessageImmToPlayer((GameObject*)(this - 8), (int*)&catchPlayerMessage))
					{
						*((int*)(this + 0x3A0)) = 2;
						xgame::MsgExtendPlayer::__dt((int*)&catchPlayerMessage);
					}
				}
			}
		}
		void StateExpantion()
		{
			int* gocAnimation = GameObject::GetGOC((GameObject*)(this - 8), GOCAnimationString);
			game::GOCLauncher::ShotInfo shotInfo;

			if (*((int*)(this + 0x3A0)) == 2)
			{
				int something[3];

				game::GOCAnimationScript::SetAnimation(gocAnimation, "JUMP_SMALL");

				int* gocSound = GameObject::GetGOC((GameObject*)(this - 8), GOCSoundString);
				if (gocSound)
					game::GOCSound::Play(gocSound, something, "obj_yossyjumpboard", 0);

				*((int*)(this + 0x3A0)) = 3;
			}

			if (*(int*)(this + 0x3A0) == 4)
			{
				shotInfo = *(int*)(this + 0x398) ? *(game::GOCLauncher::ShotInfo*)(this + 0x3D8) :
					*(game::GOCLauncher::ShotInfo*)(this + 0x3A8);

				xgame::MsgSpringImpulse springImpulseMessage;
				xgame::MsgSpringImpulse::__ct(&springImpulseMessage,
					&shotInfo.StartingPosition, &shotInfo.LaunchVector,
					shotInfo.OutOfControl, shotInfo.TravelTime);
				
				ObjYoshiJumpBoardData* setData = (ObjYoshiJumpBoardData*)CSetAdapter::GetData(*(int**)(this + 0x31C));
				springImpulseMessage.OutOfParkour = setData->OutOfParkour;
				if (ObjUtil::SendMessageImmToPlayer((GameObject*)(this - 8), (int*)&springImpulseMessage))
				{
					*(int*)(this + 0x398) = 0;
					*(int*)(this + 0x3A0) = 1;
					*(int*)(this + 0x3A4) = 0;
					*(int*)(this + 0x39C) = 0;
				}
			}
		}
	};
}