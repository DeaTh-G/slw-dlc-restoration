#pragma once
#include "../app/CObjInfo.h"
#include "../app/CSetObjectListener.h"
#include "../app/EnemyBase.h"
#include "../app/EnemyInfo.h"
#include "../app/GameDocument.h"
#include "../app/ObjUtil.h"
#include "../app/fnd/GOComponent.h"
#include "../app/animation/AnimationResContainer.h"
#include "../app/game/GOCAnimationScript.h"
#include "../app/game/GOCVisualModel.h"
#include "../LWVariables.h"

bool IsShyGuyShadowOn;

namespace app
{
	class EnemyShyGuyInfo
	{
	public:
		int field_00;
		int field_04;
		int field_08;
		int field_0C;
		const char* Pacfile;
		int field_14;
		int Model;
		int Skeleton;
		animation::AnimationResContainer AnimationContainer;
		int AnimationLeft;
		int AnimationRight;

		EnemyShyGuyInfo* __ct(EnemyShyGuyInfo* a1)
		{
			CObjInfo::__ct((int*)a1);
			a1->Pacfile = "EnemyHeyho.pac";
			a1->field_14 = 0;
			a1->field_00 = ASLR(0x00D93A44);
			a1->Model = 0;
			a1->Skeleton = 0;
			animation::AnimationResContainer::__ct(&(a1->AnimationContainer), a1->field_08);
			a1->AnimationLeft = 0;
			a1->AnimationRight = 0;

			return a1;
		}

		void Initialize(GameDocument* gameDocument)
		{
			int packFile = 0;
			int animationScript = 0;
			int animation = 0;

			EnemyInfo::GetModelResource((int*)this, &(this->Model), "enm_heyho");
			EnemyInfo::GetSkeletonResource((int*)this, &(this->Skeleton), "enm_heyho");

			EnemyInfo::GetEnemyPackfile((int*)this, &packFile);
			ObjUtil::GetAnimationScriptResource(&animationScript, "shyguy", packFile);

			if (animationScript)
				animation::AnimationResContainer::LoadFromBuffer((int*)&(this->AnimationContainer), &animationScript, packFile);

			EnemyInfo::GetAnimationResource((int*)this, &(this->AnimationLeft), "hey_walk_left_loop");
			EnemyInfo::GetAnimationResource((int*)this, &(this->AnimationRight), "hey_walk_right_loop");
		}

		const char* GetInfoName()
		{
			return "EnemyShyGuyInfo";
		}
	};

	class EnemyShyGuy
	{
	public:
		CSetObjectListener* __ct()
		{
			EnemyBase::__ct((GameObject*)this);
			*(int*)this = ASLR(0x00D93A14);
			*((int*)this + 2) = ASLR(0x00D939F8);

			*(int*)(this + 0x4C0) = 0;
			//ObjUtil::SetPropertyLockonTarget();

			return (CSetObjectListener*)this;
		}

		void AddCallback(GameDocument* gameDocument)
		{
			fnd::GOCVisualModel::Description visualDescriptor;
			game::ShadowSphereShapeCInfo shadowInfo;

			fnd::GOComponent::Create((GameObject*)this, GOCVisualModel);
			fnd::GOComponent::Create((GameObject*)this, GOCAnimationScript);

			if (IsShyGuyShadowOn)
				fnd::GOComponent::Create((GameObject*)this, GOCShadowSimple);

			EnemyShyGuyInfo* info = (EnemyShyGuyInfo*)ObjUtil::GetObjectInfo(gameDocument, "EnemyShyGuyInfo");

			fnd::GOComponent::BeginSetup((GameObject*)this);
			int* gocVisual = GameObject::GetGOC((GameObject*)this, GOCVisual);
			if (gocVisual)
			{
				fnd::GOCVisualModel::Description::__ct(&visualDescriptor);
				visualDescriptor.Model = info->Model;
				visualDescriptor.Skeleton = info->Skeleton;
				visualDescriptor.Animation |= 0x400000;
				fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

				int* gocAnimation = GameObject::GetGOC((GameObject*)this, GOCAnimationString);
				if (gocAnimation)
				{
					animation::AnimationResContainer* animation = &(info->AnimationContainer);

					game::GOCAnimationScript::Setup(gocAnimation, (int*)&animation);
					fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
					game::GOCAnimationScript::SetAnimation(gocAnimation, "WALK_R");
				}
			}
			
			int* gocShadow = GameObject::GetGOC((GameObject*)this, GOCShadowString);
			if (gocShadow)
			{
				game::ShadowSphereShapeCInfo::__ct(&shadowInfo, 4);

				game::ShadowSphereShapeCInfo* ppShadowInfo = &shadowInfo;
				game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
			}

			fnd::GOComponent::EndSetup((GameObject*)this);
		}
	};

	GameObject* create_EnemyShyGuy()
	{
		GameObject* object = GameObject::New(0x4D0);
		if (!object)
			return 0;
		((EnemyShyGuy*)object)->__ct();
		return object;
	}

	EnemyShyGuyInfo*create_EnemyShyGuy_EnemyShyGuyInfo(csl::fnd::IAllocator* a1)
	{
		EnemyShyGuyInfo* result = (EnemyShyGuyInfo*)app::fnd::ReferencedObject::New(0x64, a1);
		if (result)
			result->__ct(result);
		return result;
	}
}