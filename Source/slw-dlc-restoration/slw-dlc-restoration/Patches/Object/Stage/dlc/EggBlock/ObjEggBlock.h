#pragma once

namespace slw_dlc_restoration
{
	class ObjEggBlock : public app::ObjEggBlock
	{
	public:
		uint HitPlayerNo{};

		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		bool ProcessMessage(app::fnd::Message& in_rMessage) override;
		bool ProcMsgDamage(app::xgame::MsgDamage& in_rMessage);
		void DoCheckPopEgg();
	};
}