#pragma once
#include "../LWVariables.h"
#include "../app/GameObject.h"
#include "../app/fnd/GOComponent.h"
#include "../app/game/GOCEffect.h"
#include "../app/game/GOCSound.h"
#include "../app/game/GOCCollider.h"
#include "../app/game/GOCShadowSimple.h"
#include "../app/game/ShadowShapeCInfo.h"
#include "../app/game/GOCVisualModel.h"
#include "../app/Gimmick/CRingManager.h"
#include "../app/CSetAdapter.h"
#include "../app/ObjUtil.h"
#include "../app/GameDocument.h"
#include "../app/xgame/MsgTakeObject.h"
#include "../app/fnd/CActor.h"
#include "../app/CSetObjectListener.h"
#include "../app/fnd/Message.h"

bool IsYoshiCoinShadowOn;

namespace app
{
	struct ObjYoshiCoinData
	{
		float ResetTime;
		bool IsReset;
		char SetPlaceType;
		char EventType;
	};

	class ObjYoshiCoinInfo
	{
	public:
		void Initialize(GameDocument* gameDocument)
		{
			int coinModel = 0;
			
			int packFile;
			ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(gameDocument));
			ObjUtil::GetModelResource(&coinModel, "zdlc02_obj_yoshicoin", &packFile);

			if (coinModel)
				*(int*)(this + 0x10) = coinModel;
		}

		const char* GetInfoName()
		{
			return "ObjYoshiCoinInfo";
		}
	};

	class ObjYoshiCoin
	{
	public:
		CSetObjectListener* __ct()
		{
			CSetObjectListener::__ct((GameObject*)this);
			*(int*)this = ASLR(0x00D94EEC);
			*((int*)this + 2) = ASLR(0x00D94ED0);

			return (CSetObjectListener*)this;
		}

		void AddCallback(GameDocument* gameDocument)
		{
			// Visual Offset
			void* loc = _aligned_malloc(16, 16);
			Vector3* position = new (loc)Vector3(0, 5.0f, 0);

			// Variables
			int unit = 1;
			fnd::GOComponent::Create((GameObject*)this, GOCVisualModel);
			fnd::GOComponent::Create((GameObject*)this, GOCCollider);
			fnd::GOComponent::Create((GameObject*)this, GOCEffect);
			fnd::GOComponent::Create((GameObject*)this, GOCSound);

			if (IsYoshiCoinShadowOn)
				fnd::GOComponent::Create((GameObject*)this, GOCShadowSimple);
		
			fnd::GOCVisualModel::Description visualDescriptor;
			game::CollisionObjCInfo collisionInfo;
			game::ShadowHemisphereShapeCInfo shadowInfo;
			
			// Game Object Setup
			fnd::GOComponent::BeginSetup((GameObject*)this);
			GameObject* parentObject = CSetObjectListener::GetParentObject((GameObject*)this);
			ObjYoshiCoinData* setData = (ObjYoshiCoinData*)CSetAdapter::GetData(*(int**)(this + 0x324));
			ObjYoshiCoinInfo* info = (ObjYoshiCoinInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjYoshiCoinInfo");

			// Rotate Manager
			void* ringManager = Gimmick::CRingManager::GetService(gameDocument, (void*)ASLR(0x00FECC20));
			Gimmick::CRingManager::RegisterRotateRing(ringManager, (GameObject*)this);

			// Visual Model
			int* gocVisual = GameObject::GetGOC((GameObject*)this, GOCVisual);
			if (gocVisual)
			{
				fnd::GOCVisualModel::Description::__ct(&visualDescriptor);
				visualDescriptor.Model = *(int*)(info + 0x10);
				fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
				fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, position);
			}

			// Collider
			int* gocCollider = GameObject::GetGOC((GameObject*)this, GOCColliderString);
			if (gocCollider)
			{
				game::GOCCollider::Setup(gocCollider, &unit);
				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.field_00 = 0x9C01;
				collisionInfo.field_02 = 0x0002;
				collisionInfo.CollisionType = 0x0001;
				collisionInfo.field_06 = 0xBF11;
				collisionInfo.field_3C = 1.0f;
				collisionInfo.Data[0] = 0x200;
				collisionInfo.Data[3] = -1;

				// Collision Radius
				collisionInfo.CollisionSize.X = 3.5f;
				ObjUtil::SetupCollisionFilter(12, &collisionInfo);
				game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, position);
				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
			}

			// Simple Setups
			game::GOCEffect::SimpleSetup((GameObject*)this);
			game::GOCSound::SimpleSetup((GameObject*)this, 0, 0);

			int* gocShadow = GameObject::GetGOC((GameObject*)this, GOCShadowString);
			if (gocShadow)
			{
				game::ShadowHemisphereShapeCInfo::__ct(&shadowInfo, 2);
				shadowInfo.field_04 = 6;

				game::ShadowHemisphereShapeCInfo* ppShadowInfo = &shadowInfo;
				game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
				app::game::GOCShadowSimple::SetLocalOffsetPosition(gocShadow, position);
			}

			fnd::GOComponent::EndSetup((GameObject*)this);
		}

		bool ProcessMessage(fnd::Message* message)
		{
			switch (message->field_04)
			{
			case fnd::PROC_MSG_NOTIFY_OBJECT_EVENT:
				// TODO - MsgNotifyObjectEvent
				return true;
				break;
			case fnd::PROC_MSG_RING_GET_SUCKED_INTO:
				// TODO - MsgRingGetSuckedInto
				return true;
				break;
			case fnd::PROC_MSG_HIT_EVENT_COLLISION:
			{
				ProcMsgHitEventCollision((int*)message);
				return true;
			}
			default:
				CSetObjectListener::ProcessMessage((int*)this, message);
				return true;
				break;
			}
		}

	private:
		void ProcMsgHitEventCollision(int* message)
		{
			MessageTakeObject msgTakeObject;
			game::EffectCreateInfo effectInfo;

			int* gocSound;
			int* gocEffect;
			int something[3];
			int effectVar = 0;

			xgame::MsgTakeObject::__ct(&msgTakeObject, 0);
			xgame::MsgTakeObject::SetShapeUserID(&msgTakeObject, *(int*)(message[7] + 0xBC));
			if (fnd::CActor::SendMessageImm((int*)this, message[8], (int*)&msgTakeObject))
			{
				gocEffect = GameObject::GetGOC((GameObject*)(this - 8), GOCEffectString);
				if (gocEffect)
				{
					game::EffectCreateInfo::__ct(&effectInfo);
					effectInfo.Name = "ef_dl2_coin_get";
					effectInfo.field_04 = 1;
					effectInfo.Position.Y += 5;
					effectInfo.field_30 = true;
					game::GOCEffect::CreateEffectEx(gocEffect, &effectInfo);
				}

				gocSound = GameObject::GetGOC((GameObject*)(this - 8), GOCSoundString);
				if (gocSound)
					game::GOCSound::Play(gocSound, something, "obj_yossycoin_get", 0);

				int playerNo = ObjUtil::GetPlayerNo(*(int*)(this + 32), message[8]);
				ObjUtil::AddScorePlayerAction((GameObject*)(this - 8), "GET_YOSHICOIN", playerNo);

				SetStatusToKill();
			}
		}

		void SetStatusToKill()
		{
			if ((this + 0xEA0) && 0)
				CSetObjectListener::SetStatusTemporaryRetire((GameObject*)(this - 8), *(float*)(this + 0xE90));
			else
				CSetObjectListener::SetStatusRetire((GameObject*)(this - 8));
			GameObject::Kill((GameObject*)(this - 8));
		}
	};

	GameObject* create_ObjYoshiCoin()
	{
		GameObject* object = GameObject::New(0x3A0);
		if (!object)
			return 0;
		((ObjYoshiCoin*)object)->__ct();
		return object;
	}
}