#pragma once

namespace app
{
    namespace fnd
    {
        typedef enum
        {
            PROC_MSG_WARNING_END								= 0x1003,
            PROC_MSG_GET_POSITION								= 0x2000,
            PROC_MSG_SET_POSITION								= 0x2001,
            PROC_MSG_GET_CENTER_POSITION						= 0x2002,
            PROC_MSG_GET_ROTATION								= 0x2003,
            PROC_MSG_GET_VELOCITY								= 0x2004,
            PROC_MSG_GET_FRONT_DIRECTION						= 0x2005,
            PROC_MSG_GET_CAMERA_TARGET_POSITION					= 0x2006,
            PROC_MSG_UPDATE_SET_EDITOR							= 0x2007,
            PROC_MSG_IS_2D_MODE									= 0x2009,
            PROC_MSG_GET_UP_DIRECTION							= 0x200A,
            PROC_MSG_GET_RIGHT_DIRECTION						= 0x200B,
            PROC_MSG_UPDATE_ON_SUSPEND_MODE						= 0x200C,
            PROC_MSG_RESTART									= 0x3006,
            PROC_MSG_LEVEL_STARTED								= 0x3007,
            PROC_MSG_LEVEL_ENDED								= 0x3008,
            PROC_MSG_GOAL_FOR_BATTLE                            = 0x300B,
            PROC_MSG_EXTEND_PLAYER                              = 0x300F,
            PROC_MSG_SWITCH_DISPLAY								= 0x301C,
            PROC_MSG_ABORT_GAME_SPEED_RC						= 0x3020,
            PROC_MSG_SHOW_HINT_ICON								= 0x3022,
            PROC_MSG_SHOW_HINT_MESSAGE							= 0x3023,
            PROC_MSG_ADD_LIMIT_TIME								= 0x3024,
            PROC_MSG_START_EVENT_END							= 0x3026,
            PROC_MSG_SELECT_VALID_OPT_LAYER						= 0x3027,
            PROC_MSG_DISABLE_ITEM_BAG                           = 0x302F,
            PROC_MSG_KILL										= 0x4000,
            PROC_MSG_DAMAGE										= 0x4001,
            PROC_MSG_KICK										= 0x4002,
            PROC_MSG_DEAD										= 0x4004,
            PROC_MSG_DEACTIVATE									= 0x4005,
            PROC_MSG_PL_GET_ROTATION							= 0x5000,
            PROC_MSG_PL_GET_GRAVITY								= 0x5001,
            PROC_MSG_PL_GET_HOMING_TARGET_INFO					= 0x5004,
            PROC_MSG_PL_GET_ANGLE								= 0x5005,
            PROC_MSG_GET_GROUND_INFO							= 0x5007,
            PROC_MSG_GET_EXTERNAL_MOVE_POSITION					= 0x500D,
            PROC_MSG_PL_GET_PLAYER								= 0x5010,
            PROC_MSG_PL_SEND_GAME_INFO                          = 0x5013,
            PROC_MSG_PL_GET_RING_NUM							= 0x5014,
            PROC_MSG_PL_TRANSFORM_PHANTOM						= 0x5016,
            PROC_MSG_PL_START_PHANTOM_POSE						= 0x5017,
            PROC_MSG_PL_GET_PHANTOM_TYPE						= 0x5019,
            PROC_MSG_PL_GET_PIXIE_TYPE							= 0x501A,
            PROC_MSG_PL_GET_PATH_INFO							= 0x5026,
            PROC_MSG_PL_GET_INPUT_BUTTON						= 0x5027,
            PROC_MSG_PL_GET_ENVIRONMENT							= 0x5028,
            PROC_MSG_PL_FLIP_IN_WATER							= 0x5029,
            PROC_MSG_PL_SET_RESULT_POSE							= 0x502B,
            PROC_MSG_PL_START_SLEEP_MODE						= 0x502D,
            PROC_MSG_PL_END_SLEEP_MODE							= 0x502E,
            PROC_MSG_PL_START_DIVING							= 0x502F,
            PROC_MSG_PL_END_DIVING								= 0x5030,
            PROC_MSG_PL_START_OUT_OF_CONTROL					= 0x5050,
            PROC_MSG_PL_END_OUT_OF_CONTROL						= 0x5051,
            PROC_MSG_PL_IS_DEAD									= 0x5054,
            PROC_MSG_REQUEST_CHANGE_PHANTOM						= 0x5055,
            PROC_MSG_PL_GET_USER_NO								= 0x5058,
            PROC_MSG_PL_IS_DISABLE_CHANGE_PHANTOM_AREA			= 0x5059,
            PROC_MSG_PL_GET_PHANTOM_REST_TIME					= 0x505A,
            PROC_MSG_PL_GET_WATER_LIMIT_TIME					= 0x505B,
            PROC_MSG_PL_GET_HISTORY_POSITION					= 0x5062,
            PROC_MSG_CYLINDER_SPEED_DOWN						= 0x5064,
            PROC_MSG_PL_DISABLE_INPUT							= 0x5066,
            PROC_MSG_PL_START_ALONG_PATH_MODE					= 0x5067,
            PROC_MSG_PL_END_ALONG_PATH_MODE						= 0x5068,
            PROC_MSG_PL_GET_ADDITIONAL_JUMP_VELOCITY			= 0x506A,
            PROC_MSG_PL_DISABLE_VISUAL							= 0x506B,
            PROC_MSG_PL_RIDE_ON_GROUND							= 0x506C,
            PROC_MSG_PL_LEAVE_GROUND							= 0x506D,
            PROC_MSG_PL_ENTER_DIVING_VOLUME						= 0x506E,
            PROC_MSG_PL_LEAVE_DIVING_VOLUME						= 0x506F,
            PROC_MSG_PL_BEGIN_AUTORUN							= 0x5070,
            PROC_MSG_PL_FINISH_AUTORUN							= 0x5071,
            PROC_MSG_HOMING_TARGET_CANCELED						= 0x5073,
            PROC_MSG_NOTIFY_BOSS_START							= 0x5074,
            PROC_MSG_GET_HOMING_TARGET_SUCCESSIVE_INFO			= 0x5075,
            PROC_MSG_FLIP_DISABLE_DAMAGE						= 0x5077,
            PROC_MSG_PL_HOLD_ON									= 0x5079,
            PROC_MSG_PL_RELEASE_HOLD							= 0x507A,
            PROC_MSG_PLAYER_REACH_GOAL							= 0x507B,
            PROC_MSG_PL_GET_HIGH_JUMP_INFO						= 0x507C,
            PROC_MSG_PL_ENABLE_SEARCH_ALL_DIRECTIONS			= 0x507D,
            PROC_MSG_PL_JUMP_TO_TARGET_POSITION					= 0x507E,
            PROC_MSG_PL_START_GROUND_IMPULSE					= 0x5081,
            PROC_MSG_ASTEROID_DEBRIS_INFO						= 0x5082,
            PROC_MSG_RESTRICT_PHANTOM							= 0x5083,
            PROC_MSG_RESTRICT_PHANTOM_CANCEL					= 0x5084,
            PROC_MSG_PL_STAGGER									= 0x5085,
            PROC_MSG_PL_STEALTH									= 0x5087,
            PROC_MSG_START_RESONANCE							= 0x5088,
            PROC_MSG_END_RESONANCE								= 0x5089,
            PROC_MSG_PL_RHYTHM_PHANTOM_TOUCH_NODE				= 0x508A,
            PROC_MSG_PL_SET_RESULT_POSE_BATTLE					= 0x508B,
            PROC_MSG_PL_SET_NUM_RING							= 0x508C,
            PROC_MSG_PL_DISABLE_NDC								= 0x508E,
            PROC_MSG_PL_QUAKE									= 0x508F,
            PROC_MSG_PL_PATH_SPEED_CONTROL						= 0x5090,
            PROC_MSG_HUD_END_PHANTOM_INFO						= 0x5092,
            PROC_MSG_ENABLE_CHANGE_SUPER_SONIC					= 0x5093,
            PROC_MSG_NOTIFY_ENABLE_CHANGE_SUPER_SONIC			= 0x5094,
            PROC_MSG_REQUEST_CHANGE_SUPER_SONIC					= 0x5095,
            PROC_MSG_REQUEST_RETURN_FROM_SUPER_SONIC			= 0x5096,
            PROC_MSG_TRANSFORM_SUPER_SONIC						= 0x5097,
            PROC_MSG_PL_CHANGE_HOMING_RANGE						= 0x5098,
            PROC_MSG_PL_END_ROCKET_PHANTOM						= 0x509A,
            PROC_MSG_PL_CHANGE_AREA_TOP_VIEW					= 0x509B,
            PROC_MSG_PHANTOM_GAUGE								= 0x509C,
            PROC_MSG_START_PHANTOM_BLUR							= 0x509D,
            PROC_MSG_STOP_PHANTOM_BLUR							= 0x509E,
            PROC_MSG_PL_KICK_TARGETTING							= 0x509F,
            PROC_MSG_RESTRICT_WARP								= 0x50A1,
            PROC_MSG_RESTRICT_WARP_CANCEL						= 0x50A2,
            PROC_MSG_CANCEL_BOMB_PHANTOM						= 0x50A3,
            PROC_MSG_PL_VISIBLE_ITEM_EFFECT						= 0x50A4,
            PROC_MSG_PL_CALLBACK_ACTION							= 0x50A7,
            PROC_MSG_PL_CALLBACK_HAND							= 0x50A9,
            PROC_MSG_CHANGE_DAMAGE_STATE						= 0x50AA,
            PROC_MSG_CHANGE_DEAD_STATE							= 0x50AB,
            PROC_MSG_GRAVITY_SWITCHED							= 0x50AD,
            PROC_MSG_GO_HIGH_JUMP_STATE							= 0x50AE,
            PROC_MSG_HIT_DAMAGE									= 0x50B6,
            PROC_MSG_START_KICK									= 0x50B7,
            PROC_MSG_SETUP_GROUND_IMPULSE						= 0x50B9,
            PROC_MSG_SETUP_DAMAGE_AND_BONUS						= 0x50BA,
            PROC_MSG_PULL_BACK_NDC								= 0x50BB,
            PROC_MSG_AFTER_WARP_OTHER_SIDE						= 0x50BC,
            PROC_MSG_SPRING_IMPULSE								= 0x6000,
            PROC_MSG_ON_GROUND_IMPULSE							= 0x6001,
            PROC_MSG_BUMPER_IMPULSE								= 0x6005,
            PROC_MSG_FLIPPER_IMPULSE							= 0x6006,
            PROC_MSG_PLUNGER_IMPULSE							= 0x6007,
            PROC_MSG_TAKE_OBJECT								= 0x6008,
            PROC_MSG_CHANGE_2D_MODE								= 0x600E,
            PROC_MSG_CHANGE_3D_MODE								= 0x6010,
            PROC_MSG_NOTIFY_OBJECT_EVENT						= 0x6013,
            PROC_MSG_PLAYER_PASS_POINT_MARKER					= 0x6017,
            PROC_MSG_TAKE_PIXIE									= 0x6019,
            PROC_MSG_CATCH_PLAYER								= 0x601C,
            PROC_MSG_CATCH_END_PLAYER							= 0x601D,
            PROC_MSG_EXIT_CATCH_PLAYER							= 0x601E,
            PROC_MSG_LASER_PHANTOM_CANCEL						= 0x6022,
            PROC_MSG_PRISM_IMPULSE								= 0x6026,
            PROC_MSG_PHANTOM_CANCEL								= 0x6027,
            PROC_MSG_STOP_FOR_GOAL								= 0x602E,
            PROC_MSG_BEGIN_CYLINDER_MODE						= 0x6046,
            PROC_MSG_END_CYLINDER_MODE							= 0x6047,
            PROC_MSG_BEGIN_GPATH_MODE							= 0x6049,
            PROC_MSG_SHEEP_BUMPER_IMPULSE						= 0x604D,
            PROC_MSG_SHEEP_ROLLING_IMPULSE						= 0x604E,
            PROC_MSG_ITEM_TREE_WOBBLE_POINT						= 0x604F,
            PROC_MSG_GET_CLIMB_OBJECT_INFO						= 0x6050,
            PROC_MSG_SHEEP_TARGET_IMPULSE						= 0x6051,
            PROC_MSG_BREAK_ROLLING_STONE						= 0x6052,
            PROC_MSG_ACTIVATE_OBJECT_DEBUG_WINDOW				= 0x6053,
            PROC_MSG_OPERATE_FLYING_NIMBUS						= 0x6054,
            PROC_MSG_GET_NEXT_BOUNCE_BELL_PARAM					= 0x6057,
            PROC_MSG_SHEEP_PURGE_FUR							= 0x6058,
            PROC_MSG_SHEEP_FRIENDLY_ATTACK						= 0x6059,
            PROC_MSG_TORNADO_CONTROL_START						= 0x605A,
            PROC_MSG_TORNADO_CONTROL_ACTION						= 0x605B,
            PROC_MSG_TORNADO_CONTROL_END						= 0x605C,
            PROC_MSG_WARP										= 0x605F,
            PROC_MSG_PLAYER_TRANSPORT_BY_WARP					= 0x6060,
            PROC_MSG_WARP_NEW_AREA								= 0x6061,
            PROC_MSG_WARP_OTHER_SIDE							= 0x6065,
            PROC_MSG_DROPPING_SAND								= 0x6067,
            PROC_MSG_QUICKSAND_IMPULSE							= 0x6068,
            PROC_MSG_PASS_SKY_DASH_RING							= 0x606A,
            PROC_MSG_SAND_HOLE_DRAW_IN							= 0x606B,
            PROC_MSG_LAUNCH_PATH_SPRING							= 0x606C,
            PROC_MSG_ENTER_LOOP									= 0x606D,
            PROC_MSG_EXIT_LOOP									= 0x606E,
            PROC_MSG_SET_LOOP_FILTER							= 0x6070,
            PROC_MSG_ON_RUNNING_SAND							= 0x6071,
            PROC_MSG_GET_BUBBLE									= 0x6072,
            PROC_MSG_START_ITEM_POPUP							= 0x6073,
            PROC_MSG_SET_PARENT									= 0x6074,
            PROC_MSG_GET_CATCH_PLAYER_ACTOR_ID					= 0x6075,
            PROC_MSG_SEA_MINE_CHAIN								= 0x6076,
            PROC_MSG_SEA_MINE_BROKEN							= 0x6077,
            PROC_MSG_WATER_WORKS								= 0x607A,
            PROC_MSG_MIXER_CONTROL_START						= 0x607B,
            PROC_MSG_MIXER_CONTROL_END							= 0x607C,
            PROC_MSG_GET_ENEMY_CUBE_INFO						= 0x607D,
            PROC_MSG_GET_DEATH_TORNADO_INFO						= 0x607E,
            PROC_MSG_KICKOUT_HOLE_IMPULSE						= 0x607F,
            PROC_MSG_CARRY_OBJECT								= 0x6080,
            PROC_MSG_CARRY_END_OBJECT							= 0x6081,
            PROC_MSG_CARRY_BREAK_OBJECT							= 0x6082,
            PROC_MSG_PIN_BALL_STARTER							= 0x6083,
            PROC_MSG_SAND_DRIVE_STARTER							= 0x6084,
            PROC_MSG_PRESSED_BY_SNOWBALL						= 0x6085,
            PROC_MSG_BUMBPER_PENGUIN_IMPULSE					= 0x6087,
            PROC_MSG_GET_NUMBER_NUMBER_RING						= 0x6088,
            PROC_MSG_STICKER_ENEMY_BOMB_BLAST					= 0x6089,
            PROC_MSG_SNOW_MAN_KILLED							= 0x608B,
            PROC_MSG_NOTIFY_ENEMY_MANTA_RAY_INFO				= 0x608C,
            PROC_MSG_KEKKO_BLOW									= 0x608D,
            PROC_MSG_PUFFBALL_BOUNCE							= 0x608E,
            PROC_MSG_NOTIFY_OBJ_DRAGON_FLOOR_INFO				= 0x608F,
            PROC_MSG_NOTIFY_BOSS_GM_EVENT						= 0x6090,
            PROC_MSG_BUTTERFLY_COLLIDES							= 0x6091,
            PROC_MSG_FIRE_FLOWER_GUN_WITH_PATH					= 0x6092,
            PROC_MSG_GET_READY_FOR_FIRING_BUTTERFLIES			= 0x6093,
            PROC_MSG_BUTTERFLY_READIED_FOR_FIRING				= 0x6094,
            PROC_MSG_LOSS_RING									= 0x6095,
            PROC_MSG_BOSS_MOON_MASTER_JUMP						= 0x6096,
            PROC_MSG_BOSS_MOON_MASTER_STOP						= 0x6097,
            PROC_MSG_NOTIFY_BOSS_MM_EVENT						= 0x6098,
            PROC_MSG_GET_CHAIN_HOMING_TARGET					= 0x6099,
            PROC_MSG_GET_SUCKED_INTO_PIPE						= 0x609A,
            PROC_MSG_GET_ANIMAL									= 0x609B,
            PROC_MSG_LASER_CRYSTAL_IMPULSE_BLUE					= 0x609C,
            PROC_MSG_LASER_CRYSTAL_IMPULSE_RED					= 0x609D,
            PROC_MSG_WORMHOLE_START_SUCKED_INTO					= 0x609E,
            PROC_MSG_WORMHOLE_WARP_START						= 0x60A0,
            PROC_MSG_WORMHOLE_WARP_END							= 0x60A1,
            PROC_MSG_WORMHOLE_TRANSFER_INFO						= 0x60A2,
            PROC_MSG_NOTIFY_BOSS_FM_EVENT						= 0x60A3,
            PROC_MSG_END_GPATH_MODE								= 0x60A4,
            PROC_MSG_KAMAKILLER_KILLED							= 0x60A5,
            PROC_MSG_MULTIPLE_ATTACK_END						= 0x60A6,
            PROC_MSG_HIT_UPDRAFT_COLLISION						= 0x60A7,
            PROC_MSG_BOUNCE_BIG_PENGUIN							= 0x60A8,
            PROC_MSG_SNOW_BALL_LAUNCHER							= 0x60A9,
            PROC_MSG_BELT_CONVEYOR_CONTROL_START				= 0x60AA,
            PROC_MSG_BELT_CONVEYOR_CONTROL_END					= 0x60AB,
            PROC_MSG_NOTIFY_RESET_FORCE_MOVE_MODE_CAMERA		= 0x60AC,
            PROC_MSG_NOTIFY_FINISH_SET_EDITOR					= 0x60AD,
            PROC_MSG_GET_ON_SNOW_BALL_RAIL						= 0x60AE,
            PROC_MSG_CELESTIAL_MOON_EXIT						= 0x60AF,
            PROC_MSG_DOWNED_MOON_MASTER							= 0x60B0,
            PROC_MSG_SEND_CASINO_SCORE							= 0x60B1,
            PROC_MSG_SEND_CASINO_SCORE_DISPLAY					= 0x60B2,
            PROC_MSG_SEND_CASINO_SCORE_CLEAR					= 0x60B3,
            PROC_MSG_NOTIFY_BOSS_DM_EVENT						= 0x60B4,
            PROC_MSG_RING_GET_SUCKED_INTO						= 0x60B5,
            PROC_MSG_HOLD_ON_TO_IRON_BAR						= 0x60B7,
            PROC_MSG_IRON_BAR_COLLAPSE							= 0x60B8,
            PROC_MSG_RUSHH_BOSS_OWL_MASTER_TERMINATION			= 0x60BB,
            PROC_MSG_CALL_OUT_APPROACH_LAVA						= 0x60BC,
            PROC_MSG_BREAK_LIFT									= 0x60BD,
            PROC_MSG_BREAK_LIFT_PLATFORM						= 0x60BE,
            PROC_MSG_STEAM_SPRING_FALL							= 0x60BF,
            PROC_MSG_STEAM_SPRING_RIDE							= 0x60C0,
            PROC_MSG_SNOW_BALL_DAHS								= 0x60C1,
            PROC_MSG_RUSH_BOSS_DM_BOUND_ATTACK					= 0x60C2,
            PROC_MSG_NOTIFY_SILHOUETTE_AREA						= 0x60C3,
            PROC_MSG_NOTIFY_SILHOUETTE_STATE					= 0x60C4,
            PROC_MSG_NOTIFY_MID_BOSS_PM_EVENT					= 0x60C5,
            PROC_MSG_NOTIFY_BATTLE_STATE_OF_BOSS_OWL_MASTER		= 0x60C6,
            PROC_MSG_ENTER_BALLOON_CANNON						= 0x60C7,
            PROC_MSG_RELAUNCH_ROCKET_FROM_BALLOON				= 0X60c8,
            PROC_MSG_CREATE_METEORITE							= 0x60CA,
            PROC_MSG_STOP_AND_KILL_METEORITE					= 0x60CB,
            PROC_MSG_GENERATE_BOMB								= 0x60CC,
            PROC_MSG_DESTROY_ALL_BOMBS							= 0x60CD,
            PROC_MSG_NOTIFY_OBJ_HIGH_WAVE_INFO					= 0x60CF,
            PROC_MSG_RUSH_BOSS_1ST_DEATH_NOTICE					= 0x60D0,
            PROC_MSG_RUSH_BOSS_1ST_DESTROY_LAVA_LAKE			= 0x60D1,
            PROC_MSG_RUSH_BOSS_1ST_GET_LAVA_LAKE_INDEX			= 0x60D2,
            PROC_MSG_BOSS_OWL_MASTER_KILL_BLOCK					= 0x60D3,
            PROC_MSG_MINIGAME_BALLOON_IMPULSE					= 0x60D4,
            PROC_MSG_MINIGAME_CHARACTER_CATCH					= 0x60D5,
            PROC_MSG_MINIGAME_CHARACTER_INFO					= 0x60D6,
            PROC_MSG_GET_MINIGAME_CHARACTER_EXTERNAL_TRANSFORM	= 0x60D7,
            PROC_MSG_MINIGAME_LAUNCH_IMPULSE					= 0x60D8,
            PROC_MSG_ANTLION_REFLECT							= 0x60DB,
            PROC_MSG_NOTIFY_MID_BOSS_DM_EVENT					= 0x60DC,
            PROC_MSG_NOTIFY_MID_BOSS_OM_EVENT					= 0x60DD,
            PROC_MSG_RESTRICT_RADIO_CONTROL						= 0x60DE,
            PROC_MSG_ENTER_RESTRICT_AREA_RADIO_CONTROL			= 0x60DF,
            PROC_MSG_LEAVE_RESTRICT_AREA_RADIO_CONTROL			= 0x60E0,
            PROC_MSG_GET_SUCKED_INTO_BLACK_HOLE					= 0x60E1,
            PROC_MSG_FINISHED_BEING_SUCKED_INTO_BLACK_HOLE		= 0x60E2,
            PROC_MSG_DIE_OF_BLACK_HOLE							= 0x60E3,
            PROC_MSG_RC_STAGGER									= 0x60E4,
            PROC_MSG_RC_LOCK_ABILITY							= 0x60E5,
            PROC_MSG_RC_UNLOCK_ABILITY							= 0x60E6,
            PROC_MSG_CROWS_NEST_ALERT							= 0x60E7,
            PROC_MSG_RC_GET_HOMING_TARGET_INFO					= 0x60E8,
            PROC_MSG_GET_MINIGAME_LAUNCHER_INFO					= 0x60E9,
            PROC_MSG_MINIGAME_IMPULSE							= 0x60EA,
            PROC_MSG_MINIGAME_RESET								= 0x60EB,
            PROC_MSG_GET_MINIGAME_LAUNCHER_TYPE					= 0x60EE,
            PROC_MSG_GOAL_RING_BATTLE_TARGET					= 0x60EF,
            PROC_MSG_GET_MINIGAME_CHARACTER_SETUP_INFO			= 0x60F0,
            PROC_MSG_MINIGAME_TILT								= 0x60F1,
            PROC_MSG_NOTIFY_LAST_BOSS_EVENT						= 0x60F2,
            PROC_MSG_NOTIFY_OBJ_BIG_OWL_INFO					= 0x60F3,
            PROC_MSG_CANCEL_GLIDING								= 0x60F4,
            PROC_MSG_TAKE_OFF_SNOW_BALL_OBJ						= 0x60F5,
            PROC_MSG_REMOVED_FROM_SNOW_BALL						= 0x60F6,
            PROC_MSG_GET_POINT_MARKER_WARP_INFO					= 0x60F7,
            PROC_MSG_GET_POINT_MARKER_WARP_ITEM_TRANSFORM		= 0x60F8,
            PROC_MSG_NOTIFY_OBJ_TORNADO_INFO					= 0x60F9,
            PROC_MSG_BAKUBAKU_CONTACT							= 0x60FA,
            PROC_MSG_BAKUBAKU_BUMPER_CONTACT					= 0x60FB,
            PROC_MSG_GET_TOTAL_SCORE							= 0x60FC,
            PROC_MSG_MINIGAME_BALLOON_EXPLOSION					= 0x60FD,
            PROC_MSG_MINIGAME_ANIMAL_CATCH						= 0x60FE,
            PROC_MSG_RED_RING_RADAR_EFFECT						= 0x60FF,
            PROC_MSG_END_RED_RING_RADAR							= 0x6101,
            PROC_MSG_COLLECT_MINIGAME_OBJECT					= 0x6104,
            PROC_MSG_GET_NUT_CHIMNEY_MOVE_DIR					= 0x6105,
            PROC_MSG_BALLOON_FISH_IMPULSE						= 0x6106,
            PROC_MSG_MINIGAME_TIME_START						= 0x6107,
            PROC_MSG_MINIGAME_TIME_OVER							= 0x6108,
            PROC_MSG_CONTACT_ASTEROID							= 0x6109,
            PROC_MSG_NOTIFY_CATCH_PLAYER_ATTACK_START			= 0x610B,
            PROC_MSG_NOTIFY_CATCH_PLAYER_ATTACK_END				= 0x610C,
            PROC_MSG_SNOW_BALL_SET_SIZE							= 0x610D,
            PROC_MSG_APPEARE_KNOCKBACK							= 0x610E,
            PROC_MSG_RC_HOLD_ON									= 0x610F,
            PROC_MSG_RC_RELASE_HOLD								= 0x6110,
            PROC_MSG_HOVER_IMPULSE								= 0x6111,
            PROC_MSG_GET_WORLD_POSITION							= 0x6112,
            PROC_MSG_NIGHTS_REALA_COLLISION						= 0x6114,
            PROC_MSG_DLC_BOSS_FM_MIXER_START					= 0x6115,
            PROC_MSG_GET_CHAIN_HOMING_TARGET_SINGLE				= 0x6117,
            PROC_MSG_STOP_GAME_TIMER							= 0x6118,
            PROC_MSG_RESUME_GAME_TIMER							= 0x6119,
            PROC_MSG_DLC_CHANGE_EGG_ROTATION					= 0x611A,
            PROC_MSG_DLC_ZELDA_TAKE_HEART						= 0x611B,
            PROC_MSG_DLC_ZELDA_TAKE_HEART_CONTAINER				= 0x611C,
            PROC_MSG_DLC_ZELDA_GET_NUM_HEART_CONTAINER			= 0x611D,
            PROC_MSG_DLC_ZELDA_NOTICE_STOP_ENEMY				= 0x611E,
            PROC_MSG_DLC_ZELDA_NOTICE_ACTIVE_ENEMY				= 0x611F,
            PROC_MSG_DLC_ZELDA_HEART_ALL_RECOVERY				= 0x6120,
            PROC_MSG_LOFT_BIRD_COLLISION						= 0x6121,
            PROC_MSG_CAMERA_RESET								= 0x8000,
            PROC_MSG_GET_CAMERA_UP_DIRECTION					= 0x8001,
            PROC_MSG_GET_DIRECTION_TO_CAMERA					= 0x8002,
            PROC_MSG_SET_DEBUG_CAMERA							= 0x8003,
            PROC_MSG_IS_DEBUG_CAMERA							= 0x8004,
            PROC_MSG_PUSH_CAMERA_CONTROLLER						= 0x8005,
            PROC_MSG_POP_CAMERA_CONTROLLER						= 0x8006,
            PROC_MSG_CHANGE_PLAYER_CAMERA						= 0x8008,
            PROC_MSG_CAMERA_ON									= 0x8009,
            PROC_MSG_CAMERA_OFF                                 = 0x800A,
            PROC_MSG_GET_CAMERA_DEFAULT_FOVY					= 0x800B,
            PROC_MSG_GET_CAMERA_FOVY							= 0x800C,
            PROC_MSG_CAMERA_BEGIN_QUICK_STEP					= 0x800F,
            PROC_MSG_CAMERA_END_QUICK_STEP						= 0x8010,
            PROC_MSG_SHAKE_CAMERA								= 0x8012,
            PROC_MSG_RANDOM_SHAKE_CAMERA						= 0x8013,
            PROC_MSG_SET_CAMERA_3D_PARAM						= 0x8016,
            PROC_MSG_SET_CAMERA_3D_PARAM_DASH_MODE				= 0x8017,
            PROC_MSG_SET_VIEWPORT								= 0x801A,
            PROC_MSG_GET_VIEWPORT								= 0x801B,
            PROC_MSG_PUSH_LINK_CAMERA							= 0x801C,
            PROC_MSG_POP_LINK_CAMERA							= 0x801D,
            PROC_MSG_GET_LINK_CAMERA_ID							= 0x801E,
            PROC_MSG_GET_RATIO_FOR_POSITION						= 0x801F,
            PROC_MSG_IS_PUSH_CAMERA								= 0x8020,
            PROC_MSG_SET_CAMERA_RESET							= 0x8021,
            PROC_MSG_SET_INTERPOLATE							= 0x8022,
            PROC_MSG_RESET_INTERPOLATE							= 0x8023,
            PROC_MSG_GET_MULTI_CAMERA_STATUS					= 0x8024,
            PROC_MSG_CAMERA_UPDATE								= 0x8026,
            PROC_MSG_FOCUS_CHANGE								= 0x8027,
            PROC_MSG_HIT_EVENT_COLLISION						= 0x9000,
            PROC_MSG_LEAVE_EVENT_COLLISION						= 0x9001,
            PROC_MSG_STAY_TRIGGER								= 0x9002,
            PROC_MSG_CONTACT_COLLISION							= 0x9003,
            PROC_MSG_HUD_UPDATE_INFO							= 0xC000,
            PROC_MSG_HUD_SET_VISIBLE							= 0xC002,
            PROC_MSG_HUD_POINT_GET								= 0xC004,
            PROC_MSG_HUD_BONUS_GET								= 0xC005,
            PROC_MSG_HUD_PAUSE									= 0xC006,
            PROC_MSG_HUD_RESULT									= 0xC007,
            PROC_MSG_HUD_RESULT_TIME_ATTACK						= 0xC008,
            PROC_MSG_HUD_RESULT_CIRCUS							= 0xC009,
            PROC_MSG_HUD_RING									= 0xC00A,
            PROC_MSG_HUD_COUNT_DOWN								= 0xC00B,
            PROC_MSG_HUD_RANK_TIME								= 0xC00D,
            PROC_MSG_HUD_COLOR_POWER_STOCK						= 0xC00E,
            PROC_MSG_HUD_COLOR_POWER							= 0xC00F,
            PROC_MSG_HUD_AIR_TIMER								= 0xC010,
            PROC_MSG_HUD_AIR_METER								= 0xC011,
            PROC_MSG_HUD_UNDER_WATER							= 0xC012,
            PROC_MSG_HUD_SUPER_SONIC							= 0xC013,
            PROC_MSG_HUD_RADICON_INFO							= 0xC014,
            PROC_MSG_HUD_RADICON_START							= 0xC015,
            PROC_MSG_HUD_CIRCUS_UPDATE_INFO						= 0xC016,
            PROC_MSG_HUD_BATTLE_UPDATE_INFO						= 0xC017,
            PROC_MSG_HUD_BATTLE_PL_INFO							= 0xC018,
            PROC_MSG_HUD_BATTLE_FINISH							= 0xC019,
            PROC_MSG_HUD_BATTLE_CIRCUMSTANCES					= 0xC01A,
            PROC_MSG_HUD_TIME_ATTACK_INIT						= 0xC01B,
            PROC_MSG_HUD_TIME_ATTACK_MARK_CHECK_POINT			= 0xC01C,
            PROC_MSG_HUD_TIME_ATTACK_CLEAR						= 0xC01D,
            PROC_MSG_HUD_BUTTON_GUIDE_SKIP_ON_DISP				= 0xC01E,
            PROC_MSG_HUD_BUTTON_GUIDE_SKIP_PUSH					= 0xC01F,
            PROC_MSG_HUD_CMN_BUTTON_DISPALY						= 0xC021,
            PROC_MSG_HUD_COMMON_ACTION							= 0xC022,
            PROC_MSG_END_SIGN									= 0xC025,
            PROC_MSG_START_PHANTOM_SIGN							= 0xC026,
            PROC_MSG_HUD_START_STOMPING							= 0xC027,
            PROC_MSG_HUD_START_HOMING							= 0xC028,
            PROC_MSG_HUD_START_FALL_DEAD						= 0xC02E,
            PROC_MSG_HUD_MAP_HIDE								= 0xC02F,
            PROC_MSG_HUD_RESTART								= 0xC031,
            PROC_MSG_HUD_RETRY									= 0xC032,
            PROC_MSG_IS_PLAYING									= 0xC033,
            PROC_MSG_HUD_KEY_UPDATE								= 0xC034,
            PROC_MSG_TOUCH_SCREEN_TOUCH							= 0xC035,
            PROC_MSG_TOUCH_SCREEN_DETACH						= 0xC036,
            PROC_MSG_TOUCH_SCREEN_MOVE							= 0xC037,
            PROC_MSG_TOUCH_SCREEN_TAP							= 0xC038,
            PROC_MSG_HUD_LOCKON_INFO							= 0xC03A,
            PROC_MSG_STOP_BGM                                   = 0xD001,
            PROC_MSG_CHANGE_BGM_PARAM							= 0xD002,
            PROC_MSG_NOTIFY_SOUND_ENVIRONMENT					= 0xD003,
            PROC_MSG_CHANGE_GLOBAL_AMB_SOUND					= 0xD004,
            PROC_MSG_CHANGE_BGM_BLOCK							= 0xD005,
            PROC_MSG_SOUND_REVERB								= 0xD006,
            PROC_MSG_CHANGE_BGM_VOLUME							= 0xD007,
        } MessageType;

        class Message
        {
        private:
            inline static FUNCTION_PTR(Message*, __thiscall, f_Clone, ASLR(0x0049A630), Message* This);
            inline static FUNCTION_PTR(Message*, __thiscall, __dt, ASLR(0x004AC6A0), Message* This, int a2);

        public:
            MessageType Type;
            int field_08;
            int field_0C;
            short field_10;
            short field_12;
            unsigned int field_14;

            inline static FUNCTION_PTR(Message*, __thiscall, __ct, ASLR(0x0049A5F0), Message* This, MessageType type);

            virtual void Clone() { f_Clone(this); }
            virtual ~Message()
            {
                __dt(this, 0);
            }

            Message()
            {
                Type = (fnd::MessageType)0;
                field_08 = 0;
                field_0C = 0;
                field_10 = 0;
                field_12 = 0;
                field_14 = -1;
            }
        };
    }
}