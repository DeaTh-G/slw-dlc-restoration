#include "pch.h"

void slw_dlc_restoration::objectObjectReflections::InstallPatches(ModInfo_t* in_pModInfo)
{
    // Yoshi's Island Zone DLC Patches
    // Use the create function found in slw_dlc_restoration instead of their original game restoration to fix objects for multiplayer.
    if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
    {
        PIRANHA_PLANT_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::EnemyPiranhaPlant>;
        CRAY_PIPE_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjCrayPipe>;
        CRAY_PIPE_EXIT_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjCrayPipeExit>;
        EGG_BLOCK_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjEggBlock>;
        YOSHI_GOAL_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjYoshiGoal>;
    }
    
    // Use the create function found in slw_dlc_restoration instead of their original game restoration if configured to do so.
    if (CONFIGURATION.YoshiTweaks.IsConsistentShadows)
    {
        YOSHI_COIN_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjYoshiCoin>;
        YOSHI_JUMP_BOARD_BIG_CLASS.fpCreateObject = &slw_dlc_restoration::ObjYoshiJumpBoard::Create_Big;
        YOSHI_JUMP_BOARD_SMALL_CLASS.fpCreateObject = &slw_dlc_restoration::ObjYoshiJumpBoard::Create_Small;
    }

    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2A2B0), PIRANHA_PLANT_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2A620), SHY_GUY_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2BB20), CRAY_PIPE_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2BB70), CRAY_PIPE_EXIT_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2BC10), EGG_ROTATION_CHANGE_COLLISION_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2BC60), EGG_BLOCK_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C0C0), ROTATE_LIFT_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C1B0), YOSHI_COIN_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C200), YOSHI_GOAL_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C250), YOSHI_SPECIAL_FLOWER_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C2A0), YOSHI_JUMP_BOARD_BIG_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C2F0), YOSHI_JUMP_BOARD_SMALL_CLASS);

    // The Legend of Zelda DLC Patches
    // Use the create function found in slw_dlc_restoration instead of their original game restoration to fix objects for multiplayer.
    if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs)
    {
        DEKU_NUTS_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::EnemyDekunuts>;
        STAL_BABY_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::EnemyStalBaby>;
        CHANGE_TOP_VIEW_COLLISION_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjChangeTopViewCollision>;
        GOSSIP_STONE_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjGossipStone>;
        OCCLUDE_STONE_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjOccludeStone>;
        GOLON_ROCK_GENERATOR_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjGolonRockGenerator>;
        COCCO_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjCocco>;
    }

    if (CONFIGURATION.ZeldaTweaks.IsWaterParticleFixed)
        EVENT_DROWNING_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjEventDrowning>;

    if (CONFIGURATION.ZeldaTweaks.FixLoftBirdMemoryLeak)
        LOFT_BIRD_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjLoftBird>;

    if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs || CONFIGURATION.ZeldaTweaks.ScaleObjectsWithPlayer ||
        CONFIGURATION.ZeldaTweaks.FixTreasureChestRupeeCount || CONFIGURATION.ZeldaTweaks.DisableChestLetterboxing)
    {
        TREASURE_BOX_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjTreasureBox>;
    }

    if (CONFIGURATION.ZeldaTweaks.IsStalbabyFixed)
    {
        in_pModInfo->API->BindFile(in_pModInfo->CurrentMod, "+EnemyStalbaby.pac", "dlc/0012/sonic2013_dlc_0012/EnemyStalbabyFix.pac", 1);
    }

    if (CONFIGURATION.GlobalTweaks.FixMultiplayerBugs || CONFIGURATION.ZeldaTweaks.ScaleObjectsWithPlayer || CONFIGURATION.ZeldaTweaks.UseUnusedGoalAnimation)
        ZELDA_GOAL_CLASS.fpCreateObject = app::CSetObjClass::ObjectCreator<slw_dlc_restoration::ObjZeldaGoal>;

    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D28660), ZELDA_WARP_COLLISION_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D290D0), DEKU_NUTS_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D29AD0), KEESE_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2A670), STAL_BABY_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2B990), BREAK_FLOOR_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2B9E0), BREAK_ROOF_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2BA30), CHANGE_TOP_VIEW_COLLISION_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2BA80), COCCO_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2BCB0), EVENT_DROWNING_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2BD00), GOLON_ROCK_GENERATOR_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2BD50), GOSSIP_STONE_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2BDA0), LOFT_BIRD_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2BDF0), LOFT_BIRD_COLLISION_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C020), OCCLUDE_STONE_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C110), TREASURE_BOX_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C340), ZELDA_BUSH_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C390), ZELDA_GOAL_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C3E0), ZELDA_ITEM_TREE_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C430), ZELDA_POPUP_ITEM_GENERATOR_CLASS);
    WRITE_OBJECT_CLASS_CREATE(ASLR(0x00D2C480), ZELDA_RUPEE_CLASS);
}