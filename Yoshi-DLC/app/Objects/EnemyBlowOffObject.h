#pragma once

namespace app
{
	HOOK(char, __fastcall, EnemyBlowOffObjectOnDeadHook, ASLR(0x0072BA70), GameObject* This, void* edx, int a2)
	{
		if (*((char*)This + 0x4A8) == 3 && !GameObject::IsKilled(This))
		{
			enemy::DeadEffectCInfo effectInfo;
			enemy::DeadEffectCInfo::__ct(&effectInfo);
			GameObjectHandleBase::__ct(&effectInfo, This);
			enemy::DeadEffectCInfo::SetYoshiIsland(&effectInfo);

			void* enemyManager = EnemyManager::GetService(*Document);
			EnemyManager::CreateDeadEffect(enemyManager, &effectInfo);
			GameObject::Kill(This);
		}

		return originalEnemyBlowOffObjectOnDeadHook(This, edx, a2);
	}

	class EnemyBlowOffObject
	{
	public:
		static void OnDead()
		{
			INSTALL_HOOK(EnemyBlowOffObjectOnDeadHook);
		}
	};
}