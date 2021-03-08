#pragma once

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
		int field_00;
		int field_04;
		int field_08;
		int field_0C;
		int Model;

		CObjInfo* __ct()
		{
			CObjInfo::__ct((CObjInfo*)this);
			field_00 = ASLR(0x00D9964C);
			Model = 0;
			return (CObjInfo*)this;
		}

		void Initialize(GameDocument* gameDocument)
		{
			int packFile;
			ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(gameDocument));
			ObjUtil::GetModelResource(&Model, "zdlc02_obj_yoshicoin", &packFile);
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
			Vector3 position { 0, 5.0f, 0 };

			// Variables
			int unit = 1;
			fnd::GOComponent::Create((GameObject*)this, GOCVisualModel);
			fnd::GOComponent::Create((GameObject*)this, GOCCollider);
			fnd::GOComponent::Create((GameObject*)this, GOCEffect);
			fnd::GOComponent::Create((GameObject*)this, GOCSound);

			if (IsYoshiCoinShadowOn)
				fnd::GOComponent::Create((GameObject*)this, GOCShadowSimple);
		
			fnd::GOCVisualModel::VisualDescription visualDescriptor;
			game::ColliSphereShapeCInfo collisionInfo;
			game::ShadowHemisphereShapeCInfo shadowInfo;
			
			// Game Object Setup
			fnd::GOComponent::BeginSetup((GameObject*)this);
			GameObject* parentObject = CSetObjectListener::GetParentObject((GameObject*)this);
			ObjYoshiCoinData* setData = (ObjYoshiCoinData*)CSetAdapter::GetData(*(int**)(this + 0x324));
			ObjYoshiCoinInfo* info = (ObjYoshiCoinInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjYoshiCoinInfo");

			// Rotate Manager
			void* ringManager = Gimmick::CRingManager::GetService(gameDocument, (void*)ASLR(0x00FECC20));
			if (ringManager)
				Gimmick::CRingManager::RegisterRotateRing(ringManager, (GameObject*)this);

			// Visual Model
			int* gocVisual = GameObject::GetGOC((GameObject*)this, GOCVisual);
			if (gocVisual)
			{
				fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
				visualDescriptor.Model = info->Model;
				fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
				fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &position);
			}

			// Collider
			int* gocCollider = GameObject::GetGOC((GameObject*)this, GOCColliderString);
			if (gocCollider)
			{
				game::GOCCollider::Setup(gocCollider, &unit);
				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.ShapeType = game::CollisionShapeType::TYPE_SPHERE;
				collisionInfo.MotionType = 2;
				collisionInfo.Radius = 3.5f;
				collisionInfo.field_54 = 0;
				collisionInfo.field_44 = 0;
				collisionInfo.field_48 = 0;
				ObjUtil::SetupCollisionFilter(12, &collisionInfo);
				collisionInfo.field_04 = 1;
				game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &position);

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
				app::game::GOCShadowSimple::SetLocalOffsetPosition(gocShadow, &position);
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
				CSetObjectListener::f_ProcessMessage((CSetObjectListener*)this, message);
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

	fnd::ReferencedObject* createObjInfo_ObjYoshiCoinInfo(csl::fnd::IAllocator* allocator)
	{
		fnd::ReferencedObject* object = fnd::ReferencedObject::New(sizeof(ObjYoshiCoinInfo), allocator);
		if (!object)
			return 0;
		((ObjYoshiCoinInfo*)object)->__ct();
		return object;
	}
}