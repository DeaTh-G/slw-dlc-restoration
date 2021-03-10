#pragma once
#include <Dependencies/Eigen/Geometry>

bool IsEggBlockShadowOn;

namespace app
{
	typedef enum
	{
		STATE_IDLE,
		STATE_DAMAGE
	} EggBlockState;

	struct ObjEggBlockData
	{
		int PopEggNum;
		float PopEggRandomAddSpeed;
	};

	class ObjEggBlockInfo : public CObjInfo
	{
	public:
		int Model{};

		ObjEggBlockInfo() { }

		void Initialize(GameDocument& gameDocument) override
		{
			int packFile = 0;
			ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

			ObjUtil::GetModelResource(&Model, "zdlc02_obj_eggblock", &packFile);
		}

		const char* GetInfoName() override
		{
			return "ObjEggBlockInfo";
		}

		void RegistCallback(int& container) override
		{
			ObjEggInfo* eggObject = (ObjEggInfo*)fnd::ReferencedObject::f_new(sizeof(ObjEggInfo), pAllocator);
			if (eggObject)
				new (eggObject)ObjEggInfo();
			CObjInfoContainer::Register(&container, eggObject->GetInfoName(), eggObject);

			ObjYoshiInfo* yoshiObject = (ObjYoshiInfo*)fnd::ReferencedObject::f_new(sizeof(ObjYoshiInfo), pAllocator);
			if (yoshiObject)
				new (yoshiObject)ObjYoshiInfo();
			CObjInfoContainer::Register(&container, yoshiObject->GetInfoName(), yoshiObject);
		}
	};

	class ObjEggBlock : public CSetObjectListener
	{
	private:
		struct MotorParam
		{
			float field_00;
			float field_04;
			float field_08;
			float field_0C;
			float field_10;
		};

		struct PopEggParam
		{
			csl::math::Vector3 field_00;
			csl::math::Vector3* field_10;
			float field_14;
			int field_18;
		};

		MotorParam InitMotorParam(float a1, float a2, float a3)
		{
			MotorParam result{};
			result.field_00 = a2;
			result.field_04 = 0;
			result.field_08 = a1;
			result.field_10 = a3 * 0.017453292f;
			result.field_0C = (3.1415927f + 3.1415927f) / a1;
			return result;
		}
		
		PopEggParam InitPopEggParam(csl::math::Vector3 a1, float a2)
		{
			PopEggParam result{};
			result.field_00 = a1;
			result.field_10 = &result.field_00;
			result.field_14 = a2;
			result.field_18 = 0;
			return result;
		}

		void ProcMsgDamage(xgame::MsgDamage& message)
		{
			if (State != STATE_DAMAGE)
			{
				int* handle = fnd::Handle::Get(&message.field_18);
				if (handle)
				{
					int* gocTransform = GameObject::GetGOC(this, GOCTransformString);
					if (gocTransform)
					{
						csl::math::Vector3 objectPosition = *(csl::math::Vector3*)(gocTransform + 0x50);
						math::Vector3Inverse(&objectPosition);
						objectPosition.Y *= 2.8f;

						int playerNo = ObjUtil::GetPlayerNo(field_24[1], message.field_08);
						int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], playerNo);
						if (playerInfo)
						{
							if (math::Vector3NormalizeIfNotZero(&objectPosition, &objectPosition))
							{
								csl::math::Vector3 b = csl::math::Vector3(0, 1, 0);
								if (math::Vector3DotProduct(&objectPosition, &b) >= 0)
								{
									if (*(handle + 0x2F) && AttackType::IsDamaged(message.AttackType, 0xA))
									{
										DamageMotor = InitMotorParam(0.80000001f, 0, 0);
										EggParam = InitPopEggParam(csl::math::Vector3(0, 1, 0), 50);

										int* gocSound = GameObject::GetGOC(this, GOCSoundString);
										if (gocSound)
										{
											csl::math::Vector3 translation{};
											int deviceTag[3];

											app::game::GOCSound::Play(gocSound, deviceTag, "obj_yossyeggblock_hit", 0);
											math::CalculatedTransform::GetTranslation((csl::math::Matrix34*)(gocTransform + 0x44), &translation);
											xgame::MsgDamage::SetReply(&message, &translation, 0);
											State = STATE_DAMAGE;
										}
									}
									else if (!*(handle + 0x2F) && (message.AttackType & 0x20 || message.AttackType & 0x40))
									{
										printf("Normal Attack\n");
									}
								}
							}
						}
					}
				}
			}
		}

		class asd : public CSetObjectListener
		{
			fnd::HFrame Parent{};
			MotorParam Motor{};
		};

	public:
		fnd::HFrame Parent{};
		MotorParam IdleMotor{};
		MotorParam DamageMotor{};
		EggBlockState State{};
		int field_4EC{};
		PopEggParam EggParam{};
		int field_50C{};
		int PopEggNum{};
		float PopEggRandomAddSpeed{};
		game::PathEvaluator PathEvaluator{};
		INSERT_PADDING(8);

		ObjEggBlock()
		{
			sizeof(ObjEggBlock);
			fnd::HFrame::__ct(&Parent);
			game::PathEvaluator::__ct(&PathEvaluator);
		}

		void AddCallback(GameDocument* gameDocument) override
		{
			fnd::GOCVisualModel::VisualDescription visualDescriptor;
			game::ColliBoxShapeCInfo collisionInfo{};
			csl::math::Vector3 visualOffset { 0, 5, 0 };
			int unit = 3;
			
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

			fnd::HFrame* transformFrame = (fnd::HFrame*)(GameObject::GetGOC(this, GOCTransformString) + 0x28);
			fnd::HFrame::AddChild(transformFrame, &Parent);

			int* gocVisual = GameObject::GetGOC(this, GOCVisual);
			if (gocVisual)
			{
				fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);
				visualDescriptor.Model = info->Model;
				visualDescriptor.Parent = &Parent;
				fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);
				fnd::GOCVisualTransformed::SetLocalTranslation(gocVisual, &visualOffset);
			}

			int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
			if (gocCollider)
			{
				csl::math::Vector3 collisionOffset{};
				csl::math::Vector3 collisionSize{};

				/* Damage Collision Layer */
				game::GOCCollider::Setup(gocCollider, &unit);

				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
				collisionInfo.MotionType = 2;
				collisionInfo.Size = csl::math::Vector3(5, 0.65f, 5);
				collisionOffset = csl::math::Vector3(0, 0.65f, 0);
				game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &collisionOffset);
				ObjUtil::SetupCollisionFilter(0, &collisionInfo);
				collisionInfo.field_0C = 0;
				collisionInfo.field_04 |= 1;
				
				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

				/* Unknown Collision Layer */
				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
				collisionInfo.MotionType = 2;
				collisionInfo.Size = csl::math::Vector3(3, 8, 3);
				collisionOffset = csl::math::Vector3(0, 13, 0);
				game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &collisionOffset);
				ObjUtil::SetupCollisionFilter(0, &collisionInfo);
				collisionInfo.field_0C = 1;
				collisionInfo.field_04 |= 1;

				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);

				/* Solid Collision Layer */
				game::CollisionObjCInfo::__ct(&collisionInfo);
				collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_BOX;
				collisionInfo.MotionType = 2;
				collisionInfo.field_44 = 0;
				collisionInfo.field_48 = 0;
				collisionInfo.Size = csl::math::Vector3(4.5, 4.5, 4.5);
				collisionOffset = csl::math::Vector3(0, 4.5, 0);
				game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &collisionOffset);
				collisionInfo.field_02 = 4;
				collisionInfo.field_04 |= 0x100;
				collisionInfo.field_08 = 0x4003;
				collisionInfo.field_0C = 2;

				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
			}

			game::GOCSound::SimpleSetup(this, 1, 0);
			IdleMotor = InitMotorParam(1.8f, 0, 8);

			void* pathManager = game::PathManager::GetService(gameDocument);
			if (pathManager)
			{
				game::PathRaycastInput inputRay{};
				game::PathRaycastOutput outputRay{};

				CSetAdapter::GetPosition(*(int**)((char*)this + 0x324), &inputRay.StartPostition);
				inputRay.EndPosition = inputRay.StartPostition;
				inputRay.EndPosition.Y -= 400;
				inputRay.field_20 = 1;
				inputRay.field_24 = 1;

				outputRay.field_0C = 1;

				if (game::PathManager::CastRay(pathManager, &inputRay, &outputRay))
				{
					game::PathEvaluator::SetPathObject(&PathEvaluator, outputRay.PathObject);
					game::PathEvaluator::SetDistance(&PathEvaluator, outputRay.Distance);
				}
			}

			fnd::GOComponent::EndSetup(this);
		}

		bool ProcessMessage(fnd::Message& message) override
		{
			if (PreProcessMessage(message))
				return true;

			if (message.field_04 != fnd::PROC_MSG_DAMAGE)
				return CSetObjectListener::ProcessMessage(message);
			
			ProcMsgDamage((xgame::MsgDamage&)message);
			return true;
		}
		void Update(const fnd::SUpdateInfo& updateInfo) override
		{
			float radianRotation = IdleMotor.field_10 * sinf(IdleMotor.field_0C * (IdleMotor.field_04 + IdleMotor.field_00));

			if (State == STATE_IDLE)
			{
				Eigen::Quaternion<float> q;
				q = Eigen::AngleAxis<float>(radianRotation, Eigen::Vector3f(0, 0, 1));
				csl::math::Quaternion rotation { q.x(), q.y(), q.z(), q.w() };

				IdleMotor.field_04 += updateInfo.deltaTime;
				fnd::HFrame::SetLocalRotation(&Parent, &rotation);

				int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
				if (gocCollider)
				{
					void* collider = game::GOCCollider::GetShapeById(gocCollider, 2);
					game::ColliShapeBase::SetLocalRotation(collider, &rotation);
				}
			}
			else if (State == STATE_DAMAGE)
			{
				printf("I'm in danger *chuckles*\n");
			}
		}

	private:
		void DoCheckPopEgg()
		{

		}
	};

	GameObject* create_ObjEggBlock()
	{
		return new ObjEggBlock();
	}

	ObjEggBlockInfo* createObjInfo_ObjEggBlockInfo(csl::fnd::IAllocator* pAllocator)
	{
		return new(pAllocator) ObjEggBlockInfo();
	}
}