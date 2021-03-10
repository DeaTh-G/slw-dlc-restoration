#pragma once

namespace app
{
	class ObjDroppedEgg;

	struct DroppedEggCInfo
	{
		csl::math::Matrix34 transform;
		csl::math::Vector3 field_40;
		game::PathEvaluator PathEvaluator;
		int ModelType;
		int ZIndex;
		int field_68;
		int field_6C;
	};

	namespace egg
	{
		ObjDroppedEgg* CreateDroppedEgg(GameDocument& gameDocument, DroppedEggCInfo* cInfo);
	}

	class ObjDroppedEgg : public GameObject3D
	{
	private:
		bool ProcMsgHitEventCollision(fnd::Message& message)
		{
			EggCInfo cInfo{};
			GameDocument& document = (GameDocument&)field_24[1];

			// TODO: Copy transform over to EggCInfo
			cInfo.ModelType = ModelType;
			egg::CreateEgg(document, &cInfo);

			return Kill(this);
		}

		INSERT_PADDING(0x1C);
		DroppedEggCInfo* CInfo{};
		int ModelType;
		INSERT_PADDING(0x34);
	
	public:
		ObjDroppedEgg(GameDocument& document, DroppedEggCInfo* cInfo)
		{
			CInfo = cInfo;
			printf("asd");
		}

		void AddCallback(GameDocument* gameDocument) override
		{
			fnd::GOCVisualModel::VisualDescription visualDescriptor{};
			game::ColliSphereShapeCInfo collisionInfo{};
			int unit = 1;

			fnd::GOComponent::Create(this, GOCVisualModel);
			fnd::GOComponent::Create(this, GOCCollider);
			fnd::GOComponent::Create(this, GOCMovementComplex);

			fnd::GOComponent::BeginSetup(this);

			int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
			if (gocTransform)
			{

			}

			ObjEggInfo* info = (ObjEggInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjEggInfo");

			int* gocVisual = GameObject::GetGOC((GameObject*)this, GOCVisual);
			if (gocVisual)
			{
				csl::math::Vector3 visualOffset{ 0, -3, 0 };

				visualDescriptor.Model = info->Models[ModelType];
				visualDescriptor.Animation |= 0x400000;
				visualDescriptor.ZIndex = -0.2 * CInfo->ZIndex;
				fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
				fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &visualOffset);
			}

			int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
			if (gocCollider)
			{
				game::GOCCollider::Setup(gocCollider, &unit);
				collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
				collisionInfo.MotionType = 2;
				collisionInfo.Radius = 3;
				ObjUtil::SetupCollisionFilter(8, &collisionInfo);
				collisionInfo.field_04 |= 1;
				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
			}

			// TODO: GOCMovementComplex
		}

		bool ProcessMessage(fnd::Message& message) override
		{
			if (PreProcessMessage(message))
				return true;

			if (message.field_04 != fnd::PROC_MSG_HIT_EVENT_COLLISION)
				return app::GameObject::ProcessMessage(message);

			ProcMsgHitEventCollision(message);
			return true;
		}
	};
}

app::ObjDroppedEgg* app::egg::CreateDroppedEgg(GameDocument& gameDocument, DroppedEggCInfo* cInfo)
{
	ObjDroppedEgg* object = new ObjDroppedEgg(gameDocument, cInfo);
	if (!object)
		return 0;
	GameDocument::AddGameObject(*(GameDocument**)&gameDocument, object);
	return object;
}