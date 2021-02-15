#pragma once

bool IsShyGuyShadowOn;

namespace app
{
	struct EnemyShyGuyData
	{
		float Speed;
		float MaxMoveDistance;
		char IsEventDriven;
		char Direction;
		char field_0A;
		char field_0B;
		float DepthOffset;
	};

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
			ObjUtil::SetPropertyLockonTarget((GameObject*)this);

			return (CSetObjectListener*)this;
		}

		void AddCallback(GameDocument* gameDocument)
		{
			Vector3 position;
			Quaternion rotation;
			Vector3 reticlePosition{};
			reticlePosition.Y = 5;
			fnd::GOCVisualModel::VisualDescription visualDescriptor;
			game::ShadowSphereShapeCInfo shadowInfo;
			app::game::CollisionObjCInfo collisionInfo;
			int unit = 1;

			fnd::GOComponent::Create((GameObject*)this, GOCGravity);
			fnd::GOComponent::Create((GameObject*)this, GOCVisualModel);
			fnd::GOComponent::Create((GameObject*)this, GOCAnimationScript);

			if (IsShyGuyShadowOn)
				fnd::GOComponent::Create((GameObject*)this, GOCShadowSimple);

			fnd::GOComponent::Create((GameObject*)this, GOCEffect);
			fnd::GOComponent::Create((GameObject*)this, GOCSound);
			fnd::GOComponent::Create((GameObject*)this, GOCCollider);
			fnd::GOComponent::Create((GameObject*)this, GOCEnemyHsm);
			fnd::GOComponent::Create((GameObject*)this, GOCMovementComplex);
			fnd::GOComponent::BeginSetup((GameObject*)this);

			EnemyShyGuyInfo* info = (EnemyShyGuyInfo*)ObjUtil::GetObjectInfo(gameDocument, "EnemyShyGuyInfo");
			EnemyShyGuyData* data = (EnemyShyGuyData*)CSetAdapter::GetData(*(int**)(this + 0x324));
			if (data->IsEventDriven)
				*(int*)(this + 0x4C0) |= 1;
			if (data->Direction)
				*(int*)(this + 0x4C0) |= 4;

			EnemyBase::CreateCenterPositionFrame((GameObject*)this, &reticlePosition);
			int* gocVisual = GameObject::GetGOC((GameObject*)this, GOCVisual);
			if (gocVisual)
			{
				fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
				visualDescriptor.Model = info->Model;
				visualDescriptor.Skeleton = info->Skeleton;
				visualDescriptor.Animation |= 0x400000;
				visualDescriptor.ZIndex = data->DepthOffset;
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

			int* gocCollider = GameObject::GetGOC((GameObject*)this, GOCColliderString);
			if (gocCollider)
			{
				game::GOCCollider::Setup(gocCollider, &unit);
				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.Data[0] = 0x00020000;
				collisionInfo.CollisionSize.X = 4;
				ObjUtil::SetupCollisionFilter(9, &collisionInfo);
				collisionInfo.ShapeType = 0x20000;
				collisionInfo.CollisionType |= 1;
				collisionInfo.HFramePointer = EnemyBase::GetCenterPositionFrame((GameObject*)this);
				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
			}

			game::GOCGravity::SimpleSetup((GameObject*)this, 1);
			int* gocMovement = GameObject::GetGOC((GameObject*)this, GOCMovementString);
			if (gocMovement)
			{
				game::MoveStraight::MoveParameter moveParameter{};

				CSetAdapter::GetPosition(*(int**)(this + 0x324), &moveParameter.Position);
				CSetAdapter::GetRotation(*(int**)(this + 0x324), &moveParameter.Rotation);
				moveParameter.field_20 = 100;
				moveParameter.field_24 = 4;
				moveParameter.field_28 = 0x101;
				moveParameter.field_2A = 0;
				moveParameter.field_30.Y = 5;
				moveParameter.field_30.Z = 6;
				moveParameter.field_40.Z = 10;
				moveParameter.field_50.Z = 3;
				moveParameter.field_60 = 10;

				void* moveStraight = ((csl::fnd::IAllocator*)(((int**)gocMovement)[2]))->Alloc(240, 16);
				int* ms = game::MoveStraight::__ct(moveStraight);
				game::GOCMovement::SetupController(gocMovement, moveStraight);
				game::MoveStraight::SetupParamater(moveStraight, &moveParameter);
				game::MoveStraight::SetMoveDistance(moveStraight, data->MaxMoveDistance, 0.0);
				int* contextParam = game::GOCMovement::GetContextParam(gocMovement);
				*((float*)(contextParam + 8)) = data->Speed * -1;
				if ((*(int*)(this + 0x4C0) & 4) == 4)
				{
					*((float*)(contextParam + 8)) = data->Speed;
				}
			}

			game::GOCEffect::SimpleSetup((GameObject*)this);
			game::GOCSound::SimpleSetup((GameObject*)this, 0, 0);
			fnd::GOComponent::EndSetup((GameObject*)this);
		}

		bool ProcessMessage(fnd::Message* message)
		{
			printf("%X\n", message->field_04);
			switch (message->field_04)
			{
			case fnd::PROC_MSG_DAMAGE:
				ProcMsgDamage(message);
				break;
			case fnd::PROC_MSG_KICK:
				ProcMsgKick(message);
				return true;
				break;
			case fnd::PROC_MSG_NOTIFY_OBJECT_EVENT:
				break;
			case fnd::PROC_MSG_HIT_EVENT_COLLISION:
				ProcMsgHitEventCollision(message);
				return true;
				break;
			default:
				EnemyBase::ProcessMessage((int*)this, message);
				return true;
			}
		}
	private:
		void ProcMsgDamage(fnd::Message* message)
		{
			int* gocTransform = GameObject::GetGOC((GameObject*)(this - 8), GOCTransformString);
			if (gocTransform)
			{
				Vector3 translation{};
				math::CalculatedTransform::GetTranslation((Matrix34*)(gocTransform + 0x44), &translation);
				xgame::MsgDamage::SetReply(message, &translation, 1);
				ObjUtil::AddScore((GameObject*)(this - 8), "SHYGUY", message);

				// TODO - EFFECT
				EnemyBase::ProcMission((GameObject*)(this - 8), message);
				CSetObjectListener::SetStatusRetire((GameObject*)(this - 8));
				GameObject::Kill((GameObject*)(this - 8));
			}
		}

		void ProcMsgKick(fnd::Message* message)
		{
			EnemyBlowOffObjectCInfo blowOffInfo;
			EnemyShyGuyInfo* info = (EnemyShyGuyInfo*)ObjUtil::GetObjectInfo(*Document, "EnemyShyGuyInfo");
			int* gocTransform = GameObject::GetGOC((GameObject*)(this - 8), GOCTransformString);
			if (gocTransform)
			{
				EnemyBlowOffObjectCInfo::__ct(&blowOffInfo);
				EnemyBlowOffObjectCInfo::SetParamByMessage(&blowOffInfo, message);
				blowOffInfo.Model = info->Model;
				blowOffInfo.Skeleton = info->Skeleton;
				blowOffInfo.Animation = info->AnimationLeft;
				blowOffInfo.field_10 = *(Matrix34*)(gocTransform + 0x44);
				blowOffInfo.field_50.Y = 5;
				blowOffInfo.field_60.X = 4;
				EnemyBase::CreateEnemyBlowOffObject((GameObject*)(this - 8), &blowOffInfo);
				xgame::MsgKick::SetReplyForSucceed(message);
				ObjUtil::AddScore((GameObject*)(this - 8), "SHYGUY", message);
				CSetObjectListener::SetStatusRetire((GameObject*)(this - 8));
				GameObject::Kill((GameObject*)(this - 8));
			}
		}

		void ProcMsgHitEventCollision(fnd::Message* message)
		{
			EnemyBase::SendTouchDamage((int*)this - 2, message);
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