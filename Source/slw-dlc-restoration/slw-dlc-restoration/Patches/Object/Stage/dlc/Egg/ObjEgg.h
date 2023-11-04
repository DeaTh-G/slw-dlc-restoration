#pragma once

namespace slw_dlc_restoration
{
	namespace egg
	{
		struct EggCInfo : public app::egg::EggCInfo
		{
			uint PlayerNo{};
		};
	}

	class ObjEgg : public app::ObjEgg
	{
	public:
		typedef TiFsmState_t(app::ObjEgg::* BaseState)(const TiFsmEvent_t&);

	public:
		uint PlayerNo{};

		ObjEgg(const egg::EggCInfo* in_pInfo);
		void AddCallback(app::GameDocument* in_pDocument) override;
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		TiFsmState_t StateToFirstLocus(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StateToIndexLocus(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StateMoveToExtrication(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StateMoveIndexLocus(const TiFsmEvent_t& in_rEvent);
	};
}

namespace slw_dlc_restoration::egg
{
	extern slw_dlc_restoration::ObjEgg* CreateEgg(app::GameDocument& in_rDocument, const EggCInfo& in_rCreateInfo);
}