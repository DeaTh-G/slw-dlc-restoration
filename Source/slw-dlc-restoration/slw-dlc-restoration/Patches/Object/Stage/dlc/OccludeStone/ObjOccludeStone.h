#pragma once

namespace slw_dlc_restoration
{
	class ObjOccludeStone : public app::ObjOccludeStone
	{
	public:
		bool ProcessMessage(app::fnd::Message& in_rMessage) override;
		bool ProcMsgDamage(app::xgame::MsgDamage& in_rMessage);
	};
}