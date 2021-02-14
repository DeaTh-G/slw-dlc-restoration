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
		void Initialize(GameDocument* gameDocument)
		{
			int model = 0;
			int skeleton = 0;
			int animation = 0;

			int packFile;
			app::ObjUtil::GetPackFile(&packFile, app::ObjUtil::GetStagePackName(gameDocument));
			app::ObjUtil::GetModelResource(&model, "zdlc02_obj_goalA_on", &packFile);

			int packFileVariation;
			hh::ut::Packfile::__ct(&packFileVariation, &packFile);
			app::ObjUtil::GetSkeletonResource(&skeleton, "zdlc02_obj_goalA_on", packFileVariation);
			
			
			app::ObjUtil::GetAnimationResource(&animation, "zdlc02_obj_goalA_on_idle", &packFile);

			if (model)
				*(int*)(this + 0x10) = model;

			if (skeleton)
				*(int*)(this + 0x14) = skeleton;

			if (animation)
				*(int*)(this + 0x18) = animation;
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
			app::game::CollisionObjCInfo collisionInfo;
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
				visualDescriptor.Model = *(int*)(info + 0x10);
				visualDescriptor.Skeleton = *(int*)(info + 0x14);
				app::fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
			}

			int* gocAnimationSimple = app::GameObject::GetGOC((GameObject*)this, GOCAnimation);
			if (gocAnimationSimple)
			{
				app::game::GOCAnimationSimple::Setup(gocAnimationSimple, &unit);
				app::fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimationSimple);
				app::game::GOCAnimationSimple::Add(gocAnimationSimple, "IDLE_LOOP", *(int*)(info + 0x18), 1);
				app::game::GOCAnimationSimple::SetAnimation(gocAnimationSimple, "IDLE_LOOP");
			}

			/* Collider */
			int* gocCollider = app::GameObject::GetGOC((GameObject*)this, GOCColliderString);
			if (gocCollider)
			{
				app::game::GOCCollider::Setup(gocCollider, &unit);
				app::game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.field_00 = 0x9C01;
				collisionInfo.field_02 = 0x0002;
				collisionInfo.CollisionType = 0x0001;
				collisionInfo.field_06 = 0xBF11;
				collisionInfo.field_3C = 1.0f;
				collisionInfo.Data[0] = 0x200;
				collisionInfo.Data[3] = -1;

				/* Collision Radius */
				collisionInfo.CollisionSize.X = 5;
				app::ObjUtil::SetupCollisionFilter(6, &collisionInfo);
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
}