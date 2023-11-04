#include "pch.h"
#include "GameModeStageBattle.h"

typedef app::TTinyFsm<app::GameModeStageBattle, app::GameModeUtil::Event<app::GameModeStageBattle>, false>::TiFsmState_t TiFsmState_t;

HOOK(void, __fastcall, LoadStartingCallbackHook, ASLR(0x0093A6D0), app::GameModeStageBattle::MyStageDataListener* in_pThis, void* edx, app::StageDataListener::EventType in_eventType)
{
    originalLoadStartingCallbackHook(in_pThis, edx, in_eventType);
    
    // The UI packfiles for the DLC loading screens are loaded here as to ensure that they can be displayed when the game
    // is loading those levels in multiplayer.
    if (in_eventType != app::StageDataListener::EventType::eEventType_Start)
        return;

    app::fnd::FileLoaderParam param{};

    param.SetMultiLanguageAttr(true, 0);
    in_pThis->pOwner->LoadFile("ui/ui_zdlc01_load_all.pac", param);
    param.SetMultiLanguageAttr(false, 0);

    param.SetMultiLanguageAttr(true, 0);
    in_pThis->pOwner->LoadFile("ui/ui_zdlc02_load_all.pac", param);
    param.SetMultiLanguageAttr(false, 0);

    param.SetMultiLanguageAttr(true, 0);
    in_pThis->pOwner->LoadFile("ui/ui_zdlc03_load_all.pac", param);
    param.SetMultiLanguageAttr(false, 0);
}

HOOK(void, __fastcall, LoadLevelHook, ASLR(0x00939E50), app::GameModeStageBattle* in_pThis, void* edx)
{
    originalLoadLevelHook(in_pThis, edx);

    // Ensure that the gameplay UI packfile for the Yoshi's Island Zone DLC is loaded.
    // This is done to be able to call the HUD transition *.swif used by the pipes in multiplayer.
    app::fnd::FileLoaderParam param{};
    param.pAllocator = app::game::GlobalAllocator::GetAllocator(1);
    param.SetMultiLanguageAttr(true, 0);
    in_pThis->LoadFile("ui/ui_zdlc02_gamemodestage.pac", param);
    param.SetMultiLanguageAttr(false, 0);
}

HOOK(void, __fastcall, InitFirstHook, ASLR(0x00939770), app::GameModeStageBattle* in_pThis, void* edx)
{
    originalInitFirstHook(in_pThis, edx);

    if (CONFIGURATION.ZeldaTweaks.IsVirtualLinkSonic)
    {
        auto* pStageData = in_pThis->pStageData;
        if (!pStageData->IsDLCStage())
            return;

        if (pStageData->IsYoshiIslandStage())
            in_pThis->pLevelInfo->SetDLCStage(2);
        else if (pStageData->IsZeldaStage())
            in_pThis->pLevelInfo->SetDLCStage(3);
        else
            in_pThis->pLevelInfo->SetDLCStage(1);
    }
}

HOOK(void, __fastcall, ResetStageHook, ASLR(0x009395E0), app::GameModeStageBattle* in_pThis, void* edx)
{
    originalResetStageHook(in_pThis, edx);

    // Just like app::GameModeStage::ResetStage, the Zelda Playing Event gets reset on stage restart to ensure that the objects
    // found in The Legend of Zelda Zone work correctly in multiplayer.
    in_pThis->pLevelInfo->SetPlayingZeldaEvent(false);
}

HOOK(TiFsmState_t&, __fastcall, StatePlayHook, ASLR(0x0093AC70), app::GameModeStageBattle* in_pThis, void* edx, TiFsmState_t& out_rState, const app::GameModeUtil::Event<app::GameModeStage>& in_rEvent)
{
    // These messages don't get handled as part of app::GameModeStageBattle::StatePlay, because The Legend of Zelda Zone DLC is not
    // playable in multiplayer by default. The following code ensures that the heart system works to its fullest in multiplayer.
    switch (in_rEvent.getSignal())
    {
    case TiFSM_SIGNAL_MESSAGE:
    {
        switch (in_rEvent.getMessage().GetType())
        {
        case app::xgame::MsgDlcZeldaTakeHeart::MessageID:
        {
            auto& message = static_cast<app::xgame::MsgDlcZeldaTakeHeart&>(in_rEvent.getMessage());

            in_pThis->SendToGroup(8, message);

            message.Handled = true;
            break;
        }
        case app::xgame::MsgDlcZeldaTakeHeartContainer::MessageID:
        {
            auto& message = static_cast<app::xgame::MsgDlcZeldaTakeHeartContainer&>(in_rEvent.getMessage());

            in_pThis->SendToGroup(8, message);
            in_pThis->SendToGroup(11, message);
            in_pThis->SendToGroup(12, message);

            message.Handled = true;
            break;
        }
        case app::xgame::MsgDlcZeldaNoticeStopEnemy::MessageID:
        {
            auto& message = static_cast<app::xgame::MsgDlcZeldaNoticeStopEnemy&>(in_rEvent.getMessage());

            in_pThis->pDocument->GetService<app::CLevelInfo>()->SetPlayingZeldaEvent(true);
            in_pThis->SendToGroup(6, message);
            in_pThis->SendToGroup(7, message);

            message.Handled = true;
            break;
        }
        case app::xgame::MsgDlcZeldaNoticeActiveEnemy::MessageID:
        {
            auto& message = static_cast<app::xgame::MsgDlcZeldaNoticeActiveEnemy&>(in_rEvent.getMessage());

            in_pThis->pDocument->GetService<app::CLevelInfo>()->SetPlayingZeldaEvent(false);
            in_pThis->SendToGroup(6, message);
            in_pThis->SendToGroup(7, message);

            message.Handled = true;
            break;
        }
        case app::xgame::MsgDlcZeldaHeartAllRecovery::MessageID:
        {
            auto& message = static_cast<app::xgame::MsgDlcZeldaHeartAllRecovery&>(in_rEvent.getMessage());

            in_pThis->SendToGroup(8, message);
            in_pThis->SendToGroup(11, message);
            in_pThis->SendToGroup(12, message);

            message.Handled = true;
            break;
        }
        default:
            return originalStatePlayHook(in_pThis, edx, out_rState, in_rEvent);
        }
        break;
    }
    default:
        return originalStatePlayHook(in_pThis, edx, out_rState, in_rEvent);
    }

    out_rState = {};
    return out_rState;
}

HOOK(TiFsmState_t&, __fastcall, StateWarpHook, ASLR(0x0093A1F0), app::GameModeStageBattle* in_pThis, void* edx, TiFsmState_t& out_rState, const app::GameModeUtil::Event<app::GameModeStage>& in_rEvent)
{
    auto& result = originalStateWarpHook(in_pThis, edx, out_rState, in_rEvent);

    // The HUD element for the pipes in the Yoshi's Island Zone DLC are never initialized as the DLC is not playable in multiplayer by default.
    // To ensure that the HUD element can be played it is initialized in the same fashion as it is in app::GameModeStage::StateWarp.
    switch (in_rEvent.getSignal())
    {
    case TiFSM_SIGNAL_UPDATE:
    {
        if (in_pThis->Unk1 != 5 || in_pThis->WarpInfo.Unk1 != 5)
            break;

        auto* pHudObject = new app::HUD::HudWipeCrayPipe();
        if (pHudObject)
            in_pThis->GetGameDocument()->AddGameObject(pHudObject);

        break;
    }
    }

    return out_rState;
}

void slw_dlc_restoration::GameModeStageBattle::InstallHooks()
{
    // General Hooks
    INSTALL_HOOK(InitFirstHook);
    if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
        INSTALL_HOOK(LoadStartingCallbackHook);

    // Yoshi's Island Zone DLC Hooks
    if (!CONFIGURATION.YoshiTweaks.DisablePipeTransition)
    {
        INSTALL_HOOK(LoadLevelHook);
        INSTALL_HOOK(StateWarpHook);
    }

    // The Legend of Zelda Zone DLC Hooks
    if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
    {
        INSTALL_HOOK(ResetStageHook);
        INSTALL_HOOK(StatePlayHook);
    }
}