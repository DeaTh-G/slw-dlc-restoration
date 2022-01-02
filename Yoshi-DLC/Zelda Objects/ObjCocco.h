#pragma once

namespace app
{
	enum class ObjCoccoState : int
	{
		STATE_IDLE,
		STATE_ATTACK_IN,
		STATE_ATTACK_OUT,
		STATE_END
	};

	struct ObjCoccoData
	{
		float MoveRange;
		int* CoccoList;
		int CoccoListSize;
	};

	class ObjCoccoInfo : public CObjInfo
	{
	public:
		int Model{};
		int Skeleton{};
		animation::AnimationResContainer AnimationContainer{};

		ObjCoccoInfo()
		{
			animation::AnimationResContainer::__ct(&AnimationContainer, (csl::fnd::IAllocator*)pAllocator);
		}

		void Destructor(size_t deletingFlags) override
		{
			ObjCoccoInfo::~ObjCoccoInfo();
			animation::AnimationResContainer::__dt(&AnimationContainer); 

			CObjInfo::Destructor(deletingFlags);
		}

		void Initialize(GameDocument& gameDocument) override
		{
			int packFile = 0;
			int animationScript[3]{};
			ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(&gameDocument));

			ObjUtil::GetModelResource(&Model, "zdlc03_obj_cocco", &packFile);
			ObjUtil::GetSkeletonResource(&Skeleton, "zdlc03_obj_cocco", packFile);

			ObjUtil::GetAnimationScriptResource(&animationScript, "cocco", packFile);
			animationScript[1] = Skeleton;

			if (animationScript)
				animation::AnimationResContainer::LoadFromBuffer(&AnimationContainer, animationScript, packFile);
		}

		const char* GetInfoName() override
		{
			return "ObjCoccoInfo";
		}
	};

	class ObjCocco : public CSetObjectListener
	{
	private:
		ObjCoccoState State{};
		float DeltaTime{};
		INSERT_PADDING(16); // TinyFSM
		int field_3B4{};
		int field_3B8{};
		int field_3BC{};
		csl::math::Vector3 Position{};
		csl::math::Quaternion Rotation{};
		ObjCoccoData* Spawner{};
		int field_3E4{};
		int field_3E8{};
		int field_3EC{};
		char ActionType{};
		INSERT_PADDING(3);
		MoveObjCocco* MovementController{};
		float field_408{};
		float MoveRange{};
		std::vector<ObjCocco*> SubCoccos{};
		float RunAwayTime{};
		float Time{};
		float CryTime{};
		int HealthPoint{};
		char Flags{};
		INSERT_PADDING(3);
		int field_424{};
		int field_428{};
		int field_42C{};

	public:
		ObjCocco()
		{
			HealthPoint = 3;
			ObjUtil::SetPropertyLockonTarget(this);
		}

	protected:
		void AddCallback(GameDocument* gameDocument) override
		{
			fnd::GOComponent::Create(this, GOCVisualModel);
			fnd::GOComponent::Create(this, GOCAnimationScript);
			fnd::GOComponent::Create(this, GOCCollider);
			fnd::GOComponent::Create(this, GOCEffect);
			fnd::GOComponent::Create(this, GOCSound);
			fnd::GOComponent::Create(this, GOCShadowSimple);
			fnd::GOComponent::Create(this, GOCMovementComplex);

			ObjCoccoData* data = GetSpawner();
			MoveRange = data->MoveRange;

			ObjCoccoInfo* info = (ObjCoccoInfo*)ObjUtil::GetObjectInfo(gameDocument, "ObjCoccoInfo");
			GameObject* parentObject = CSetObjectListener::GetParentObject(this);

			fnd::GOComponent::BeginSetup(this);

			if (ActionType)
			{
				fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
				if (gocTransform)
					gocTransform->SetLocalTranslationAndRotation(&Position, &Rotation);
			}

			int* gocVisual = GameObject::GetGOC(this, GOCVisual);
			if (gocVisual)
			{
				fnd::GOCVisualModel::VisualDescription visualDescriptor;
				fnd::GOCVisualModel::VisualDescription::__ct(&visualDescriptor);

				visualDescriptor.Model = info->Model;
				visualDescriptor.Skeleton = info->Skeleton;
				visualDescriptor.Animation |= 0x400000;

				fnd::GOCVisualModel::Setup(gocVisual, &visualDescriptor);

				int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
				if (gocAnimation)
				{
					animation::AnimationResContainer* animation = &(info->AnimationContainer);
					game::GOCAnimationScript::Setup(gocAnimation, &animation);
					fnd::GOCVisualModel::AttachAnimation(gocVisual, gocAnimation);
					game::GOCAnimationScript::ChangeAnimation(gocAnimation, "IDLE");

					csl::fnd::IAllocator* allocator{};
					auto funcPtr = &ObjCocco::SoundCallback;
					animation::AnimCallbackBridge<ObjCocco>* callback =
						(animation::AnimCallbackBridge<ObjCocco>*)AnimCallbackBridge_Initialize(allocator);
					callback->GameObject = this;
					callback->field_10 = reinterpret_cast<void*&>(funcPtr);
					callback->field_14 = -1;

					game::GOCAnimationScript::RegisterCallback(gocAnimation, 0, callback);
				}
			}

			int* gocCollider = GameObject::GetGOC(this, GOCColliderString);
			if (gocCollider)
			{
				csl::math::Vector3 position{ 0, 3, 0 };

				int shapeCount = 1;
				game::GOCCollider::Setup(gocCollider, &shapeCount);

				game::ColliSphereShapeCInfo collisionInfo;
				game::CollisionObjCInfo::__ct(&collisionInfo);

				collisionInfo.ShapeType = game::CollisionShapeType::ShapeType::TYPE_SPHERE;
				collisionInfo.MotionType = 2;
				collisionInfo.field_04 = 1;
				collisionInfo.Radius = 2;
				ObjUtil::SetupCollisionFilter(0, &collisionInfo);
				if (!ActionType)
					collisionInfo.field_08 = 0x20000;

				game::CollisionObjCInfo::SetLocalPosition(&collisionInfo, &position);

				game::GOCCollider::CreateShape(gocCollider, &collisionInfo);
			}

			int* gocShadow = GameObject::GetGOC(this, GOCShadowString);
			if (gocShadow)
			{
				csl::math::Vector3 position{ 0, 3, 0 };
				game::ShadowHemisphereShapeCInfo shadowInfo;

				game::ShadowHemisphereShapeCInfo::__ct(&shadowInfo, 2.5f);
				shadowInfo.field_04 = 1;
				shadowInfo.ShadowQualityType = 2;

				game::ShadowHemisphereShapeCInfo* ppShadowInfo = &shadowInfo;
				game::GOCShadowSimple::Setup(gocShadow, (int**)&ppShadowInfo);
				game::GOCShadowSimple::SetLocalOffsetPosition(gocShadow, &position);
			}

			int* gocMovement = GameObject::GetGOC(this, GOCMovementString);
			if (gocMovement)
			{
				void* movementMem = ((app::fnd::ReferencedObject*)gocMovement)->pAllocator->
					Alloc(sizeof(MoveObjCocco), 16);
				MovementController = new(movementMem) MoveObjCocco();
				game::GOCMovement::SetupController(gocMovement, MovementController);

				auto notifyStopCallback = &ObjCocco::NotifyStopCallback;
				MovementController->SetNotifyStopCallback(-1, notifyStopCallback, this);
			}

			game::GOCEffect::SimpleSetupEx(this, 1, 1);
			game::GOCSound::SimpleSetup(this, 0, 0);

			if (GetExtUserData(0) == 1)
			{
				fnd::GOCVisual::SetVisible(gocVisual, 0);
				game::GOCShadowSimple::SetVisible(gocShadow, 0);
				game::GOCCollider::SetEnable(gocCollider, 0);

				Sleep(this);
			}

			SetEnableAttack(true);

			fnd::GOComponent::EndSetup(this);

			if (State == ObjCoccoState::STATE_IDLE)
			{
				SetTargetOnCircle();
				int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
				if (!gocAnimation)
					return;

				game::GOCAnimationScript::ChangeAnimation(gocAnimation, "MOVE");
			}
		}
		
		bool ProcessMessage(fnd::Message& message) override
		{
			if (PreProcessMessage(message))
				return true;

			switch (message.Type)
			{
			case fnd::PROC_MSG_DAMAGE:
				ProcMsgDamage((xgame::MsgDamage&)message);
				return true;
			case fnd::PROC_MSG_PL_GET_HOMING_TARGET_INFO:
				ProcMsgPLGetHomingTargetInfo((xgame::MsgPLGetHomingTargetInfo&)message);
				return true;
			default:
				return CSetObjectListener::ProcessMessage(message);
			}
		}

		void Update(const fnd::SUpdateInfo& updateInfo) override
		{
			fnd::Message msg{};

			switch (State)
			{
			case app::ObjCoccoState::STATE_IDLE:
				StateIdle(updateInfo);
				break;
			case app::ObjCoccoState::STATE_ATTACK_IN:
				break;
			case app::ObjCoccoState::STATE_ATTACK_OUT:
				break;
			case app::ObjCoccoState::STATE_END:
				break;
			default:
				break;
			}
		}

	private:
		void StateIdle(const fnd::SUpdateInfo& updateInfo)
		{
			int deviceTag[3]{};
			int* gocSound = GameObject::GetGOC(this, GOCSoundString);
			if (!gocSound)
				return;
			
			int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
			if (!gocAnimation)
				return;

			if (CryTime > 0)
			{
				CryTime -= updateInfo.deltaTime;
				if (!(Flags & 1))
					return;
			}
			else
			{
				game::GOCSound::Play3D(gocSound, deviceTag, "obj_cock_cry", 0);
				CryTime += 2;
				if (!(Flags & 1))
					return;
			}

			Flags &= ~1;

			if (HealthPoint <= 0)
			{
				game::GOCAnimationScript::ChangeAnimation(gocAnimation, "ATTACK");

				// StateAttackOut Enter
				State = ObjCoccoState::STATE_ATTACK_OUT;
				
				int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)field_24[1], 0);
				if (playerInfo)
				{
					float magnitude{};
					math::Vector3SquareMagnitude((csl::math::Vector3*)playerInfo + 3, &magnitude);
					if (22500 >= magnitude)
						MovementController->SetTargetPlayer(70, 20);
					else
						MovementController->SetTargetPlayer(220, 20);
				}
				
				return;
			}

			SetTargetOnCircle();
			game::GOCAnimationScript::ChangeAnimation(gocAnimation, "MOVE");
		}

		void StateAttackOut(const fnd::SUpdateInfo& updateInfo)
		{

		}

		void ProcMsgDamage(xgame::MsgDamage& message)
		{
			fnd::SUpdateInfo updateInfo{};
			if (State == ObjCoccoState::STATE_IDLE)
			{
				DamageJump(message.field_40);
				message.field_0C = 1;

				HealthPoint -= message.HitCount;
			}

			message.SetReply(&message.field_30, (HealthPoint | HealthPoint - 1) >> 31);
			message.field_90 |= 0x20;
		}

		void ProcMsgPLGetHomingTargetInfo(xgame::MsgPLGetHomingTargetInfo& message)
		{
			if (HealthPoint <= 0)
			{
				message.field_18 |= 1;
				return;
			}

			csl::math::Vector3 positionOffset { 0, 3, 0 };
			fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
			if (!gocTransform)
				return;
			csl::math::Matrix34 matrix = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);

			message.HitCount = HealthPoint;
			message.field_20 = MultiplyMatrixByVector(&matrix, &positionOffset);
		}
		
		inline static void* AnimCallbackBridge_Initialize(csl::fnd::IAllocator* pAllocator)
		{
			return new animation::AnimCallbackBridge<ObjCocco>();
		}

		ObjCoccoData* GetSpawner()
		{
			if (ActionType)
				return Spawner;
			else
				return (ObjCoccoData*)CSetAdapter::GetData(*(int**)((char*)this + 0x324));
		}

		void SoundCallback(int a1, int a2, int a3)
		{
			int* gocSound = GameObject::GetGOC((GameObject*)((char*)this + 1), GOCSoundString);
			int deviceTag[3]{};
			if (!gocSound)
				return;

			if (!a2)
				if (!a3)
					game::GOCSound::Play3D(gocSound, deviceTag, "obj_cock_flap", 0);
		}

		void SetEnableAttack(bool isEnable)
		{
			if (isEnable)
				Flags |= 4;
			else
				Flags &= ~4;
		}

		void SetTargetOnCircle()
		{
			csl::math::Vector3 depthVector { 0, 0, 1 };

			fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
			if (!gocTransform)
				return;

			csl::math::Matrix34 matrix = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);

			unsigned int random = SonicUSA::System::Random::genrand_int32((int*)ASLR(0x00FBC1C8));
			float distance = SonicUSA::System::RadianMaskS(random * 2.328306436538696e-10f * 3.1415927f + (field_408 + 1.5707964f));
			
			csl::math::Quaternion rotation{};
			CSetAdapter::GetRotation(*(int**)((char*)this + 0x324), &rotation);
			math::Vector3Rotate(&depthVector, &rotation, &depthVector);
			Eigen::Vector3f v(matrix.data[1][0], matrix.data[1][1], matrix.data[1][2]);
			Eigen::Quaternionf q(Eigen::AngleAxisf(distance, v));
			rotation = csl::math::Quaternion(q.x(), q.y(), q.z(), q.w());
			math::Vector3Rotate(&depthVector, &rotation, &depthVector);

			csl::math::Vector3 position{};
			CSetAdapter::GetPosition(*(int**)((char*)this + 0x324), &position);
			math::Vector3Scale(&depthVector, MoveRange, &depthVector);
			math::Vector3Add(&position, &depthVector, &position);

			MovementController->SetTargetPoint(position, 10);
			field_408 = distance;
		}

		void DamageJump(csl::math::Vector3& vector)
		{
			fnd::GOCTransform* gocTransform = (fnd::GOCTransform*)GameObject::GetGOC(this, GOCTransformString);
			if (!gocTransform)
				return;
			csl::math::Matrix34 matrix = *(csl::math::Matrix34*)((int*)gocTransform + 0x44);

			csl::math::Vector3 leftVector = Vector3(matrix.data[1][0], matrix.data[1][1], matrix.data[1][2]);
			csl::math::Vector3 forwardVector = Vector3(matrix.data[2][0], matrix.data[2][1], matrix.data[2][2]);

			float dot = math::Vector3DotProduct(&vector, &leftVector);
			math::Vector3Scale(&leftVector, dot, &leftVector);
			math::Vector3Subtract(&vector, &leftVector, &leftVector);
			if (!math::Vector3NormalizeIfNotZero(&leftVector, &leftVector))
				leftVector = forwardVector;
			
			MovementController->SetTargetDirectionJump(leftVector, 10, 30);

			int* gocAnimation = GameObject::GetGOC(this, GOCAnimationString);
			if (!gocAnimation)
				return;

			game::GOCAnimationScript::ChangeAnimation(gocAnimation, "DAMAGE");

			int* gocEffect = GameObject::GetGOC(this, GOCEffectString);
			if (!gocEffect)
				return;

			game::EffectCreateInfo effectInfo{};
			game::EffectCreateInfo::__ct(&effectInfo);
			effectInfo.Name = "ef_dl3_cocco_dmg";
			effectInfo.field_04 = 1;
			effectInfo.field_08 = 1;
			effectInfo.field_40 = GameObject::GetGOC(this, GOCVisual);
			effectInfo.field_44 = "waist";
			effectInfo.field_48 = -1;
			game::GOCEffect::CreateEffectEx(gocEffect, &effectInfo);

			int deviceTag[3]{};
			int* gocSound = GameObject::GetGOC(this, GOCSoundString);
			if (!gocSound)
				return;

			game::GOCSound::Play3D(gocSound, deviceTag, "obj_cock_damage", 0);
		}

	public:
		void NotifyStopCallback() { Flags |= 1; }
	};

	inline static ObjCocco* create_ObjCocco() { return new ObjCocco(); }

	inline static ObjCoccoInfo* createObjInfo_ObjCoccoInfo(csl::fnd::IAllocator* pAllocator)
	{
		return new(pAllocator) ObjCoccoInfo();
	}
}