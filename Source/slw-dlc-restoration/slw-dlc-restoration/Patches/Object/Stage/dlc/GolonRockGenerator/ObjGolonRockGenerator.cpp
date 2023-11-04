#include "pch.h"
#include "ObjGolonRockGenerator.h"

void slw_dlc_restoration::ObjGolonRockGenerator::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	if (!Flags.test(1))
		return;

	ElapsedTime += in_rUpdateInfo.DeltaTime;

	if (Flags.test(2) || ElapsedTime <= CreateInterval)
		return;

	ElapsedTime = 0.0f;

	app::golon_rock::GolonRockCreateInfo createInfo{};
	createInfo.TransformMtx = GetComponent<app::fnd::GOCTransform>()->Frame.Unk3.Mtx;
	createInfo.Speed = -Speed;
	createInfo.IsCheckFall = Flags.test(0);

	golon_rock::CreateGolonRock(GetAllocator(), GetDocument(), createInfo);

	GetComponent<app::game::GOCSound>()->Play3D(ms_pSoundName, 0.0f);
	GetComponent<app::game::GOCEffect>()->CreateEffect(ms_pEffectName);
}