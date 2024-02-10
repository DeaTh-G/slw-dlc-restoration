#include "pch.h"
#include "StateUtil.h"

HOOK(void, __cdecl, ScatterRingForDamageHook, ASLR(0x008D8FF0), app::Player::CStateGOC& in_rStateGoc, bool in_unk, int in_count)
{
	if (in_rStateGoc.IsNowSuperSonic())
		return;

	if (app::Player::StateUtil::IsBarrier(in_rStateGoc))
	{
		originalScatterRingForDamageHook(in_rStateGoc, in_unk, in_count);
		return;
	}

	// This added code ensures that if the mod is configured to use the Heart System always the player does
	// not lose any rings upon contact damage. It also means that rings will no longer save them from damage.
	if (CONFIGURATION.ZeldaTweaks.IsAlwaysHeartLife)
	{
		app::Player::StateUtil::ReduceHeartLife(in_rStateGoc);
		return;
	}

	if (app::Player::StateUtil::ReduceHeartLife(in_rStateGoc))
		originalScatterRingForDamageHook(in_rStateGoc, in_unk, in_count);
}

HOOK(void, __cdecl, ScatterRingForDamageFixedCountHook, ASLR(0x008D9F50), app::Player::CStateGOC& in_rStateGoc, bool in_unk)
{
	if (in_rStateGoc.IsNowSuperSonic())
		return;

	if (app::Player::StateUtil::IsBarrier(in_rStateGoc))
	{
		originalScatterRingForDamageFixedCountHook(in_rStateGoc, in_unk);
		return;
	}

	// This added code ensures that if the mod is configured to use the Heart System always the player does
	// not lose any rings upon contact damage. It also means that rings will no longer save them from damage.
	if (CONFIGURATION.ZeldaTweaks.IsAlwaysHeartLife)
	{
		app::Player::StateUtil::ReduceHeartLife(in_rStateGoc);
		return;
	}

	if (app::Player::StateUtil::ReduceHeartLife(in_rStateGoc))
		originalScatterRingForDamageFixedCountHook(in_rStateGoc, in_unk);
}

void slw_dlc_restoration::Player::StateUtil::InstallHooks()
{
	// The Legend of Zelda Zone DLC Hooks
	INSTALL_HOOK(ScatterRingForDamageHook);
	INSTALL_HOOK(ScatterRingForDamageFixedCountHook);
}
