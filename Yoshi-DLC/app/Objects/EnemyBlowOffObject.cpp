#include "pch.h"

HOOK(char, __fastcall, EnemyBlowOffObjectOnDeadHook, ASLR(0x0072BA70), app::GameObject* This, void* edx, int a2)
{
	if (*((char*)This + 0x4A8) == 3 && !app::GameObject::IsKilled(This))
	{
		app::enemy::DeadEffectCInfo effectInfo;
		app::enemy::DeadEffectCInfo::__ct(&effectInfo);
		app::GameObjectHandleBase::__ct(&effectInfo, This);
		app::enemy::DeadEffectCInfo::SetYoshiIsland(&effectInfo);

		void* enemyManager = app::EnemyManager::GetService(*app::Document);
		app::EnemyManager::CreateDeadEffect(enemyManager, &effectInfo);
		app::GameObject::Kill(This);
	}

	return originalEnemyBlowOffObjectOnDeadHook(This, edx, a2);
}

void app::EnemyBlowOffObject::OnDead()
{
	INSTALL_HOOK(EnemyBlowOffObjectOnDeadHook);
}
