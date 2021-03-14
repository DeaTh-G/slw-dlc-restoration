#pragma once

namespace app
{
	class ObjEgg;
	struct EggCInfo;
	namespace egg
	{
		ObjEgg* CreateEgg(GameDocument& gameDocument, EggCInfo* cInfo);
	}

	struct EggCInfo
	{
		csl::math::Matrix34* Transform;
		int ModelType;
	};

	class ObjEggInfo : public CObjInfo
	{
	public:
		int Models[4]{};

		void Initialize(GameDocument& gameDocument) override
		{
			const char* names[4] { "zdlc02_obj_yoshiegg_green", "zdlc02_obj_yoshiegg_blue", "zdlc02_obj_yoshiegg_red", "zdlc02_obj_yoshiegg_yellow" };

			int packFile = 0;
			ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

			for (size_t i = 0; i < 4; i++)
				ObjUtil::GetModelResource(&Models[i], names[i], &packFile);
		}
		const char* GetInfoName() override { return "ObjEggInfo"; }
	};

	class ObjEgg : public GameObject3D
	{
		int State{};
		INSERT_PADDING(0x4);
		INSERT_PADDING(0x14); // TinyFsm
		EggCInfo* CInfo = new EggCInfo();
		int ModelType{};
		int field_33C{};
		int field_340{};
		int field_344{};
		int field_348{};
		int field_34C{};
		csl::math::Vector3 field_350{};
		int field_360{};
		int field_364{};
		int field_368{};
		int field_36C{};
		int field_370{};
		int field_374{};
		int field_378{};
		int field_37C{};

	public:
		ObjEgg(GameDocument& gameDocument, EggCInfo* cInfo)
		{
			CInfo = cInfo;
			ModelType = cInfo->ModelType;
		}

		void AddCallback(GameDocument* gameDocument) override
		{
			fnd::GOComponent::Create(this, GOCVisualModel);
			fnd::GOComponent::Create(this, GOCCollider);
			fnd::GOComponent::Create(this, GOCGravity);
			fnd::GOComponent::Create(this, GOCEffect);
			fnd::GOComponent::Create(this, GOCSound);

			EggManager* eggManager = EggManager::GetService(gameDocument);
			if (!eggManager)
				return;
			bool isEggStored = eggManager->AddEgg(this);

			fnd::GOComponent::BeginSetup(this);
			
			csl::math::Vector3 position = *(csl::math::Vector3*)&CInfo->Transform->data[3][0];
			csl::math::Quaternion rotation = GetRotationFromMatrix(CInfo->Transform);

			int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
			if (gocTransform)
			{
				fnd::GOCTransform::SetLocalTranslation(gocTransform, &position);
				fnd::GOCTransform::SetLocalRotation(gocTransform, &rotation);
			}

			ObjEggInfo* info = (ObjEggInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjEggInfo");

			int* gocVisual = GameObject::GetGOC(this, GOCVisual);
			if (gocVisual)
			{
				fnd::GOCVisualModel::VisualDescription visualDescriptor{};

				fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
				visualDescriptor.Model = info->Models[ModelType];
				visualDescriptor.Animation |= 0x400000;
				visualDescriptor.ZIndex = (-0.2 * field_33C) - 2;
				fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

				Eigen::Quaternion<float> q;
				q = Eigen::AngleAxis<float>((3.1415927 / 2), Eigen::Vector3f(0, 1, 0));
				q.normalize();
				csl::math::Quaternion visualRotation{ q.x(), q.y(), q.z(), q.w() };
				fnd::GOCVisualTransformed::SetLocalRotation(gocVisual, &visualRotation);
			}

			int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
			if (gocCollider)
			{
				int shapeCount = 1;

				game::ColliSphereShapeCInfo collisionInfo{};

				game::GOCCollider::Setup(gocCollider, &shapeCount);
				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
				collisionInfo.MotionType = 2;
				collisionInfo.Radius = 3;
				collisionInfo.field_44 = 0;
				collisionInfo.field_48 = 0;
				collisionInfo.field_54 = 0;
				ObjUtil::SetupCollisionFilter(2, &collisionInfo);
				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
			}

			game::GOCGravity::SimpleSetup(this, 1);
			game::GOCEffect::SimpleSetup(this);
			game::GOCSound::SimpleSetup(this, 0, 0);

			fnd::GOComponent::EndSetup(this);

			int* gocSound = GameObject::GetGOC(this, GOCSoundString);
			if (gocSound)
			{
				int deviceTag[3];

				if (isEggStored)
					game::GOCSound::Play(gocSound, deviceTag, "obj_yossyegg_get", 0);
				else
				{
					xgame::MsgTakeObject msg { 3 };
					ObjUtil::SendMessageImmToPlayer(this, 0, &msg);
					int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
					if (msg.field_20 && gocEffect)
					{
						game::GOCEffect::CreateEffect(gocEffect, "ef_dl2_yossi_birth");
						game::GOCSound::Play(gocSound, deviceTag, "obj_yossy_1up", 0);
					}
					app::GameObject::Kill(this);
				}
			}
		}
	};
}

app::ObjEgg* app::egg::CreateEgg(GameDocument& gameDocument, EggCInfo* cInfo)
{
	ObjEgg* object = new ObjEgg(gameDocument, cInfo);
	if (!object)
		return 0;
	GameDocument::AddGameObject(*(GameDocument**)&gameDocument, object);
	return object;
}