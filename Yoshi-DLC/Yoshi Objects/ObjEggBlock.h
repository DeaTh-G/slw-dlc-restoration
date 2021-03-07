#pragma once
#define M_PI           3.14159265358979323846;

bool IsEggBlockShadowOn;

namespace app
{
	struct ObjEggBlockData
	{
		int PopEggNum;
		float PopEggRandomAddSpeed;
	};

	class ObjEggBlockInfo : public CObjInfo
	{
	public:
		int Model;

		ObjEggBlockInfo()
		{
			field_00 = ASLR(0x00D9494C);
			Model = 0;
		}

		void Initialize(GameDocument* gameDocument)
		{
			int packFile = 0;
			ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(gameDocument));

			ObjUtil::GetModelResource(&Model, "zdlc02_obj_eggblock", &packFile);
		}

		const char* GetInfoName()
		{
			return "ObjEggBlockInfo";
		}

		void RegistCallback(csl::fnd::IAllocator* allocator)
		{

		}
	};

	class ObjEggBlock : public CSetObjectListener
	{
	private:
		struct MotorSwing
		{
			float field_00;
			float field_04;
			float field_08;
			float field_0C;
			float field_10;
		};

		MotorSwing InitMotorParam(float a1, float a2, float a3)
		{
			MotorSwing result{};
			result.field_00 = a2;
			result.field_04 = 0;
			result.field_08 = a1;
			result.field_10 = (3.1415927f + 3.1415927f) / a1;
			result.field_0C = a3 * 0.017453292f;
			return result;
		}

	public:
		fnd::HFrame HFrame;
		MotorSwing Motor;
		float field_04D4[5];
		int EggBlockState;
		Vector3 field_04F0;
		float field_0500;
		float field_0504;
		int field_0508;
		int field_050C;
		int PopEggNum;
		float PopEggRandomAddSpeed;
		game::PathEvaluator PathEvaluator;
		int Padding[2];

		CSetObjectListener* __ct()
		{
			sizeof(CActor);
			CSetObjectListener::__ct(this);

			vftable = ASLR(0x00D9491C);
			field_00[2] = ASLR(0x00D94900);
			game::PathEvaluator::__ct(&PathEvaluator);

			return (CSetObjectListener*)this;
		}

		void AddCallback(GameDocument* gameDocument)
		{
			fnd::GOCVisualModel::VisualDescription visualDescriptor;
			game::ColliBoxShapeCInfo collisionInfo{};
			Vector3 visualOffset { 0, 5, 0 };
			int unit = 1;
			
			fnd::GOComponent::Create(this, GOCVisualModel);
			fnd::GOComponent::Create(this, GOCCollider);
			fnd::GOComponent::Create(this, GOCSound);

			ObjEggBlockData* data = (ObjEggBlockData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
			ObjEggBlockInfo* info = (ObjEggBlockInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjEggBlockInfo");

			uint32_t extUserData = CSetObjectListener::GetExtUserData(this, 0);
			if (!extUserData)
				extUserData = data->PopEggNum + 1;
			this->PopEggNum = extUserData;
			this->PopEggRandomAddSpeed = data->PopEggRandomAddSpeed;

			fnd::GOComponent::BeginSetup(this);

			int* gocVisual = GameObject::GetGOC(this, GOCVisual);
			if (gocVisual)
			{
				fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
				visualDescriptor.Model = info->Model;
				fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
				fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &visualOffset);
			}

			int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
			if (gocCollider)
			{
				Vector3 collisionOffset{};
				Vector3 collisionSize{};

				/* Damage Collision Layer */
				game::GOCCollider::Setup(gocCollider, &unit);

				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.ShapeType = game::CollisionShapeType::TYPE_BOX;
				collisionInfo.MotionType = 2;
				collisionInfo.Size = Vector3(5, 0.65f, 5);
				collisionOffset = Vector3(0, 0.65f, 0);
				game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &collisionOffset);
				ObjUtil::SetupCollisionFilter(0, &collisionInfo);
				collisionInfo.field_0C = 0;
				collisionInfo.field_04 |= 1;
				
				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

				/* Unknown Collision Layer */
				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.ShapeType = game::CollisionShapeType::TYPE_BOX;
				collisionInfo.MotionType = 2;
				collisionInfo.Size = Vector3(3, 8, 3);
				collisionOffset = Vector3(0, 13, 0);
				game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &collisionOffset);
				ObjUtil::SetupCollisionFilter(0, &collisionInfo);
				collisionInfo.field_0C = 1;
				collisionInfo.field_04 |= 1;

				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

				/* Solid Collision Layer */
				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.ShapeType = game::CollisionShapeType::TYPE_BOX;
				collisionInfo.MotionType = 2;
				collisionInfo.Size = Vector3(4.5, 4.5, 4.5);
				collisionOffset = Vector3(0, 4.5, 0);
				game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &collisionOffset);
				collisionInfo.field_02 = 4;
				collisionInfo.field_04 |= 100;
				collisionInfo.field_08 = 0x4003;
				collisionInfo.field_0C = 2;

				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
			}

			game::GOCSound::SimpleSetup(this, 1, 0);
			this->Motor = InitMotorParam(1.8f, 0, 8);

			void* pathManager = game::PathManager::GetService(gameDocument);
			/*if (pathManager && app::game::PathManager::CastRay(pathManager))
			{*/
				game::PathEvaluator::SetPathObject(&PathEvaluator, 0);
				game::PathEvaluator::SetDistance(&PathEvaluator, 0);
			//}

			fnd::GOComponent::EndSetup(this);
		}
	};

	GameObject* create_ObjEggBlock()
	{
		GameObject* object = GameObject::New(sizeof(ObjEggBlock));
		if (!object)
			return 0;
		((ObjEggBlock*)object)->__ct();
		return object;
	}

	ObjEggBlockInfo* createObjInfo_ObjEggBlockInfo(csl::fnd::IAllocator* allocator)
	{
		fnd::ReferencedObject* object = fnd::ReferencedObject::New(sizeof(ObjEggBlockInfo), allocator);
		if (!object)
			return 0;
		return new (object)ObjEggBlockInfo();
	}
}