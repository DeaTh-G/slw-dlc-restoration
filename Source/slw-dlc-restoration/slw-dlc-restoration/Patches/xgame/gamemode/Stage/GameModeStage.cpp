#include "pch.h"
#include "GameModeStage.h"

typedef app::TTinyFsm<app::GameModeStage, app::GameModeUtil::Event<app::GameModeStage>, false>::TiFsmState_t TiFsmState_t;

HOOK(void, __fastcall, ConstructorHook, ASLR(0x00919D70), app::GameModeStage* in_pThis, void* edx, const app::SGameModeStageCinfo& in_rCreateInfo)
{
    originalConstructorHook(in_pThis, edx, in_rCreateInfo);

    if (CONFIGURATION.ZeldaTweaks.LinkSonicPlayType == slw_dlc_restoration::SConfiguration::SZeldaTweaks::ePlayType_Always)
        in_pThis->CreateInfo.IsSuperSonicUnlocked = false;
}

HOOK(void, __fastcall, LoadLevelHook, ASLR(0x00917730), app::GameModeStage* in_pThis, void* edx)
{
    originalLoadLevelHook(in_pThis, edx);

    // The UI packfile for The Legend of Zelda Zone DLC is loaded upon creation of GameModeStage to ensure that
    // we have access to the *.swif files inside that packfile later on.
    app::fnd::FileLoaderParam param{};
    param.pAllocator = app::game::GlobalAllocator::GetAllocator(1);
    param.SetMultiLanguageAttr(true, 0);
    in_pThis->LoadFile("ui/ui_zdlc03_gamemodestage.pac", param);
    param.SetMultiLanguageAttr(false, 0);
}

HOOK(void, __fastcall, ResetStageHook, ASLR(0x00916BC0), app::GameModeStage* in_pThis, void* edx)
{
    originalResetStageHook(in_pThis, edx);

    // The original game on Wii U calls this function to reset to the Zelda Playing Event set by objects in The Legend of Zelda Zone DLC.
    // As the DLC does not exist on the PC version this function is never called either originally.
    in_pThis->pLevelInfo->SetPlayingZeldaEvent(false);
}

HOOK(TiFsmState_t&, __fastcall, StatePlayHook, ASLR(0x0091A680), app::GameModeStage* in_pThis, void* edx, TiFsmState_t& out_rState, const app::GameModeUtil::Event<app::GameModeStage>& in_rEvent)
{
    // These messages don't get handled as part of app::GameModeStage::StatePlay, because The Legend of Zelda Zone DLC is no longer part of the game.
    // To ensure that the DLC is working correctly these messages are added back to be handled by the game just like they would be on the Wii U version.
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

            in_pThis->MaxNumHearts++;

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

HOOK(TiFsmState_t&, __fastcall, StateWarpHook, ASLR(0x00917C30), app::GameModeStage* in_pThis, void* edx, TiFsmState_t& out_rState, const app::GameModeUtil::Event<app::GameModeStage>& in_rEvent)
{
    auto& result = originalStateWarpHook(in_pThis, edx, out_rState, in_rEvent);

    // The HUD element for the pipes in the Yoshi's Island Zone DLC are never initialized as neither the HUD or the DLC exist on PC.
    // The following code ensures that the HUD does get created the same way as it would be on the Wii U version of the game.
    switch (in_rEvent.getSignal())
    {
    case TiFSM_SIGNAL_UPDATE:
    {
        if (in_pThis->Unk8 != 3 || in_pThis->Unk15 > 0.0f || in_pThis->WarpInfo.Unk1 != 5)
            break;

        auto* pHudObject = new app::HUD::HudWipeCrayPipe();
        if (pHudObject)
            in_pThis->GetGameDocument()->AddGameObject(pHudObject);

        break;
    }
    }

    return out_rState;
}

HOOK(void, __fastcall, DisposeMsgPLSendGameInfoHook, ASLR(0x00915120), app::GameModeStage* in_pThis, void* edx, app::xgame::MsgPLSendGameInfo& in_rMessage)
{
    originalDisposeMsgPLSendGameInfoHook(in_pThis, edx, in_rMessage);

    // On the PC version of the game, GameModeStage no longer stores the values of the Heart System as The Legend of Zelda Zone DLC does not exist.
    // The following code re-adds this functionality to GameModeStage to ensure the functionality of the Heart System like on the Wii U version
    if (in_pThis->IsPlayer(in_rMessage.Sender))
    {
        auto* pPlayerInfo = in_pThis->GetPlayerInfo(in_rMessage.PlayerNo);
        pPlayerInfo->NumHearts = in_rMessage.NumHearts;
        pPlayerInfo->MaxNumHearts = in_rMessage.MaxNumHearts;
    }
}

void slw_dlc_restoration::GameModeStage::InstallPatches()
{
    // Yoshi's Island Zone DLC Patches
    WRITE_CALL(ASLR(0x00916EED), csl::ut::union_cast<uint32_t>(&app::StageInfo::SStageData::IsYoshiIslandStage));
    
    // The Legend of Zelda Zone DLC Patches
    WRITE_CALL(ASLR(0x00916F00), csl::ut::union_cast<uint32_t>(&app::StageInfo::SStageData::IsZeldaStage));
}

void slw_dlc_restoration::GameModeStage::InstallHooks()
{
    // Yoshi's Island Zone DLC Hooks
    if (!CONFIGURATION.YoshiTweaks.DisablePipeTransition)
        INSTALL_HOOK(StateWarpHook);

    // The Legend of Zelda Zone DLC Hooks
    INSTALL_HOOK(ConstructorHook);
    INSTALL_HOOK(ResetStageHook);
    INSTALL_HOOK(StatePlayHook);
    INSTALL_HOOK(DisposeMsgPLSendGameInfoHook);
    if (CONFIGURATION.ZeldaTweaks.IsAlwaysHeartLife)
        INSTALL_HOOK(LoadLevelHook);
}