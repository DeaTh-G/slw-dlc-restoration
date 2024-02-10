#include "pch.h"
#include "ObjGolonRock.h"

slw_dlc_restoration::ObjGolonRock::ObjGolonRock(const app::golon_rock::GolonRockCreateInfo& in_rCreateInfo) : app::ObjGolonRock(in_rCreateInfo)
{

}

void slw_dlc_restoration::ObjGolonRock::AddCallback(app::GameDocument* in_pDocument)
{
	app::fnd::GOComponent::Create<app::fnd::GOCVisualContainer>(*this);
	app::fnd::GOComponent::Create<app::game::GOCAnimationSimple>(*this);
	app::fnd::GOComponent::Create<app::game::GOCGravity>(*this);
	app::fnd::GOComponent::Create<app::game::GOCCollider>(*this);
	app::fnd::GOComponent::Create<app::game::GOCMovementComplex>(*this);
	app::fnd::GOComponent::Create<app::game::GOCSound>(*this);
	app::fnd::GOComponent::Create<app::game::GOCEffect>(*this);
	app::fnd::GOComponent::Create<app::game::GOCShadowSimple>(*this);

	app::fnd::GOComponent::BeginSetup(*this);

	auto* pInfo = app::ObjUtil::GetObjectInfo<app::ObjGolonRockGeneratorInfo>(*in_pDocument);

	if (auto* pTransformGoc = GetComponent<app::fnd::GOCTransform>())
	{
		csl::math::Quaternion rotation{ pCreateInfo->TransformMtx };

		pTransformGoc->SetLocalTranslation({ pCreateInfo->TransformMtx.GetTransVector() + app::math::Vector3Rotate(rotation, { -csl::math::Vector3::UnitY() }) * 20.0f });
		pTransformGoc->SetLocalRotation(rotation);
	}

	if (auto* pVisualContainerGoc = GetComponent<app::fnd::GOCVisualContainer>())
	{
		pVisualContainerGoc->Setup({ ms_ModelCount });

		if (auto* pVisualGoc = app::fnd::GOComponent::CreateSingle<app::fnd::GOCVisualModel>(*this))
		{
			app::fnd::GOCVisualModel::Description description{};
			description.Model = pInfo->ModelContainer.Models[0];
			description.Skeleton = pInfo->Skeleton;
			description.Unk2 |= 0x400000;

			pVisualGoc->Setup(description);
			pVisualContainerGoc->Add(pVisualGoc);
			pVisualGoc->SetLocalRotation({ Eigen::AngleAxisf(MATHF_PI, csl::math::Vector3::UnitY()) });

			if (auto* pAnimationGoc = GetComponent<app::game::GOCAnimationSimple>())
			{
				pAnimationGoc->Setup({ ms_AnimationCount });
				pVisualGoc->AttachAnimation(pAnimationGoc);

				pAnimationGoc->Add(ms_pAnimationName, pInfo->Animation, app::game::PlayPolicy::Once);
			}
		}

		if (pRockVisualGoc = app::fnd::GOComponent::CreateSingle<app::fnd::GOCVisualModel>(*this))
		{
			app::fnd::GOCVisualModel::Description description{};
			description.Model = pInfo->ModelContainer.Models[1];
			description.Unk2 |= 0x400000;

			pRockVisualGoc->Setup(description);
			pVisualContainerGoc->Add(pRockVisualGoc);
			pRockVisualGoc->SetVisible(false);
		}
	}

	if (auto* pColliderGoc = GetComponent<app::game::GOCCollider>())
	{
		pColliderGoc->Setup({ ms_ShapeCount });

		app::game::ColliSphereShapeCInfo hitCollisionInfo{};
		hitCollisionInfo.ShapeType = app::game::CollisionShapeType::ShapeType::eShapeType_Sphere;
		hitCollisionInfo.MotionType = app::game::PhysicsMotionType::MotionType::eMotionType_Value2;
		hitCollisionInfo.Unk2 |= 1;
		hitCollisionInfo.Radius = ms_HitCollisionSize;
		hitCollisionInfo.ShapeID = 0;
		app::ObjUtil::SetupCollisionFilter(app::ObjUtil::EFilter::eFilter_Default, hitCollisionInfo);
		pColliderGoc->CreateShape(hitCollisionInfo);

		app::game::ColliSphereShapeCInfo collisionInfo{};
		collisionInfo.ShapeType = app::game::CollisionShapeType::ShapeType::eShapeType_Sphere;
		collisionInfo.MotionType = app::game::PhysicsMotionType::MotionType::eMotionType_Value0;
		collisionInfo.Unk2 |= 0x100;
		collisionInfo.Unk3 = 3;
		collisionInfo.Radius = ms_CollisionSize;
		collisionInfo.ShapeID = 1;
		pColliderGoc->CreateShape(collisionInfo);
	}

	if (auto* pMovementGoc = GetComponent<app::game::GOCMovementComplex>())
	{
		if (pMovementController = pMovementGoc->SetMoveController<game::MoveObjGolonRock>())
		{
			game::MoveObjGolonRock::SetupParam setupParam{};
			setupParam.Position = GetComponent<app::fnd::GOCTransform>()->Transform.Position;
			setupParam.YOffset = 20.0f;
			setupParam.PopupTime = 0.3f;
			setupParam.RollWaitTime = 0.4f;
			setupParam.Speed = pCreateInfo->Speed;
			setupParam.Unk1 = MATHF_PI / 90.0f;
			setupParam.Unk2 = MATHF_PI / 2.0f;
			setupParam.Unk5 = 300.0f;
			setupParam.Unk6 = 1.0f;
			setupParam.MoveEndMemFunc = static_cast<void (app::GameObject::*)()>(&ObjGolonRock::NotifyMoveEndCallback);
			setupParam.PassPlayerMemFunc = static_cast<void (app::GameObject::*)()>(&ObjGolonRock::NotifyPassPlayerCallback);
			setupParam.pOwner = this;

			setupParam.IsCheckFall = pCreateInfo->IsCheckFall;

			pMovementController->Setup(setupParam);
		}
	}

	if (auto* pShadowGoc = GetComponent<app::game::GOCShadowSimple>())
	{
		app::game::ShadowSphereShapeCInfo shadowInfo{ ms_ShadowRadius };
		shadowInfo.Unk2 = 5;
		shadowInfo.ShadowQuality = 3;

		app::game::GOCShadowSimple::Description description{ &shadowInfo };
		pShadowGoc->Setup(description);
		pShadowGoc->SetLocalOffsetPosition(ms_ShadowOffset);
		pShadowGoc->SetVisible(false);
	}

	app::game::GOCSound::SimpleSetup(this, 1, 1);

	if (GetComponent<app::game::GOCEffect>())
		app::game::GOCEffect::SimpleSetup(this, 1, false);

	app::fnd::GOComponent::EndSetup(*this);

	InitFSM();

	pCreateInfo = nullptr;
}