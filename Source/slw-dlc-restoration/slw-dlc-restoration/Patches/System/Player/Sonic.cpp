#include "pch.h"
#include "Sonic.h"

HOOK(void, __fastcall, SendPlayerInfoHook, ASLR(0x00861A90), app::Player::CSonic* in_pThis, void* edx)
{
	originalSendPlayerInfoHook(in_pThis, edx);

	// Sonic doesn't send the number of hearts and the maximum the player can have to the game mode on the PC port's original code,
	// due to The Legend of Zelda DLC not existing on it. This means that when the app::xgame::MsgPLSendGameInfo message is sent,
	// the player's heart count and maximum heart count is sent as 0 as part of the original code, while it should be taking the value
	// from PluginStateHeartLife. The message is intercepted before its sent off to the game mode and this information is filled in.
	auto* pStateGoc = in_pThis->GetStateGOC();
	auto* pHeartLifePlugin = pStateGoc->GetStatePlugin<app::Player::PluginStateHeartLife>();
	if (!pHeartLifePlugin)
		return;

	for (auto& pMessage : in_pThis->pMessageManager->Messages)
	{
		if (pMessage->GetType() == app::xgame::MsgPLSendGameInfo::MessageID)
		{
			static_cast<app::xgame::MsgPLSendGameInfo*>(pMessage)->NumHearts = pHeartLifePlugin->GetNumHearts();
			static_cast<app::xgame::MsgPLSendGameInfo*>(pMessage)->MaxNumHearts = pHeartLifePlugin->GetMaxNumHearts();
		}
	}
}

HOOK(void, __fastcall, AddCallbackHook, ASLR(0x00861B00), app::Player::CSonic* in_pThis, void* edx, app::GameDocument& in_rDocument)
{
	originalAddCallbackHook(in_pThis, edx, in_rDocument);

	// The following code ensures that after the Heart System Plugin's flag has been set it does not get created
	// when the mod's configuration does not desire it to be there.
	auto* pStateGoc = in_pThis->GetStateGOC();
	if (!in_pThis->GetLevelInfo()->StageFlags.test(19) && !CONFIGURATION.ZeldaTweaks.IsAlwaysHeartLife)
	{
		if (pStateGoc->GetStatePlugin<app::Player::PluginStateHeartLife>())
			pStateGoc->RemoveStatePlugin(17);
	}
	else
	{
		if (!pStateGoc->GetStatePlugin<app::Player::PluginStateHeartLife>())
			pStateGoc->AddStatePlugin(17);
	}
}

void slw_dlc_restoration::Player::CSonic::InstallHooks()
{
	// The Legend of Zelda Zone DLC Hooks
	INSTALL_HOOK(SendPlayerInfoHook);
	INSTALL_HOOK(AddCallbackHook);
}
