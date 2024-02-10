#include "pch.h"
#include "ObjEgg.h"

slw_dlc_restoration::ObjEgg::ObjEgg(const slw_dlc_restoration::egg::EggCInfo* in_pInfo) : app::ObjEgg(in_pInfo)
{
    PlayerNo = in_pInfo->PlayerNo;

    FSM_INIT(static_cast<BaseState>(&ObjEgg::StateToFirstLocus));
}

void slw_dlc_restoration::ObjEgg::AddCallback(app::GameDocument* in_pDocument)
{
    app::fnd::GOComponent::Create<app::fnd::GOCVisualModel>(*this);
    app::fnd::GOComponent::Create<app::game::GOCCollider>(*this);
    app::fnd::GOComponent::Create<app::game::GOCGravity>(*this);
    app::fnd::GOComponent::Create<app::game::GOCEffect>(*this);
    app::fnd::GOComponent::Create<app::game::GOCSound>(*this);

    app::fnd::GOComponent::BeginSetup(*this);

    bool eggSuccess{};
    if (auto* pEggManager = in_pDocument->GetService<slw_dlc_restoration::EggManager>())
        eggSuccess = pEggManager->AddEgg(this, PlayerNo, &Index);

    if (auto* pTransform = GetComponent<app::fnd::GOCTransform>())
    {
        pTransform->SetLocalTranslation(pCreateInfo->Mtx.GetTransVector());
        pTransform->SetLocalRotation(pCreateInfo->Mtx.GetRotation());
    }

    auto* pInfo = app::ObjUtil::GetObjectInfo<app::ObjEggInfo>(*in_pDocument);

    if (auto* pVisualModel = GetComponent<app::fnd::GOCVisualModel>())
    {
        app::fnd::GOCVisualModel::Description description{};
        description.Model = pInfo->ModelContainer.Models[Type];
        description.Unk2 |= 0x400000u;
        description.ZOffset = -0.2f * Index - 2.0f;

        pVisualModel->Setup(description);
        pVisualModel->SetLocalScale(ms_Scale);

        csl::math::Quaternion q(Eigen::AngleAxisf(MATHF_PI * 0.5f, csl::math::Vector3::UnitY()));
        pVisualModel->SetLocalRotation({ q.normalized() });
    }

    if (auto* pCollider = GetComponent<app::game::GOCCollider>())
    {
        pCollider->Setup({ ms_ShapeCount });

        app::game::ColliSphereShapeCInfo collisionInfo{};
        collisionInfo.ShapeType = app::game::CollisionShapeType::ShapeType::eShapeType_Sphere;
        collisionInfo.MotionType = app::game::PhysicsMotionType::MotionType::eMotionType_Value2;
        collisionInfo.Radius = ms_CollisionRadius;
        app::ObjUtil::SetupCollisionFilter(app::ObjUtil::eFilter_Unk2, collisionInfo);
        pCollider->CreateShape(collisionInfo);
    }

    if (auto* pGravity = GetComponent<app::game::GOCGravity>())
        app::game::GOCGravity::SimpleSetup(this, 1);

    if (auto* pEffect = GetComponent<app::game::GOCEffect>())
        app::game::GOCEffect::SimpleSetup(this, 0, 0);

    if (auto* pSound = GetComponent<app::game::GOCSound>())
        app::game::GOCSound::SimpleSetup(this, 0, 0);

    app::fnd::GOComponent::EndSetup(*this);

    pCreateInfo = nullptr;
    InitFSM();

    if (eggSuccess)
    {
        if (auto* pSound = GetComponent<app::game::GOCSound>())
            pSound->Play(ms_pGetSoundName, 0.0f);
    }
    else
    {
        app::xgame::MsgTakeObject msg{ app::xgame::MsgTakeObject::eType_OneUp };
        if (app::ObjUtil::SendMessageImmToPlayer(*this, msg) && msg.Taken)
        {
            if (auto* pEffect = GetComponent<app::game::GOCEffect>())
                pEffect->CreateEffect(ms_pBirthEffectName);

            if (auto* pSound = GetComponent<app::game::GOCSound>())
                pSound->Play(ms_pOneUpSoundName, 0.0f);
        }

        Kill();
    }
}

void slw_dlc_restoration::ObjEgg::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
    if (FSM_STATE() == &app::ObjEgg::StateMoveToExtrication)
        ChangeState(static_cast<BaseState>(&ObjEgg::StateMoveToExtrication));

    DispatchFSM(app::TiFsmBasicEvent<app::ObjEgg>::CreateUpdate(in_rUpdateInfo.DeltaTime));
}

app::TTinyFsm<app::ObjEgg>::TiFsmState_t slw_dlc_restoration::ObjEgg::StateToFirstLocus(const TiFsmEvent_t& in_rEvent)
{
    switch (in_rEvent.getSignal())
    {
    case TiFSM_SIGNAL_ENTER:
    {
        ElapsedFrames = 0;
        if (auto* pLevelInfo = GetDocument()->GetService<app::CLevelInfo>())
        {
            if (pLevelInfo->StageFlags.test(16))
            {
                Time = 30.0f / 30.0f;
            }
            else
            {
                Time = 30.0f / 60.0f;
            }
        }

        return {};
    }
    case TiFSM_SIGNAL_UPDATE:
    {
        auto* pEggManager = GetDocument()->GetService<slw_dlc_restoration::EggManager>();
        if (!pEggManager)
            return {};

        auto* pTransform = GetComponent<app::fnd::GOCTransform>();
        if (!pTransform)
            return {};

        auto targetLocus = pEggManager->GetTargetDataFromLocusIndex(0, PlayerNo, nullptr, nullptr);

        auto position = pTransform->Frame.Unk3.GetTranslation();

        pTransform->SetLocalTranslation({ (targetLocus.Position - position) * csl::math::Clamp(ElapsedFrames / 30.0f, 0.0f, 1.0f) + position });
        UpdateRotation(targetLocus.Rotation, in_rEvent.getFloat());
        UpdateSlippery(targetLocus.IsAirborne, true, in_rEvent.getFloat(), 0.0f);

        ++ElapsedFrames;
        if (ElapsedFrames > 30)
        {
            ChangeState(static_cast<BaseState>(&ObjEgg::StateToIndexLocus));
        }

        return {};
    }
    default:
        return FSM_TOP();
    }
}

app::TTinyFsm<app::ObjEgg>::TiFsmState_t slw_dlc_restoration::ObjEgg::StateToIndexLocus(const TiFsmEvent_t& in_rEvent)
{
    switch (in_rEvent.getSignal())
    {
    case TiFSM_SIGNAL_ENTER:
    {
        ElapsedFrames = 0;

        return {};
    }
    case TiFSM_SIGNAL_UPDATE:
    {
        auto* pEggManager = GetDocument()->GetService<slw_dlc_restoration::EggManager>();
        if (!pEggManager)
            return {};

        auto* pTransform = GetComponent<app::fnd::GOCTransform>();
        if (!pTransform)
            return {};

        auto locusIndex = pEggManager->GetTargetLocusIndex(Index, PlayerNo);
        auto index = ElapsedFrames;
        if (locusIndex < index)
            index = locusIndex;

        auto targetLocus = pEggManager->GetTargetDataFromLocusIndex(index, PlayerNo, nullptr, nullptr);

        pTransform->SetLocalTranslation(targetLocus.Position);
        UpdateRotation(targetLocus.Rotation, in_rEvent.getFloat());
        UpdateSlippery(targetLocus.IsAirborne, true, in_rEvent.getFloat(), 0.0f);

        ++ElapsedFrames;
        if (locusIndex < ElapsedFrames)
        {
            ChangeState(static_cast<BaseState>(&ObjEgg::StateMoveIndexLocus));
        }

        return {};
    }
    default:
        return FSM_TOP();
    }
}

app::TTinyFsm<app::ObjEgg>::TiFsmState_t slw_dlc_restoration::ObjEgg::StateMoveToExtrication(const TiFsmEvent_t& in_rEvent)
{
    switch (in_rEvent.getSignal())
    {
    case TiFSM_SIGNAL_ENTER:
    {
        return {};
    }
    case TiFSM_SIGNAL_UPDATE:
    {
        bool isExtricationReady = true;

        auto* pEggManager = GetDocument()->GetService<slw_dlc_restoration::EggManager>();
        if (!pEggManager)
            return {};

        auto locusIndex = pEggManager->GetTargetLocusIndex(Index, PlayerNo);
        if (locusIndex)
        {
            for (size_t i = 0; i < locusIndex;)
            {
                if (pEggManager->GetTargetDataFromLocusIndex(i, PlayerNo, nullptr, nullptr).IsAirborne)
                {
                    isExtricationReady = false;
                    break;
                }

                if (++i >= locusIndex)
                    break;
            }
        }

        auto targetLocus = pEggManager->GetTargetDataFromLocusIndex(locusIndex, PlayerNo, nullptr, nullptr);

        auto* pTransform = GetComponent<app::fnd::GOCTransform>();
        if (!pTransform)
            return {};

        pTransform->SetLocalTranslation(targetLocus.Position);
        UpdateRotation(targetLocus.Rotation, in_rEvent.getFloat());
        UpdateSlippery(targetLocus.IsAirborne, true, in_rEvent.getFloat(), 0.0f);

        if (targetLocus.IsAirborne)
            return {};

        if (isExtricationReady)
        {
            Extrication();
            ChangeState(&ObjEgg::StateAfterExtrication);
        }

        return {};
    }
    default:
        return FSM_TOP();
    }
}

app::TTinyFsm<app::ObjEgg>::TiFsmState_t slw_dlc_restoration::ObjEgg::StateMoveIndexLocus(const TiFsmEvent_t& in_rEvent)
{
    switch (in_rEvent.getSignal())
    {
    case TiFSM_SIGNAL_ENTER:
    {
        return {};
    }
    case TiFSM_SIGNAL_UPDATE:
    {
        auto* pEggManager = GetDocument()->GetService<slw_dlc_restoration::EggManager>();
        if (!pEggManager)
            return {};

        auto* pTransform = GetComponent<app::fnd::GOCTransform>();
        if (!pTransform)
            return {};

        bool isValidLocus{};
        float locusNorm{};
        auto targetLocus = pEggManager->GetTargetData(Index, PlayerNo, &isValidLocus, &locusNorm);

        pTransform->SetLocalTranslation(targetLocus.Position);
        UpdateRotation(targetLocus.Rotation, in_rEvent.getFloat());
        UpdateSlippery(targetLocus.IsAirborne, isValidLocus, in_rEvent.getFloat(), locusNorm);

        return {};
    }
    default:
        return FSM_TOP();
    }
}

slw_dlc_restoration::ObjEgg* slw_dlc_restoration::egg::CreateEgg(app::GameDocument& in_rDocument, const slw_dlc_restoration::egg::EggCInfo& in_rCreateInfo)
{
    auto* pObject = new slw_dlc_restoration::ObjEgg(&in_rCreateInfo);
    if (!pObject)
        return { nullptr };

    in_rDocument.AddGameObject(pObject);
    return pObject;
}