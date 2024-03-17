#include "pch.h"
#include "ModLoader.h"

slw_dlc_restoration::SConfiguration CONFIGURATION = {};

void InitializeConfiguration(ModInfo_t* in_pModInfo, const char* in_pConfigFileName)
{
    std::string dir = in_pModInfo->CurrentMod->Path;
    INIReader* pReader = new INIReader(in_pConfigFileName);

    if (pReader->ParseError() != 0)
        delete pReader, pReader = new INIReader(dir + in_pConfigFileName);

    if (pReader->ParseError() != 0)
        MessageBox(NULL, L"Failed to parse slw-dlc-restoration.ini", NULL, MB_ICONERROR);

    CONFIGURATION.IsNightsDlcEnabled = pReader->GetBoolean("DLCAvailability", "isNightsDlcEnabled", true);
    CONFIGURATION.IsYoshiDlcEnabled = pReader->GetBoolean("DLCAvailability", "isYoshiDlcEnabled", true);
    CONFIGURATION.IsZeldaDlcEnabled = pReader->GetBoolean("DLCAvailability", "isZeldaDlcEnabled", true);

    if (CONFIGURATION.IsYoshiDlcEnabled)
    {
        CONFIGURATION.YoshiTweaks.IsConsistentShadows = pReader->GetBoolean("YoshiTweaks", "isConsistentShadows", false);
        CONFIGURATION.YoshiTweaks.DisablePipeTransition = pReader->GetBoolean("YoshiTweaks", "disablePipeTransition", false);
    }

    if (CONFIGURATION.IsZeldaDlcEnabled)
    {
        CONFIGURATION.ZeldaTweaks.LinkSonicPlayType = (slw_dlc_restoration::SConfiguration::SZeldaTweaks::EPlayType)pReader->GetInteger("ZeldaTweaks", "linkSonicPlayType", (uint)slw_dlc_restoration::SConfiguration::SZeldaTweaks::EPlayType::ePlayType_Default);
        CONFIGURATION.ZeldaTweaks.IsLinkSonicFixed = pReader->GetBoolean("ZeldaTweaks", "isLinkSonicFixed", false);
        CONFIGURATION.ZeldaTweaks.IsWaterParticleFixed = pReader->GetBoolean("ZeldaTweaks", "isWaterParticleFixed", false);
        CONFIGURATION.ZeldaTweaks.IsVirtualLinkSonic = pReader->GetBoolean("ZeldaTweaks", "isVirtualLinkSonic", false);
        CONFIGURATION.ZeldaTweaks.IsAlwaysHeartLife = pReader->GetBoolean("ZeldaTweaks", "isAlwaysHeartLife", false);
        CONFIGURATION.ZeldaTweaks.DisableChestLetterboxing = pReader->GetBoolean("ZeldaTweaks", "disableChestLetterboxing", false);
        CONFIGURATION.ZeldaTweaks.IsStalbabyFixed = pReader->GetBoolean("ZeldaTweaks", "isStalbabyFixed", false);
        CONFIGURATION.ZeldaTweaks.UseUnusedGoalAnimation = pReader->GetBoolean("ZeldaTweaks", "useUnusedGoalAnimation", false);
    }

    CONFIGURATION.Is4StarBombWispDlcEnabled = pReader->GetBoolean("DLCAvailability", "is4StarBombWispDlcEnabled", false);
    CONFIGURATION.Is25LivesDlcEnabled = pReader->GetBoolean("DLCAvailability", "is25LivesDlcEnabled", false);
    CONFIGURATION.IsBombWispDlcEnabled = pReader->GetBoolean("DLCAvailability", "isBombWispDlcEnabled", false);
    CONFIGURATION.IsGoldOmochaoDlcEnabled = pReader->GetBoolean("DLCAvailability", "isGoldOmochaoDlcEnabled", false);
    CONFIGURATION.IsSonicOmochaoDlcEnabled = pReader->GetBoolean("DLCAvailability", "isSonicOmochaoDlcEnabled", false);
}

void InitializeMod(ModInfo_t* in_pModInfo)
{

    // General Patches/Hooks
    slw_dlc_restoration::objectObjectReflections::InstallPatches(in_pModInfo);
    slw_dlc_restoration::GameModeStage::InstallPatches();
    slw_dlc_restoration::CGameSequence::InstallPatches();
    slw_dlc_restoration::PrizeData::InstallPatches();
    slw_dlc_restoration::AocManager::InstallHooks();
    slw_dlc_restoration::xgame::DlcManager::InstallHooks();
    slw_dlc_restoration::GameModeStage::InstallHooks();
    slw_dlc_restoration::GameModeStageBattle::InstallHooks();
    slw_dlc_restoration::CGameSequence::InstallHooks();
    slw_dlc_restoration::StageInfo::CStageInfo::InstallHooks();
    slw_dlc_restoration::HUD::CHudGameMainDisplay::InstallHooks();
    slw_dlc_restoration::xgame::CStageSoundDirector::InstallHooks();
    slw_dlc_restoration::Player::InstallHooks();
    slw_dlc_restoration::Player::CStateBase::InstallHooks();

    // Yoshi's Island Zone DLC Patches/Hooks
    slw_dlc_restoration::EggManager::InstallPatches();
    slw_dlc_restoration::dropped_ring::InstallHooks();
    slw_dlc_restoration::Player::CPlayer::InstallHooks();

    // The Legend of Zelda Zone DLC Patches/Hooks
    slw_dlc_restoration::Player::VirtualSonicInfo::InstallPatches(in_pModInfo);
    slw_dlc_restoration::WorldAreaMapInfo::InstallHooks();
    slw_dlc_restoration::MinigameCharacterInfo::InstallHooks();
    slw_dlc_restoration::GameOverInfo::InstallHooks();
    slw_dlc_restoration::Player::SonicZeldaInfo::InstallHooks();
    slw_dlc_restoration::Player::VirtualSonicInfo::InstallHooks();
    slw_dlc_restoration::Player::StateUtil::InstallHooks();
    slw_dlc_restoration::Player::CStateSonicBase::InstallHooks();
    slw_dlc_restoration::Player::CVisualHuman::InstallHooks();
    slw_dlc_restoration::Player::CSonic::InstallHooks();
    slw_dlc_restoration::CObjCamera::InstallHooks();
}

extern "C"
{
    void _declspec(dllexport) __cdecl Init(ModInfo_t* in_pModInfo)
    {
        InitializeConfiguration(in_pModInfo, "slw-dlc-restoration.ini");

        InitializeMod(in_pModInfo);
    }
}