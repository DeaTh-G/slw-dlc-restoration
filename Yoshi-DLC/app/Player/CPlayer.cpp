#include "pch.h"
#include "CPlayer.h"

HOOK(int*, __fastcall, UpdatePlayerInformationHook, ASLR(0x00851F20), int* This)
{
    int* result = originalUpdatePlayerInformationHook(This);
    
    void* service = app::xgame::Blackboard::GetService();
    int* blackBoard = app::xgame::Blackboard::GetPlayerInformation(This, service);
    
    /* Store IsMovingGround because Sonic Team felt like yeeting that shit. */
    if (*((int*)This[0xD2] + 9) >= 0)
    {
        int* playerInfo = *(int**)(blackBoard + 1 * *((int*)This[0xD2] + 9) + 0xF);

        if (playerInfo)
        {
            int* cStateGOC = app::CGOCCollectionImpl::GetGOC((void*)(This + 0xCB), CStateGOC);
            unsigned int isMovGround = app::Player::StateUtil::IsLandOnMovableGround(cStateGOC);
            *((char*)playerInfo + 0x145) = isMovGround;

            /* Store Heart Count for the Player */
            app::Player::PluginStateHeartLife* pluginState = (app::Player::PluginStateHeartLife*)app::Player::CStateGOC::GetStatePluginPtr(cStateGOC, PluginStateHeartLifeString);
            if (!pluginState)
                return result;

            if (MAX_NUM_HEARTS < AFTER_DEATH_MAX_NUM_HEARTS)
            {
                app::Player::PluginStateHeartLife::SetMaxNumHearts(pluginState, AFTER_DEATH_MAX_NUM_HEARTS);
                app::Player::PluginStateHeartLife::SetNumHearts(pluginState, AFTER_DEATH_MAX_NUM_HEARTS);
            }
            NUM_HEARTS = app::Player::PluginStateHeartLife::GetNumHearts(pluginState);
            MAX_NUM_HEARTS = app::Player::PluginStateHeartLife::GetMaxNumHearts(pluginState);
        }
    }

    return result;
}

HOOK(bool, __fastcall, CPlayerProcessMessageHook, ASLR(0x008514B0), int* This, void* edx, app::fnd::Message& message)
{
    if (message.Type == app::fnd::PROC_MSG_DLC_ZELDA_TAKE_HEART)
    {
        app::Player::CStateGOC* cStateGOC = (app::Player::CStateGOC*)app::CGOCCollectionImpl::GetGOC((void*)(This + 0xC9), CStateGOC);
        app::Player::StateUtil::RecoveryHeartLife(cStateGOC);
        return 1;
    }
    else if (message.Type == app::fnd::PROC_MSG_DLC_ZELDA_TAKE_HEART_CONTAINER)
    {
        app::Player::CStateGOC* cStateGOC = (app::Player::CStateGOC*)app::CGOCCollectionImpl::GetGOC((void*)(This + 0xC9), CStateGOC);
        app::Player::StateUtil::IncrementMaxHeartLife(cStateGOC);
        app::HUD::DO_RECOVER_LIFE = true;
        return 1;
    }
    else if (message.Type == app::fnd::PROC_MSG_DLC_ZELDA_HEART_ALL_RECOVERY)
    {
        app::Player::CStateGOC* cStateGOC = (app::Player::CStateGOC*)app::CGOCCollectionImpl::GetGOC((void*)(This + 0xC9), CStateGOC);
        app::Player::StateUtil::AllRecoveryHeartLife(cStateGOC);
        app::HUD::DO_RECOVER_LIFE = true;
        return 1;
    }

    return originalCPlayerProcessMessageHook(This, edx, message);
}

void app::Player::CPlayer::ProcessMessage()
{
    INSTALL_HOOK(CPlayerProcessMessageHook);
}

void app::Player::CPlayer::UpdatePlayerInformation()
{
    INSTALL_HOOK(UpdatePlayerInformationHook);
}