#include "pch.h"
#include "EggManager.h"

slw_dlc_restoration::EggManager::EggManager() : app::EggManager()
{
	
}

void slw_dlc_restoration::EggManager::StartGame(bool a1)
{
	NumPlayers = pOwnerDocument->GetService<app::CLevelInfo>()->GetNumPlayers();

	Loci.clear();
	Loci.reserve(NumPlayers);
	for (size_t i = 0; i < Loci.capacity(); i++)
	{
		Loci[i].clear();
		Loci[i].pAllocator = csl::fnd::Singleton<app::GameObjectSystem>::GetInstance()->pPooledAllocator;
		Loci[i].set_capacity(app::egg::GetMaxLocusPositionNum());
	}

	Eggs.clear();
	Eggs.resize(NumPlayers);
	for (size_t i = 0; i < Eggs.capacity(); i++)
	{
		Eggs[i].clear();
		Eggs[i].change_allocator(GetAllocator());
		Eggs[i].reserve(12);
	}

	ElapsedTimes.clear();
	ElapsedTimes.reserve(NumPlayers);
	for (size_t i = 0; i < ElapsedTimes.capacity(); i++)
	{
		ElapsedTimes[i] = 0.0f;
	}

	SpaceShrinkFlags.clear();
	SpaceShrinkFlags.reserve(NumPlayers);
	for (size_t i = 0; i < SpaceShrinkFlags.capacity(); i++)
	{
		SpaceShrinkFlags[i].reset();
	}

	app::EggManager::StartGame(a1);
}

bool slw_dlc_restoration::EggManager::AddEgg(app::GameObjectHandle<app::ObjEgg> in_egg, uint in_playerNo, size_t* out_pIndex)
{
	if (Eggs[in_playerNo].size() >= 12)
		return false;

	*out_pIndex = Eggs[in_playerNo].size();
	Eggs[in_playerNo].push_back(in_egg);

	return true;
}

void slw_dlc_restoration::EggManager::AddSpaceCount(uint in_playerNo)
{
	for (auto egg : Eggs[in_playerNo])
	{
		if (egg.IsValid())
		{
			bool added = egg.Get()->AddSpaceOffset();
			if (added)
				break;
		}
	}

	SpaceShrinkFlags[in_playerNo].set(2);
}

void slw_dlc_restoration::EggManager::SubSpaceCount(uint in_playerNo)
{
	for (auto egg : Eggs[in_playerNo])
	{
		if (egg.IsValid())
		{
			bool removed = egg.Get()->SubSpaceOffset();
			if (removed)
				break;
		}
	}
	
	SpaceShrinkFlags[in_playerNo].set(2);
}

void slw_dlc_restoration::EggManager::UpdateLocusPos(float in_deltaTime)
{
	for (size_t i = 0; i < NumPlayers; i++)
	{
		auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*pOwnerDocument, i);
		if (!pPlayerInfo)
			continue;

		if (Loci[i].size() <= 1)
		{
			LocusData locus{};
			locus.Position = pPlayerInfo->Position;
			locus.Rotation = pPlayerInfo->Rotation;
			locus.IsAirborne = !pPlayerInfo->IsOnGround;

			Loci[i].push_front(locus);
			continue;
		}

		float firstLocusDist = csl::math::Vector3(pPlayerInfo->Position - Loci[i].at(0).Position).squaredNorm();
		float secondLocusDist = csl::math::Vector3(Loci[i].at(0).Position - Loci[i].at(1).Position).squaredNorm();

		if (firstLocusDist >= 0.09f || (firstLocusDist - secondLocusDist) > 0.000001f)
		{
			ElapsedTimes[i] = 0.3f;
			LocusData locus{};
			locus.Position = pPlayerInfo->Position;
			locus.Rotation = pPlayerInfo->Rotation;
			locus.IsAirborne = !pPlayerInfo->IsOnGround;

			Loci[i].push_front(locus);
			continue;
		}

		ElapsedTimes[i] -= in_deltaTime;
		if (ElapsedTimes[i] >= 0.0f || IsLocusDataInGroundToAir(i))
		{
			LocusData locus{};
			locus.Position = pPlayerInfo->Position;
			locus.Rotation = pPlayerInfo->Rotation;
			locus.IsAirborne = !pPlayerInfo->IsOnGround;

			Loci[i].push_front(locus);
			continue;
		}
	}
}

void slw_dlc_restoration::EggManager::UpdateEggSpace()
{
	for (size_t i = 0; i < NumPlayers; i++)
	{
		SpaceShrinkFlags[i].reset(2);
		auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*pOwnerDocument, i);
		if (pPlayerInfo && pPlayerInfo->IsLandOnMovableGround)
		{
			SubSpaceCount(i);
			continue;
		}

		if (SpaceShrinkFlags[i].test(0))
		{
			SubSpaceCount(i);
			continue;
		}

		if (ElapsedTimes[i] > 0.0f)
			AddSpaceCount(i);
	}
}

void slw_dlc_restoration::EggManager::DoCheckReleaseAllEgg()
{
	for (size_t i = 0; i < NumPlayers; i++)
	{
		auto* pPlayerInfo = app::ObjUtil::GetPlayerInformation(*pOwnerDocument, i);
		if (!pPlayerInfo)
			continue;

		if (!pPlayerInfo->IsTimerDisabled)
		{
			SpaceShrinkFlags[i].reset(3);
			continue;
		}

		if (SpaceShrinkFlags[i].test(3))
			continue;

		for (auto egg : Eggs[i])
			egg.Get()->StartDrop();

		Eggs[i].clear();
		SpaceShrinkFlags[i].set(3);
	}
}

void slw_dlc_restoration::EggManager::DoCheckClearAllEggAndExtrication()
{
	for (size_t i = 0; i < NumPlayers; i++)
	{
		if (!SpaceShrinkFlags[i].test(1))
			continue;
		
		bool ended = true;
		for (auto egg : Eggs[i])
		{
			ended = ended && egg.Get()->IsEndExtrication();
			if (!ended)
				break;
		}

		if (!ended)
			continue;

		for (auto egg : Eggs[i])
			egg.Get()->Kill();

		Eggs[i].clear();
		SpaceShrinkFlags[i].reset(1);
	}
}

void slw_dlc_restoration::EggManager::Update(const app::fnd::SUpdateInfo& in_rUpdateInfo)
{
	UpdateLocusPos(in_rUpdateInfo.DeltaTime);
	UpdateEggSpace();
	DoCheckReleaseAllEgg();
	DoCheckClearAllEggAndExtrication();
}

bool slw_dlc_restoration::EggManager::IsEndExtrication(uint in_playerNo) const
{
	return Eggs[in_playerNo].size() == 0;
}

void slw_dlc_restoration::EggManager::StartExtrication(uint in_playerNo)
{
	for (auto egg : Eggs[in_playerNo])
		egg.Get()->StartExtrication();

	SpaceShrinkFlags[in_playerNo].set(1);
}

void slw_dlc_restoration::EggManager::SetForceSpaceShrink(bool in_isShrink, uint in_playerNo)
{
	SpaceShrinkFlags[in_playerNo].set(0, in_isShrink);
}

bool slw_dlc_restoration::EggManager::IsLocusDataInGroundToAir(uint in_playerNo) const
{
	bool result{};
	bool isInAir{};

	for (size_t i = 0; i <= Eggs[in_playerNo].size() * 10; i++)
	{
		EggManager::LocusData locus{};

		if (CheckAccessLocusData(i, in_playerNo, &locus))
		{
			if (locus.IsAirborne)
			{
				if (isInAir)
					result = true;
			}
			else
			{
				isInAir = true;
			}
		}
	}

	return result;
}

bool slw_dlc_restoration::EggManager::CheckAccessLocusData(size_t in_index, uint in_playerNo, LocusData* out_pOutput) const
{
	size_t lociSize = Loci[in_playerNo].size();

	if (!lociSize)
		return false;
	
	if (in_index < lociSize)
		*out_pOutput = Loci[in_playerNo].at(in_index);
	else
		*out_pOutput = Loci[in_playerNo].back();

	return true;
}

size_t slw_dlc_restoration::EggManager::GetTargetLocusIndex(size_t in_index, uint in_playerNo) const
{
	size_t index{};

	for (size_t i = 0; i <= in_index; i++)
	{
		if (Eggs[in_playerNo][i].IsValid())
			index += Eggs[in_playerNo][i].Get()->SpaceOffset;
	}

	return index;
}

app::EggManager::LocusData slw_dlc_restoration::EggManager::GetTargetDataFromLocusIndex(size_t in_index, uint in_playerNo, bool* out_pIsValid, float* out_pNorm) const
{
	LocusData previousLocus{};
	LocusData currentLocus{};

	if (CheckAccessLocusData(in_index, in_playerNo, &currentLocus))
	{
		if (!out_pIsValid)
			return currentLocus;

		if ((int)in_index - 1 < 0)
			return currentLocus;

		if (!CheckAccessLocusData(in_index - 1, in_playerNo, &previousLocus))
			return currentLocus;

		csl::math::Vector3 positionOffset = { currentLocus.Position - previousLocus.Position };
		float offset = positionOffset.norm();
		if (offset > 0.3f && (ElapsedTimes[in_playerNo] > 0.0f || SpaceShrinkFlags[in_playerNo].test(2)))
		{
			*out_pIsValid = true;
			if (out_pNorm)
				*out_pNorm = offset;
		}
	}

	return currentLocus;
}

app::EggManager::LocusData slw_dlc_restoration::EggManager::GetTargetData(size_t in_index, uint in_playerNo, bool* out_pIsValid, float* out_pNorm) const
{
	return GetTargetDataFromLocusIndex(GetTargetLocusIndex(in_index, in_playerNo), in_playerNo, out_pIsValid, out_pNorm);
}

app::fnd::GameService* slw_dlc_restoration::construct_MultiplayerEggManager(csl::fnd::IAllocator* in_pAllocator)
{
	return new(in_pAllocator) slw_dlc_restoration::EggManager();
}

void slw_dlc_restoration::EggManager::InstallPatches()
{
	// Yoshi's Island Zone DLC Patches
	// Replace stubbed construct function in the GameServiceClass constructor for the static class of EggManager
	if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
	{
		WRITE_MEMORY(ASLR(0x00D41253), void*, slw_dlc_restoration::construct_MultiplayerEggManager);
	}
	else
	{
		WRITE_MEMORY(ASLR(0x00D41253), void*, construct_EggManager);
	}

	// Add EggManager to GameModeStage::CreateService Array
	WRITE_MEMORY(ASLR(0x00E05B8C), int32_t, ASLR(0x00FEE764));

	// Replace Junk Data with null padding after Array
	WRITE_MEMORY(ASLR(0x00E05B90), int32_t, 0);

	// Add EggManager to GameModeStageBattle::CreateService Array
	WRITE_MEMORY(ASLR(0x00E09D0C), int32_t, ASLR(0x00FEE764));

	// Replace Junk Data with null padding after Array
	WRITE_MEMORY(ASLR(0x00E09D10), int32_t, 0);
}