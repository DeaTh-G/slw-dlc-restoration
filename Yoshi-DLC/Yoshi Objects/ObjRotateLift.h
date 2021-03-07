#pragma once

namespace app
{
	struct ObjRotateLiftData
	{
		float Omega;
		float Offset;
		bool UseGlobalTime;
		bool IsEventDriven;
	};

	class ObjRotateLiftInfo
	{
	public:
		int field_00;
		int field_04;
		int field_08;
		int field_0C;
		int StepModel;
		int ChainModel;

		CObjInfo* __ct()
		{
			CObjInfo::__ct((CObjInfo*)this);
			field_00 = ASLR(0x00D94DDC);
			StepModel = 0;
			ChainModel = 0;

			return (CObjInfo*)this;
		}

		void Initialize(GameDocument* gameDocument)
		{
			int packFile;
			ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(gameDocument));
			ObjUtil::GetModelResource(&StepModel, "zdlc02_obj_rollinglift_step", &packFile);
			ObjUtil::GetModelResource(&ChainModel, "zdlc02_obj_rollinglift", &packFile);
		}

		const char* GetInfoName()
		{
			return "ObjRotateLiftInfo";
		}
	};

	class ObjRotateLift : public CSetObjectListener
	{
	public:
		fnd::HFrame Parents[4];
		INSERT_PADDING(16);
		char StepCount;
		bool UseGlobalTime;
		INSERT_PADDING(12);

		CSetObjectListener* __ct()
		{
			CSetObjectListener::__ct(this);
			vftable = ASLR(0x00D94DAC);
			field_00[0] = ASLR(0x00D94D90);

			for (fnd::HFrame& pointer : Parents)
				fnd::HFrame::__ct(&pointer);

			StepCount = 4;
			UseGlobalTime = false;

			GameObject::SetObjectCategory(this, 4);

			return (CSetObjectListener*)this;
		}

		void AddCallback(GameDocument* gameDocument)
		{
			int unit = StepCount;
			float distanceFromOrigin = 25.0f;
			Vector3 positionArray[4] = {
				Vector3(0, distanceFromOrigin, 0),
				Vector3(0, -distanceFromOrigin, 0),
				Vector3(0, 0, distanceFromOrigin),
				Vector3(0, 0, -distanceFromOrigin)
			};
			fnd::GOCVisualModel::VisualDescription visualDescriptor{};
			game::ColliBoxShapeCInfo collisionInfo{};

			// Create Game Object Components
			fnd::GOComponent::Create(this, GOCVisualContainer);
			fnd::GOComponent::Create(this, GOCCollider);
			fnd::GOComponent::Create(this, GOCSound);
			fnd::GOComponent::Create(this, GOCMotorRotate);
			fnd::GOComponent::BeginSetup(this);

			ObjRotateLiftInfo* info = (ObjRotateLiftInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjRotateLiftInfo");
			ObjRotateLiftData* data = (ObjRotateLiftData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));

			UseGlobalTime = !data->UseGlobalTime;

			fnd::HFrame* transformFrame = (fnd::HFrame*)(GameObject::GetGOC(this, GOCTransformString) + 0x28);

			csl::math::Quaternion rotation;
			CSetAdapter::GetRotation(*(int**)((char*)this + 0x324), &rotation);

			size_t i = 0;
			for (fnd::HFrame& pointer : Parents)
			{
				fnd::HFrame::SetLocalTranslation(&pointer, &positionArray[i]);
				fnd::HFrame::SetLocalRotation(&pointer, &rotation);
				fnd::HFrame::ResetFlag(&pointer, 0x20);
				fnd::HFrame::AddChild(transformFrame, &pointer);
				i++;
			}

			int modelCount = StepCount + 1;

			int* gocContainer = GameObject::GetGOC(this, GOCVisual);
			if (gocContainer)
			{
				fnd::GOCVisualContainer::Setup(gocContainer, &modelCount);

				for (size_t i = 0; i < modelCount; i++)
				{
					int* gocVisual = fnd::GOComponent::CreateSingle(this, GOCVisualModel);
					if (gocVisual)
					{
						fnd::GOCVisualContainer::Add(gocContainer, gocVisual);
						fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

						if (i < 4)
						{
							visualDescriptor.Model = info->StepModel;
							visualDescriptor.Parent = &Parents[i];
						}

						if (i == 4)
							visualDescriptor.Model = info->ChainModel;

						fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

						if (i == 4)
						{
							csl::math::Quaternion chainRot{ 0, 0.707f, 0, 0.707f };
							fnd::GOCVisualTransformed::SetLocalRotation(gocVisual, &chainRot);
						}
					}
				}
			}

			game::GOCSound::SimpleSetup(this, 1, 0);

			int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
			if (gocCollider)
			{
				game::GOCCollider::Setup(gocCollider, &unit);
				for (size_t i = 0; i < StepCount; i++)
				{
					game::CollisionObjCInfo::__ct(&collisionInfo);
					collisionInfo.ShapeType = game::CollisionShapeType::TYPE_BOX;
					collisionInfo.MotionType = 2;
					collisionInfo.field_44 = 0;
					collisionInfo.field_48 = 0;
					collisionInfo.Size = Vector3(1, 2.5f, 8);
					collisionInfo.field_08 = 0x4003;
					collisionInfo.field_04 |= 0x100;
					collisionInfo.field_02 = 14;
					collisionInfo.Parent = &Parents[i];
					game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
				}
			}

			int* gocMotor = GameObject::GetGOC(this, GOCMotorString);
			if (gocMotor)
			{
				game::MotorInfo motorInfo{};
				motorInfo.someVector.X = 1;
				motorInfo.Speed = data->Omega;
				game::GOCMotorRotate::Setup(gocMotor, &motorInfo);

				if (UseGlobalTime)
				{
					game::GOCMotor::RequestEnable(gocMotor, 0);
				}
			}

			fnd::GOComponent::EndSetup(this);
			
			int* gocSound = GameObject::GetGOC(this, GOCSoundString);
			if (gocSound)
			{
				int deviceTag[3]{};
				game::GOCSound::Play3D(gocSound, deviceTag, "obj_yossyrollinglift_rotate", 0);
			}
		}

		bool ProcessMessage(fnd::Message* message)
		{
			printf("%X: \n", message->field_04);
			return CSetObjectListener::ProcessMessage((int*)this, message);
		}
	};

	GameObject* create_ObjRotateLift()
	{
		GameObject* object = GameObject::New(sizeof(ObjRotateLift));
		if (!object)
			return 0;
		((ObjRotateLift*)object)->__ct();
		return object;
	}

	fnd::ReferencedObject* createObjInfo_ObjRotateLiftInfo(csl::fnd::IAllocator* allocator)
	{
		fnd::ReferencedObject* object = fnd::ReferencedObject::New(sizeof(ObjRotateLiftInfo), allocator);
		if (!object)
			return 0;
		((ObjRotateLiftInfo*)object)->__ct();
		return object;
	}
}