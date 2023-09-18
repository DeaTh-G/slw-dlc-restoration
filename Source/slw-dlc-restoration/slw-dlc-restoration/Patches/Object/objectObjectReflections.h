#pragma once

namespace slw_dlc_restoration
{
	class objectObjectReflections
	{
		inline static app::CSetObjClass PIRANHA_PLANT_CLASS{ app::CSetObjClass::Create<app::EnemyPiranhaPlant, app::EnemyPiranhaPlantInfo>("enmPiranhaPlant", "OBJECT", "DLC-Yoshi", app::paramMap_EnemyPiranhaPlant) };
		inline static app::CSetObjClass SHY_GUY_CLASS{ app::CSetObjClass::Create<app::EnemyShyGuy, app::EnemyShyGuyInfo>("enmShyGuy", "OBJECT", "DLC-Yoshi", app::paramMap_EnemyShyGuy) };
		inline static app::CSetObjClass CRAY_PIPE_CLASS{ app::CSetObjClass::Create<app::ObjCrayPipe>("CrayPipe", "OBJECT", "DLC-Yoshi", nullptr, app::paramMap_ObjCrayPipe) };
		inline static app::CSetObjClass CRAY_PIPE_EXIT_CLASS{ app::CSetObjClass::Create<app::ObjCrayPipeExit>("CrayPipeExit", "OBJECT", "DLC-Yoshi", nullptr, app::paramMap_ObjCrayPipeExit) };
		inline static app::CSetObjClass EGG_ROTATION_CHANGE_COLLISION_CLASS{ app::CSetObjClass::Create<app::ObjEggRotationChangeCollision>("EggRotationChangeCollision", "OBJECT", "DLC-Yoshi", nullptr, app::paramMap_ObjEggRotationChangeCollision) };
		inline static app::CSetObjClass EGG_BLOCK_CLASS{ app::CSetObjClass::Create<app::ObjEggBlock, app::ObjEggBlockInfo>("EggBlock", "OBJECT", "DLC-Yoshi", app::paramMap_ObjEggBlock) };
		inline static app::CSetObjClass ROTATE_LIFT_CLASS{ app::CSetObjClass::Create<app::ObjRotateLift, app::ObjRotateLiftInfo>("RotateLift", "OBJECT", "DLC-Yoshi", app::paramMap_ObjRotateLift) };
		inline static app::CSetObjClass YOSHI_COIN_CLASS{ app::CSetObjClass::Create<app::ObjYoshiCoin, app::ObjYoshiCoinInfo>("YoshiCoin", "OBJECT", "DLC-Yoshi", app::paramMap_ObjYoshiCoin) };
		inline static app::CSetObjClass YOSHI_GOAL_CLASS{ app::CSetObjClass::Create<app::ObjYoshiGoal, app::ObjYoshiGoalInfo>("YoshiGoal", "OBJECT", "DLC-Yoshi", app::paramMap_ObjYoshiGoal) };
		inline static app::CSetObjClass YOSHI_SPECIAL_FLOWER_CLASS{ app::CSetObjClass::Create<app::ObjYoshiSpecialFlower, app::ObjYoshiSpecialFlowerInfo>("YoshiSpecialFlower", "OBJECT", "DLC-Yoshi", app::paramMap_ObjYoshiSpecialFlower) };
		inline static app::CSetObjClass YOSHI_JUMP_BOARD_BIG_CLASS{ app::CSetObjClass::Create("YoshiJumpBoardBig", "OBJECT", "DLC-Yoshi", &app::ObjYoshiJumpBoard::Create_Big, app::createObjInfo_ObjYoshiJumpBoardBig_ObjYoshiJumpBoardInfo, app::paramMap_ObjYoshiJumpBoardBig) };
		inline static app::CSetObjClass YOSHI_JUMP_BOARD_SMALL_CLASS{ app::CSetObjClass::Create("YoshiJumpBoardSmall", "OBJECT", "DLC-Yoshi", &app::ObjYoshiJumpBoard::Create_Small, app::createObjInfo_ObjYoshiJumpBoardSmall_ObjYoshiJumpBoardInfo, app::paramMap_ObjYoshiJumpBoardBig) };

		inline static app::CSetObjClass ZELDA_WARP_COLLISION_CLASS{ app::CSetObjClass::Create<app::ObjZeldaWarpCollision>("ZeldaWarpCollision", "OBJECT", "DLC-Zelda", nullptr, app::paramMap_ObjZeldaWarpCollision) };
		inline static app::CSetObjClass DEKU_NUTS_CLASS{ app::CSetObjClass::Create<app::EnemyDekunuts, app::EnemyDekunutsInfo>("enmDekunuts", "OBJECT", "DLC-Zelda", app::paramMap_EnemyDekunuts) };
		inline static app::CSetObjClass KEESE_CLASS{ app::CSetObjClass::Create<app::EnemyKeese, app::EnemyKeeseInfo>("enmKeese", "OBJECT", "DLC-Zelda", app::paramMap_EnemyKeese) };
		inline static app::CSetObjClass STAL_BABY_CLASS{ app::CSetObjClass::Create<app::EnemyStalBaby, app::EnemyStalBabyInfo>("enmStalBaby", "OBJECT", "DLC-Zelda", app::paramMap_EnemyStalBaby) };
		inline static app::CSetObjClass BREAK_FLOOR_CLASS{ app::CSetObjClass::Create<app::ObjBreakFloor, app::ObjBreakFloorInfo>("BreakFloor", "OBJECT", "DLC-Zelda", app::paramMap_ObjBreakFloor) };
		inline static app::CSetObjClass BREAK_ROOF_CLASS{ app::CSetObjClass::Create<app::ObjBreakRoof, app::ObjBreakRoofInfo>("BreakRoof", "OBJECT", "DLC-Zelda", app::paramMap_ObjBreakRoof) };
		inline static app::CSetObjClass CHANGE_TOP_VIEW_COLLISION_CLASS{ app::CSetObjClass::Create<app::ObjChangeTopViewCollision>("ChangeTopViewCollision", "OBJECT", "DLC-Zelda", nullptr, app::paramMap_ObjChangeTopViewCollision) };
		inline static app::CSetObjClass COCCO_CLASS{ app::CSetObjClass::Create<app::ObjCocco, app::ObjCoccoInfo>("Cocco", "OBJECT", "DLC-Zelda", app::paramMap_ObjCocco) };
		inline static app::CSetObjClass EVENT_DROWNING_CLASS{ app::CSetObjClass::Create<app::ObjEventDrowning>("EventDrowningCollision", "OBJECT", "DLC-Zelda", nullptr, app::paramMap_ObjEventDrowning) };
		inline static app::CSetObjClass GOLON_ROCK_GENERATOR_CLASS{ app::CSetObjClass::Create<app::ObjGolonRockGenerator, app::ObjGolonRockGeneratorInfo>("GolonRockGenerator", "OBJECT", "DLC-Zelda", app::paramMap_ObjGolonRockGenerator) };
		inline static app::CSetObjClass GOSSIP_STONE_CLASS{ app::CSetObjClass::Create<app::ObjGossipStone, app::ObjGossipStoneInfo>("GossipStone", "OBJECT", "DLC-Zelda", app::paramMap_ObjGossipStone) };
		inline static app::CSetObjClass LOFT_BIRD_CLASS{ app::CSetObjClass::Create<app::ObjLoftBird, app::ObjLoftBirdInfo>("LoftBird", "OBJECT", "DLC-Zelda", app::paramMap_ObjLoftBird) };
		inline static app::CSetObjClass LOFT_BIRD_COLLISION_CLASS{ app::CSetObjClass::Create<app::ObjLoftBirdCollision>("LoftBirdCollision", "OBJECT", "DLC-Zelda", nullptr, app::paramMap_ObjLoftBirdCollision) };
		inline static app::CSetObjClass OCCLUDE_STONE_CLASS{ app::CSetObjClass::Create<app::ObjOccludeStone, app::ObjOccludeStoneInfo>("OccludeStone", "OBJECT", "DLC-Zelda", app::paramMap_ObjOccludeStone) };
		inline static app::CSetObjClass TREASURE_BOX_CLASS{ app::CSetObjClass::Create<app::ObjTreasureBox, app::ObjTreasureBoxInfo>("TreasureBox", "OBJECT", "DLC-Zelda", app::paramMap_ObjTreasureBox) };
		inline static app::CSetObjClass ZELDA_BUSH_CLASS{ app::CSetObjClass::Create<app::ObjZeldaBush, app::ObjZeldaBushInfo>("ZeldaBush", "OBJECT", "DLC-Zelda", app::paramMap_ObjZeldaBush) };
		inline static app::CSetObjClass ZELDA_GOAL_CLASS{ app::CSetObjClass::Create<app::ObjZeldaGoal, app::ObjZeldaGoalInfo>("ZeldaGoal", "OBJECT", "DLC-Zelda", app::paramMap_ObjZeldaGoal) };
		inline static app::CSetObjClass ZELDA_ITEM_TREE_CLASS{ app::CSetObjClass::Create<app::ObjZeldaItemTree, app::ObjZeldaItemTreeInfo>("ZeldaItemTree", "OBJECT", "DLC-Zelda", app::paramMap_ObjZeldaItemTree) };
		inline static app::CSetObjClass ZELDA_POPUP_ITEM_GENERATOR_CLASS{ app::CSetObjClass::Create<app::ObjZeldaPopupItemGenerator, app::ObjZeldaPopupItemInfo>("ZeldaPopupItemGenerator", "OBJECT", "DLC-Zelda", app::paramMap_ObjZeldaPopupItemGenerator) };
		inline static app::CSetObjClass ZELDA_RUPEE_CLASS{ app::CSetObjClass::Create<app::ObjZeldaRupee, app::ObjZeldaRupeeInfo>("ZeldaRupee", "OBJECT", "DLC-Zelda", app::paramMap_ObjZeldaRupee) };

	public:
		static void InstallPatches();
	};
}