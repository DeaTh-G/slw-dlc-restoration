#pragma once

namespace slw_dlc_restoration
{
	class ObjGolonRock : public app::ObjGolonRock
	{
	public:
		ObjGolonRock(const app::golon_rock::GolonRockCreateInfo& in_rCreateInfo);
		void AddCallback(app::GameDocument* in_pDocument) override;
	};

	namespace golon_rock
	{
		inline static ObjGolonRock* CreateGolonRock(csl::fnd::IAllocator* in_pAllocator, app::GameDocument* in_pDocument, app::golon_rock::GolonRockCreateInfo& in_rCreateInfo)
		{
			auto* pObject = new slw_dlc_restoration::ObjGolonRock(in_rCreateInfo);
			if (!pObject)
				return { nullptr };

			in_pDocument->AddGameObject(pObject);
			return pObject;
		}
	}
}