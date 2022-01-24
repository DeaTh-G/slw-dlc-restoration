#include "pch.h"

HOOK(void, __fastcall, EnemyManagerCreateDeadEffectHook, ASLR(0x00728630), app::EnemyManager* This, void* edx, app::enemy::DeadEffectCInfo* effectInfo)
{
    originalEnemyManagerCreateDeadEffectHook(This, edx, effectInfo);

	if ((effectInfo->field_62 & 4) && effectInfo->field_10 >= 0 && app::AttackType::And(effectInfo->field_08, 0x20))
	{
		if ((effectInfo->field_62 & 0x20) || (effectInfo->field_62 & 0x40) || (effectInfo->field_62 & 0x100))
		{
			csl::math::Matrix34 m{};

			int* playerInfo = app::ObjUtil::GetPlayerInformation(*app::Document, effectInfo->field_10);

			app::math::Matrix34AffineTransformation(&m, (csl::math::Vector3*)playerInfo + 1, (csl::math::Quaternion*)playerInfo + 2);
			app::zelda_popupitem::ZeldaPopupItemCinfo createInfo { &m, app::ObjZeldaPopupItemType::HEART, effectInfo->field_10 };

			app::zelda_popupitem::CreateZeldaPopupItem((app::GameDocument&)(((int*)This)[10]), &createInfo);
		}
	}
}

void app::EnemyManager::CreateDeadEffectHook()
{
    INSTALL_HOOK(EnemyManagerCreateDeadEffectHook);
}