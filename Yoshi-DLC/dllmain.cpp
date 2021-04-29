// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MemAccess.h"
#include "LostCodeLoader.h"
#include "Dependencies/INIReader.h"

bool DisablePipeTransition = false;
bool IsConsistentShadow = false;
PlayType LinkSonicPlayType = PlayType::DEFAULT;
bool IsLinkSonicFixed = false;

char IsYoshiIslandStage()
{
    const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*)*app::Document);
    if (strncmp(packFileName, "zdlc02", 6) == 0)
        return 1;
    return 0;
}

void Initialize()
{
    MessageBox(NULL, L"AAAAAAAAAA", NULL, MB_ICONERROR);

    /* TODO: Please replace this with sane code. */
    WRITE_MEMORY(ASLR(0x00D41252),
        0x68,
        (const char)((uint32_t)app::construct_EggManager >> 0),
        (const char)((uint32_t)app::construct_EggManager >> 8),
        (const char)((uint32_t)app::construct_EggManager >> 16),
        (const char)((uint32_t)app::construct_EggManager >> 24)
    );

    WriteCall((void*)ASLR(0x009159F3), &IsYoshiIslandStage);
    WriteCall((void*)ASLR(0x00915A16), &IsYoshiIslandStage);
    WriteCall((void*)ASLR(0x00916EED), &IsYoshiIslandStage);

    auto create_EnemyShyGuy = &app::create_EnemyShyGuy;
    auto create_EnemyPiranhaPlant = &app::create_EnemyPiranhaPlant;
    auto create_ObjCrayPipe = &app::create_ObjCrayPipe;
    auto create_ObjCrayPipeExit = &app::create_ObjCrayPipeExit;
    auto create_ObjEggBlock = &app::create_ObjEggBlock;
    auto create_ObjEggRotationChangeCollision = &app::create_ObjEggRotationChangeCollision;
    auto create_ObjRotateLift = &app::create_ObjRotateLift;
    auto create_ObjYoshiCoin = &app::create_ObjYoshiCoin;
    auto create_ObjYoshiGoal = &app::create_ObjYoshiGoal;
    auto create_ObjYoshiJumpBoardBig = &app::ObjYoshiJumpBoard::Create_Big;
    auto create_ObjYoshiJumpBoardSmall = &app::ObjYoshiJumpBoard::Create_Small;
    auto create_ObjYoshiSpecialFlower = &app::create_ObjYoshiSpecialFlower;

    auto createObjInfo_EnemyShyGuyInfo = &app::createObjInfo_EnemyShyGuyInfo;
    auto createObjInfo_EnemyPiranhaPlantInfo = &app::createObjInfo_EnemyPiranhaPlantInfo;
    auto createObjInfo_ObjEggBlockInfo = &app::createObjInfo_ObjEggBlockInfo;
    auto createObjInfo_ObjRotateLiftInfo = &app::createObjInfo_ObjRotateLiftInfo;
    auto createObjInfo_ObjYoshiCoinInfo = &app::createObjInfo_ObjYoshiCoinInfo;
    auto createObjInfo_ObjYoshiGoalInfo = &app::createObjInfo_ObjYoshiGoalInfo;
    auto createObjInfo_ObjYoshiJumpBoard = &app::createObjInfo_ObjYoshiJumpBoard;
    auto createObjInfo_ObjYoshiSpecialFlowerInfo = &app::createObjInfo_ObjYoshiSpecialFlowerInfo;

    WRITE_FUNCTION(ASLR(0x00D2A654), *(void**)&create_EnemyShyGuy);
    WRITE_FUNCTION(ASLR(0x00D2A2E4), *(void**)&create_EnemyPiranhaPlant);
    WRITE_FUNCTION(ASLR(0x00D2BB51), *(void**)&create_ObjCrayPipe);
    WRITE_FUNCTION(ASLR(0x00D2BBA1), *(void**)&create_ObjCrayPipeExit);
    WRITE_FUNCTION(ASLR(0x00D2BC94), *(void**)&create_ObjEggBlock);
    WRITE_FUNCTION(ASLR(0x00D2BC41), *(void**)&create_ObjEggRotationChangeCollision);
    WRITE_FUNCTION(ASLR(0x00D2C0F4), *(void**)&create_ObjRotateLift);
    WRITE_FUNCTION(ASLR(0x00D2C1E4), *(void**)&create_ObjYoshiCoin);
    WRITE_FUNCTION(ASLR(0x00D2C234), *(void**)&create_ObjYoshiGoal);
    WRITE_FUNCTION(ASLR(0x00D2C2D4), *(void**)&create_ObjYoshiJumpBoardBig);
    WRITE_FUNCTION(ASLR(0x00D2C324), *(void**)&create_ObjYoshiJumpBoardSmall);
    WRITE_FUNCTION(ASLR(0x00D2C284), *(void**)&create_ObjYoshiSpecialFlower);

    WRITE_FUNCTION(ASLR(0x00D2A64F), *(void**)&createObjInfo_EnemyShyGuyInfo);
    WRITE_FUNCTION(ASLR(0x00D2A2DF), *(void**)&createObjInfo_EnemyPiranhaPlantInfo);
    WRITE_FUNCTION(ASLR(0x00D2BC8F), *(void**)&createObjInfo_ObjEggBlockInfo);
    WRITE_FUNCTION(ASLR(0x00D2C0EF), *(void**)&createObjInfo_ObjRotateLiftInfo);
    WRITE_FUNCTION(ASLR(0x00D2C1DF), *(void**)&createObjInfo_ObjYoshiCoinInfo);
    WRITE_FUNCTION(ASLR(0x00D2C22F), *(void**)&createObjInfo_ObjYoshiGoalInfo);
    WRITE_FUNCTION(ASLR(0x00D2C2CF), *(void**)&createObjInfo_ObjYoshiJumpBoard);
    WRITE_FUNCTION(ASLR(0x00D2C31F), *(void**)&createObjInfo_ObjYoshiJumpBoard);
    WRITE_FUNCTION(ASLR(0x00D2C27F), *(void**)&createObjInfo_ObjYoshiSpecialFlowerInfo);

    auto create_ObjChangeTopViewCollision = &app::create_ObjChangeTopViewCollision;
    auto create_ObjZeldaWarpCollision = &app::create_ObjZeldaWarpCollision;
    auto create_ObjZeldaItemTree = &app::create_ObjZeldaItemTree;
    auto create_ObjZeldaRupee = &app::create_ObjZeldaRupee;
    auto create_ObjZeldaBush = &app::create_ObjZeldaBush;
    auto create_ObjOccludeStone = &app::create_ObjOccludeStone;
    auto create_ObjBreakFloor = &app::create_ObjBreakFloor;

    auto createObjInfo_ObjZeldaItemTreeInfo = &app::createObjInfo_ObjZeldaItemTreeInfo;
    auto createObjInfo_ObjZeldaRupeeInfo = &app::createObjInfo_ObjZeldaRupeeInfo;
    auto createObjInfo_ObjZeldaBushInfo = &app::createObjInfo_ObjZeldaBushInfo;
    auto createObjInfo_ObjOccludeStoneInfo = &app::createObjInfo_ObjOccludeStoneInfo;
    auto createObjInfo_ObjBreakFloorInfo = &app::createObjInfo_ObjBreakFloorInfo;

    WRITE_FUNCTION(ASLR(0x00D2BA61), *(void**)&create_ObjChangeTopViewCollision);
    WRITE_FUNCTION(ASLR(0x00D28691), *(void**)&create_ObjZeldaWarpCollision);
    WRITE_FUNCTION(ASLR(0x00D2C414), *(void**)&create_ObjZeldaItemTree);
    WRITE_FUNCTION(ASLR(0x00D2C4B4), *(void**)&create_ObjZeldaRupee);
    WRITE_FUNCTION(ASLR(0x00D2C374), *(void**)&create_ObjZeldaBush);
    WRITE_FUNCTION(ASLR(0x00D2C054), *(void**)&create_ObjOccludeStone);
    WRITE_FUNCTION(ASLR(0x00D2B9C4), *(void**)&create_ObjBreakFloor);

    WRITE_FUNCTION(ASLR(0x00D2C40F), *(void**)&createObjInfo_ObjZeldaItemTreeInfo);
    WRITE_FUNCTION(ASLR(0x00D2C4AF), *(void**)&createObjInfo_ObjZeldaRupeeInfo);
    WRITE_FUNCTION(ASLR(0x00D2C36F), *(void**)&createObjInfo_ObjZeldaBushInfo);
    WRITE_FUNCTION(ASLR(0x00D2C04F), *(void**)&createObjInfo_ObjOccludeStoneInfo);
    WRITE_FUNCTION(ASLR(0x00D2B9BF), *(void**)&createObjInfo_ObjBreakFloorInfo);

    // Install Yoshi Hooks
    app::xgame::IsDLCStagePurchase::Func();
    app::HUD::CHudGameMainDisplay::__ct();
    app::HUD::CHudGameMainDisplay::SpecialRingUpdate();
    app::CSharedObjInfo::Initialize();
    app::CObjDroppedRing::ProcMsgHitEventCollision();
    app::EnemyBlowOffObject::OnDead();
    app::Player::CPlayer::UpdatePlayerInformation();
    app::Player::CStateBase::ProcMsgTakeObject();
    app::GameModeStageBattle::LoadLoadingScreenData();

    if (!DisablePipeTransition)
        app::GameModeStage::StateWarp();

    // Install Zelda Hooks
    app::Player::CSonic::AddCallback();
    app::Player::CSonic::SendPlayerInfo();
    app::Player::CVisualSonic::ActivateSub();
    app::Player::CVisualSonic::RegisterResource();
    app::Player::CVisualBase::IsLinkCostume();
    app::GameModeStage::RegisterObjInfos();
    /*app::HUD::CHudGameMainDisplay::InitLayer();
    app::HUD::CHudGameMainDisplay::SetInfo();*/

    if (IsLinkSonicFixed)
        app::Player::SonicZeldaInfo::Initialize();

    if (LinkSonicPlayType == PlayType::ALWAYS)
    {
        app::WorldAreaMapInfo::Load();
        app::WorldAreaMapInfo::Initialize();
        app::MinigameCharacterInfo::Load();
        app::MinigameCharacterInfo::Initialize();
    }
}

extern "C"
{
    void _declspec(dllexport) __cdecl Init(ModInfo* modInfo)
    {
        std::string dir = modInfo->CurrentMod->Path;

        INIReader* reader = new INIReader("DLCRestoration.ini");
        if (reader->ParseError() != 0)
            delete reader, reader = new INIReader(dir + "DLCRestoration.ini");

        if (reader->ParseError() != 0)
            MessageBox(NULL, L"Failed to parse DLCRestoration.ini", NULL, MB_ICONERROR);

        IsConsistentShadow = reader->GetBoolean("YoshiTweaks", "isConsistentShadows", false);
        DisablePipeTransition = reader->GetBoolean("YoshiTweaks", "disablePipeTransition", false);
        LinkSonicPlayType = (PlayType)reader->GetInteger("ZeldaTweaks", "linkSonicPlayType", (uint32_t)PlayType::DEFAULT);
        IsLinkSonicFixed = reader->GetBoolean("ZeldaTweaks", "isLinkSonicFixed", false);

        Initialize();
    }
}

BOOL WINAPI DllMain(_In_ HINSTANCE instance, _In_ DWORD reason, _In_ LPVOID reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}