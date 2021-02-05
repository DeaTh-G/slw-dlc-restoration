#pragma once

bool IsShyGuyShadowOn;

namespace app
{
	/*struct EnemyShyGuyData
	{
		float Speed;
		float MaxMoveDistance;
		char IsEventDriven;
		char Direction;
		char field_0A;
		char field_0B;
		float DepthOffset;
	};*/

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
			/*Vector3 position;
			Quaternion rotation;*/
			fnd::GOCVisualModel::Description visualDescriptor;
			game::ShadowSphereShapeCInfo shadowInfo;

			fnd::GOComponent::Create((GameObject*)this, GOCGravity);
			fnd::GOComponent::Create((GameObject*)this, GOCVisualModel);
			fnd::GOComponent::Create((GameObject*)this, GOCAnimationScript);

			if (IsShyGuyShadowOn)
				fnd::GOComponent::Create((GameObject*)this, GOCShadowSimple);

			fnd::GOComponent::Create((GameObject*)this, GOCEffect);
			fnd::GOComponent::Create((GameObject*)this, GOCSound);
			/*fnd::GOComponent::Create((GameObject*)this, GOCCollider);
			fnd::GOComponent::Create((GameObject*)this, GOCEnemyHsm);
			fnd::GOComponent::Create((GameObject*)this, GOCMovementComplex);*/


			EnemyShyGuyInfo* info = (EnemyShyGuyInfo*)ObjUtil::GetObjectInfo(gameDocument, "EnemyShyGuyInfo");
			//EnemyShyGuyData* data = (EnemyShyGuyData*)CSetAdapter::GetData(*(int**)(this + 0x324));

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

			//game::GOCGravity::SimpleSetup((GameObject*)this, 1);
			int* gocMovement = GameObject::GetGOC((GameObject*)this, GOCMovementString);
			if (gocMovement)
			{
				//app::CSetAdapter::GetPosition(*(int**)(this + 0x324), &position);
				//app::CSetAdapter::GetRotation(*(int**)(this + 0x324), &rotation);

				//void* moveStraight = ((csl::fnd::IAllocator*)(((int**)gocMovement)[2]))->Alloc(240, 16);
				//app::game::MoveStraight::__ct(moveStraight);
			}

			game::GOCEffect::SimpleSetup((GameObject*)this);
			game::GOCSound::SimpleSetup((GameObject*)this, 0, 0);
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