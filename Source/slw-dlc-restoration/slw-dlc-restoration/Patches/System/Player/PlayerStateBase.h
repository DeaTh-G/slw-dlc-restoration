#pragma once

namespace slw_dlc_restoration::Player
{
	class CStateBase : public app::Player::CStateBase
	{
	public:
		bool ProcMsgTakeObject(app::Player::CStateGOC& in_rStateGoc, app::xgame::MsgTakeObject& in_rMessage);
		bool ProcMsgPlayerReachGoal(app::Player::CStateGOC& in_rStateGoc, app::xgame::MsgPlayerReachGoal& in_rMessage);

		bool CheckHitDamage(app::Player::CStateGOC& in_rStateGoc, bool* out_pDamaged) const;

		static void InstallHooks();
	};

	class CStateSonicBase : public app::Player::CStateSonicBase
	{
	public:
		static void InstallHooks();
	};
}