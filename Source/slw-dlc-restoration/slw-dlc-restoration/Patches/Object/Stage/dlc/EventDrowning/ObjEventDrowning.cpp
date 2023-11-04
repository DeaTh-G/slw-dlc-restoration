#include "pch.h"
#include "ObjEventDrowning.h"

void slw_dlc_restoration::ObjEventDrowning::AddCallback(app::GameDocument* in_pDocument)
{
	app::ObjEventDrowning::AddCallback(in_pDocument);

	auto* pEffectCollider = GetComponent<app::game::GOCCollider>()->GetShapeById(1);
	pEffectCollider->SetLocalTranslation({ pEffectCollider->UnkMtx.GetTransVector() / 2.0f });
}

bool slw_dlc_restoration::ObjEventDrowning::ProcessMessage(app::fnd::Message& in_rMessage)
{
	if (PreProcessMessage(in_rMessage))
		return true;

	switch (in_rMessage.GetType())
	{
	case app::xgame::MsgHitEventCollision::MessageID:		return ProcMsgHitEventCollision(static_cast<app::xgame::MsgHitEventCollision&>(in_rMessage));
	default:												return app::ObjEventDrowning::ProcessMessage(in_rMessage);
	}
}

bool slw_dlc_restoration::ObjEventDrowning::ProcMsgHitEventCollision(app::xgame::MsgHitEventCollision& in_rMessage)
{
	if (!app::ObjUtil::CheckShapeUserID(in_rMessage.pSelf, 1))
		return app::ObjEventDrowning::ProcMsgHitEventCollision(in_rMessage);

	auto* pParam = GetAdapter()->GetData<app::SEventDrowningParam>();

	csl::math::Matrix34 transformMtx = GetComponent<app::fnd::GOCTransform>()->Frame.Unk3.Mtx;
	csl::math::Vector3 upVector{ transformMtx.GetColumn(1) };
	csl::math::Vector3 position = transformMtx.GetTransVector();

	csl::math::Vector3 contactDistance{ *in_rMessage.GetContactPointPosition() - position };
	csl::math::Vector3 contactPosition{ position + contactDistance - csl::math::Vector3(upVector * contactDistance.dot(upVector)) };

	transformMtx.SetTransVector({ contactPosition + upVector * pParam->EffectOffset / 2.0f });
	GetComponent<app::game::GOCEffect>()->CreateEffectWorld(ms_pEffectName, transformMtx, 1.0f);
	GetComponent<app::game::GOCSound>()->Play3D(ms_pSoundName, contactPosition, 0.0f);

	return true;
}