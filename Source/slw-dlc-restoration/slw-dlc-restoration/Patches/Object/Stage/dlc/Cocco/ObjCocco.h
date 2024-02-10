#pragma once

namespace slw_dlc_restoration
{
	class ObjCocco : public app::ObjCocco
	{
	public:
		typedef TiFsmState_t(app::ObjCocco::* BaseState)(const TiFsmEvent_t&);

	public:
		class CInfo : public app::ObjCocco::CInfo
		{
		public:
			uint PlayerNo{ static_cast<uint>(-1) };
		};

		uint PlayerNo{ static_cast<uint>(-1) };

		ObjCocco();
		ObjCocco(const CInfo& in_rCreateInfo);
		void AddCallback(app::GameDocument* in_pDocument) override;
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		TiFsmState_t StateIdle(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StateAttackIn(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StateAttackOut(const TiFsmEvent_t& in_rEvent);
		void SetTargetPlayer();
		bool IsInCamera() const;
		void CreateAttackers();

		static ObjCocco* CreateAttacker(const CInfo& in_rCreateInfo)
		{
			return new ObjCocco(in_rCreateInfo);
		}
	};
}