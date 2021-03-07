#pragma once

bool IsYoshiSpecialFlowerShadowOn;

namespace app
{
	struct ObjYoshiSpecialFlowerData
	{
		unsigned int FlowerID;
	};

	class ObjYoshiSpecialFlowerInfo
	{
	public:
		int field_00;
		int field_04;
		int field_08;
		int field_0C;
		int Model;
		int Skeleton;
		int Animation;

		CObjInfo* __ct()
		{
			CObjInfo::__ct((CObjInfo*)this);
			field_00 = ASLR(0x00D95004);
			Model = 0;
			Skeleton = 0;
			Animation = 0;
			return (CObjInfo*)this;
		}

		void Initialize(GameDocument* gameDocument)
		{
			int packFile;
			app::ObjUtil::GetPackFile(&packFile, app::ObjUtil::GetStagePackName(gameDocument));
			app::ObjUtil::GetModelResource(&Model, "zdlc02_obj_goalA_on", &packFile);

			int packFileVariation;
			hh::ut::Packfile::__ct(&packFileVariation, &packFile);
			app::ObjUtil::GetSkeletonResource(&Skeleton, "zdlc02_obj_goalA_on", packFileVariation);
			
			app::ObjUtil::GetAnimationResource(&Animation, "zdlc02_obj_goalA_on_idle", &packFile);
		}

		const char* GetInfoName()
		{
			return "ObjYoshiSpecialFlowerInfo";
		}
	};

	class ObjYoshiSpecialFlower
	{
	public:
		CSetObjectListener* __ct()
		{
			app::CSetObjectListener::__ct((GameObject*)this);
			*(int*)this = ASLR(0x00D94FD4);
			*((int*)this + 2) = ASLR(0x00D94FB8);
			
			/* FlowerID */
			*(int*)(this + 0x490) = 0;

			return (CSetObjectListener*)this;
		}

		void AddCallback(GameDocument* gameDocument)
		{
			app::fnd::GOCVisualModel::VisualDescription visualDescriptor;
			app::game::ColliSphereShapeCInfo collisionInfo;
			game::ShadowHemisphereShapeCInfo shadowInfo;
			int unit = 1;

			/* Create Game Object Components */
			app::fnd::GOComponent::Create((GameObject*)this, GOCVisualModel);
			app::fnd::GOComponent::Create((GameObject*)this, GOCAnimationSimple);
			app::fnd::GOComponent::Create((GameObject*)this, GOCCollider);
			app::fnd::GOComponent::Create((GameObject*)this, GOCEffect);
			app::fnd::GOComponent::Create((GameObject*)this, GOCSound);

			if (IsYoshiSpecialFlowerShadowOn)
				fnd::GOComponent::Create((GameObject*)this, GOCShadowSimple);

			ObjYoshiSpecialFlowerInfo* info = (ObjYoshiSpecialFlowerInfo*)app::ObjUtil::GetObjectInfo(gameDocument, "ObjYoshiSpecialFlowerInfo");
			ObjYoshiSpecialFlowerData* data = (ObjYoshiSpecialFlowerData*)app::CSetAdapter::GetData(*(int**)(this + 0x324));
			*(int*)(this + 0x490) = data->FlowerID;

			app::fnd::GOComponent::BeginSetup((GameObject*)this);

			/* Visual Model */
			int* gocVisual = app::GameObject::GetGOC((GameObject*)this, GOCVisual);
			if (gocVisual)
			{
				app::fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
				visualDescriptor.Model = info->Model;
				visualDescriptor.Skeleton = info->Skeleton;
				app::fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
			}

			int* gocAnimationSimple = app::GameObject::GetGOC((GameObject*)this, GOCAnimation);
			if (gocAnimationSimple)
			{
				app::game::GOCAnimationSimple::Setup(gocAnimationSimple, &unit);
				app::fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimationSimple);
				app::game::GOCAnimationSimple::Add(gocAnimationSimple, "IDLE_LOOP", info->Animation, 1);
				app::game::GOCAnimationSimple::SetAnimation(gocAnimationSimple, "IDLE_LOOP");
			}

			/* Collider */
			int* gocCollider = app::GameObject::GetGOC((GameObject*)this, GOCColliderString);
			if (gocCollider)
			{
				app::game::GOCCollider::Setup(gocCollider, &unit);
				app::game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.ShapeType = game::CollisionShapeType::TYPE_SPHERE;
				collisionInfo.MotionType = 2;
				collisionInfo.Radius = 5;
				collisionInfo.field_54 = 0;
				collisionInfo.field_44 = 0;
				collisionInfo.field_48 = 0;
				app::ObjUtil::SetupCollisionFilter(6, &collisionInfo);
				collisionInfo.field_04 |= 1;

				app::game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
			}

			int* gocShadow = GameObject::GetGOC((GameObject*)this, GOCShadowString);
			if (gocShadow)
			{
				game::ShadowHemisphereShapeCInfo::__ct(&shadowInfo, 6.5f);
				shadowInfo.field_04 = 6;

				game::ShadowHemisphereShapeCInfo* test = &shadowInfo;
				game::GOCShadowSimple::Setup(gocShadow, (int**)&test);
			}

			app::game::GOCEffect::SimpleSetup((GameObject*)this);
			app::game::GOCSound::SimpleSetup((GameObject*)this, 0, 0);

			app::fnd::GOComponent::EndSetup((GameObject*)this);
		}

		bool ProcessMessage(fnd::Message* message)
		{
			switch (message->field_04)
			{
			case fnd::PROC_MSG_HIT_EVENT_COLLISION:
			{
				/* Crashed in 2P */
				ProcMsgHitEventCollision((int*)message);
				return true;
			}
			default:
				return true;
				break;
			}
		}

	private:
		void ProcMsgHitEventCollision(int* message)
		{
			int field_00[3];
			
			void* eggManager = EggManager::GetService(*(GameDocument**)(this + 0x20));
			if (eggManager)
				app::EggManager::TakeYoshiSpecialFlower(eggManager, *(int*)(this + 0x488));

			int* gocEffect = GameObject::GetGOC((GameObject*)(this - 8), GOCEffectString);
			game::GOCEffect::CreateEffect(gocEffect, "ef_dl2_goal_get");

			int* gocSound = GameObject::GetGOC((app::GameObject*)(this - 8), GOCSoundString);
			game::GOCSound::Play(gocSound, field_00, "obj_specialflower_get", 0);

			GameObject::Kill((GameObject*)(this - 8));
			GameObject::SetStatusRetire((GameObject*)(this - 8));
		}
	};

	GameObject* create_ObjYoshiSpecialFlower()
	{
		app::GameObject* object = app::GameObject::New(0x4A0);
		if (!object)
			return 0;
		((app::ObjYoshiSpecialFlower*)object)->__ct();
		return object;
	}

	fnd::ReferencedObject* createObjInfo_ObjYoshiSpecialFlowerInfo(csl::fnd::IAllocator* allocator)
	{
		fnd::ReferencedObject* object = fnd::ReferencedObject::New(sizeof(ObjYoshiSpecialFlowerInfo), allocator);
		if (!object)
			return 0;
		((ObjYoshiSpecialFlowerInfo*)object)->__ct();
		return object;
	}
}