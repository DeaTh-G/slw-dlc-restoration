#pragma once

namespace slw_dlc_restoration
{
	class ObjEgg;

	class EggManager : public app::EggManager
	{
	private:
		int NumPlayers{};
		csl::ut::ObjectMoveArray<csl::ut::ObjectMoveArray<app::GameObjectHandle<app::ObjEgg>>> Eggs{ GetAllocator() };
		csl::ut::ObjectMoveArray<csl::ut::CircularBuffer<LocusData>> Loci{ GetAllocator() };
		csl::ut::ObjectMoveArray<float> ElapsedTimes{ GetAllocator() };
		csl::ut::ObjectMoveArray<csl::ut::Bitset<uint8>> SpaceShrinkFlags{ GetAllocator() };

	public:
		EggManager();
		void StartGame(bool a1) override;
		bool AddEgg(app::GameObjectHandle<app::ObjEgg> in_egg, uint in_playerNo, size_t* out_pIndex);
		void AddSpaceCount(uint in_playerNo);
		void SubSpaceCount(uint in_playerNo);
		void UpdateLocusPos(float in_deltaTime);
		void UpdateEggSpace();
		void DoCheckReleaseAllEgg();
		void DoCheckClearAllEggAndExtrication();
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		bool IsEndExtrication(uint in_playerNo) const;
		void StartExtrication(uint in_playerNo);
		void SetForceSpaceShrink(bool in_isShrink, uint in_playerNo);
		bool IsLocusDataInGroundToAir(uint in_playerNo) const;
		bool CheckAccessLocusData(size_t in_index, uint in_playerNo, LocusData* out_pOutput) const;
		size_t GetTargetLocusIndex(size_t in_index, uint in_playerNo) const;
		LocusData GetTargetDataFromLocusIndex(size_t in_index, uint in_playerNo, bool* out_pIsValid, float* out_pNorm) const;
		LocusData GetTargetData(size_t in_index, uint in_playerNo, bool* out_pIsValid, float* out_pNorm) const;
		static void InstallPatches();
	};

	extern app::fnd::GameService* construct_MultiplayerEggManager(csl::fnd::IAllocator* in_pAllocator);
}