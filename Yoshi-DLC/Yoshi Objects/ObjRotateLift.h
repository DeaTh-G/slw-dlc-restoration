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
		void Initialize(GameDocument* gameDocument)
		{
			int stepModel = 0;
			int chainModel = 0;

			int packFile;
			ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(gameDocument));
			ObjUtil::GetModelResource(&stepModel, "zdlc02_obj_rollinglift_step", &packFile);
			ObjUtil::GetModelResource(&chainModel, "zdlc02_obj_rollinglift", &packFile);

			if (stepModel)
				*(int*)(this + 0x10) = stepModel;

			if (chainModel)
				*(int*)(this + 0x14) = chainModel;
		}

		const char* GetInfoName()
		{
			return "ObjRotateLiftInfo";
		}
	};

	class ObjRotateLift
	{
	public:
		CSetObjectListener* __ct()
		{
			CSetObjectListener::__ct((GameObject*)this);
			*(int*)this = ASLR(0x00D94DAC);
			*((int*)this + 2) = ASLR(0x00D94D90);
			fnd::HFrame::__ct((char*)this + 0x3A0);
			fnd::HFrame::__ct((char*)this + 0x4D0);
			fnd::HFrame::__ct((char*)this + 0x600);
			fnd::HFrame::__ct((char*)this + 0x730);
			*((char*)this + 0x870) = 0x4;
			*((bool*)this + 0x871) = false;
			GameObject::SetObjectCategory((GameObject*)this, 4);

			return (CSetObjectListener*)this;
		}

		void AddCallback(GameDocument* gameDocument)
		{
			int stepCount = *((char*)this + 0x870);
			int containerSize = stepCount + 1;

			float distanceFromOrigin = 25.0f;
			Vector3 positionArray[4] = {
				Vector3(0, distanceFromOrigin, 0),
				Vector3(0, -distanceFromOrigin, 0),
				Vector3(0, 0, distanceFromOrigin),
				Vector3(0, 0, -distanceFromOrigin)
			};

			void* locColSize = _aligned_malloc(16, 16);
			Vector3* collisionSize = new (locColSize)Vector3(10, 10, 10);

			void* locRot = _aligned_malloc(16, 16);
			csl::math::Quaternion* rotation = new (locRot)csl::math::Quaternion(0, 0.707, 0, 0.707);

			// Variables
			int unit = 1;
			fnd::GOCVisualModel::VisualDescription visualDescriptor;
			game::CollisionObjCInfo collisionInfo;
			csl::math::Quaternion setRotation;

			// Create Game Object Components
			int* visualContainer = fnd::GOComponent::Create((GameObject*)this, GOCVisualContainer);
			int* gocCollider	 = fnd::GOComponent::Create((GameObject*)this, GOCCollider);
			int* gocSound		 = fnd::GOComponent::Create((GameObject*)this, GOCSound);
			int* gocMotor		 = fnd::GOComponent::Create((GameObject*)this, GOCMotorRotate);
			
			fnd::GOComponent::BeginSetup((GameObject*)this);

			ObjRotateLiftInfo* info = (ObjRotateLiftInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjRotateLiftInfo");
			ObjRotateLiftData* data = (ObjRotateLiftData*)CSetAdapter::GetData(*(int**)(this + 0x324));
			*(bool*)(this + 0x871) = (true - (data->UseGlobalTime == true));

			CSetAdapter::GetRotation(*(int**)(this + 0x324), &setRotation);

			int* branchHFrame = GameObject::GetGOC((GameObject*)this, GOCTransformString) + 0x28;

			for (size_t i = 0; i < stepCount; i++)
			{
				int* currentHFrameAddress = (int*)(this + 0x3A0 + (i * 0x130));

				fnd::HFrame::SetLocalTranslation(currentHFrameAddress, &positionArray[i]);

				fnd::HFrame::SetLocalRotation(currentHFrameAddress, &setRotation);
				
				fnd::HFrame::ResetFlag(currentHFrameAddress, 0x20);

				fnd::HFrame::AddChild(branchHFrame, currentHFrameAddress);
			}

			fnd::GOCVisualContainer::Setup(visualContainer, &containerSize);

			for (size_t i = 0; i < containerSize; i++)
			{
				int* gocVisual = fnd::GOComponent::CreateSingle((GameObject*)this, GOCVisualModel);
				fnd::GOCVisualContainer::Add(visualContainer, gocVisual);
				fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
				
				if (i < 4)
				{
					int* currentHFrameAddress = (int*)(this + 0x3A0 + (i * 0x130));
					visualDescriptor.Model = *(int*)(info + 0x10);
					visualDescriptor.HFramePointer = currentHFrameAddress;
				}

				if (i == 4)
					visualDescriptor.Model = *(int*)(info + 0x14);

				fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

				if (i == 4)
					fnd::GOCVisualTransformed::SetLocalRotation(gocVisual, rotation);
			}

			// Collider
			if (gocCollider)
			{
				for (size_t i = 0; i < stepCount; i++)
				{
					int* currentHFrameAddress = (int*)(this + 0x3A0 + (i * 0x130));

					game::GOCCollider::Setup(gocCollider, &unit);
					game::CollisionObjCInfo::__ct(&collisionInfo);
					collisionInfo.field_02 = 0xE;
					collisionInfo.CollisionType = 0x0100;		
					collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::Invisible | 0x4003;
					collisionInfo.HFramePointer = currentHFrameAddress;
					collisionInfo.Data[0] = 0x3F7F0201;

					// Collision Radius
					collisionInfo.CollisionSize = Vector3(1, 2.5f, 8);
					game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
				}
			}

			// Motor
			game::MotorInfo motorInfo = {};
			motorInfo.someVector.X = 1;
			motorInfo.Speed = data->Omega;
			game::GOCMotorRotate::Setup(gocMotor, &motorInfo);

			game::GOCSound::SimpleSetup((GameObject*)this, 1, 0);

			fnd::GOComponent::EndSetup((GameObject*)this);

			int deviceTag[3]{};
			game::GOCSound::Play3D(gocSound, deviceTag, "obj_yossyrollinglift_rotate", 0);

		}

		bool ProcessMessage(fnd::Message* message)
		{
			CSetObjectListener::ProcessMessage((int*)this, message);
			return true;
		}
	};

	GameObject* create_ObjRotateLift()
	{
		GameObject* object = GameObject::New(0x880);
		if (!object)
			return 0;
		((ObjRotateLift*)object)->__ct();
		return object;
	}
}