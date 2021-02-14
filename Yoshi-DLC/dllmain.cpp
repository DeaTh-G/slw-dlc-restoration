// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MemAccess.h"
#include "LostCodeLoader.h"
#include "Dependencies/INIReader.h"
#include "Headers.h"

HOOK(char, __cdecl, IsDLCStagePurchase, ASLR(0x0095DB10), unsigned int dlc)
{
	if (dlc == 1)
		return '\1';
	if (dlc == 2)
		return '\1';
	else
		return originalIsDLCStagePurchase(dlc);
}

char IsYoshiIslandStage()
{
	const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*)*app::Document);
	if (strncmp(packFileName, "zdlc02", 6) == 0)
		return 1;
	return 0;
}

HOOK(int*, __fastcall, CHudGameMainDisplayCtor, ASLR(0x00503290), int* This, int* edx, char a2, int a3, int a4, char a5, int a6, int a7)
{
	originalCHudGameMainDisplayCtor(This, edx, a2, a3, a4, a5, a6, a7);

	if (IsYoshiIslandStage())
	{
		*(This + 0x7A) |= 0x40;
		*(This + 0x7A) |= 0x20;
	}

	return This;
}

int* SetSpecialFlower(int* This)
{
	char DstBuf[32]{};
	void* eggManager = app::EggManager::GetService(*(app::GameDocument**)(This + 0xA));
	if (This[0x52])
	{
		int FlowerFlag = 0;
		if (eggManager)
		{
			for (size_t i = 0; i < 5; ++i)
			{
				int flower = app::EggManager::IsYoshiSpecialFlowerTaked(eggManager, i);
				if (flower)
					FlowerFlag |= 1 << i;
			}

			if (This[0x68] != FlowerFlag)
			{
				This[0x68] = FlowerFlag;
				for (size_t i = 0; i < 5; i++)
				{
					snprintf(DstBuf, 32, "icon_s_ring_%d", i);
					SurfRide::Layer::SetCastPatternIndex(This + 0x52, DstBuf, FlowerFlag & 1);
					FlowerFlag >>= 1;
				}
				if ( !*(This + 0x228))
				{
					app::HUD::SRUtility::SetAnimation(This + 0x52, "Display_2_Anim", 0, -1, 0, 0);
					This[0x3F] = 0x40000000;
					This[0x39] |= 0x20;
				}
			}
		}
	}

	return This;
}

void SpecialFlowerUpdate(int* This, float a2, char a3)
{
	int result = *(This + 0x52);
	if (This[0x52])
	{
		SetSpecialFlower(This);
		int unknown_1 = This[0x39] >> 5;
		if (!a3)
			unknown_1 = ~unknown_1;
		
		int unknown_2 = unknown_1 & 1;

		if (unknown_2)
		{
			This[0x3F] = 0x3F000000;
			return;
		}

		float timer = *(float*)(This + 0x3F);
		if (timer > 0.0)
		{
			float timeDown = timer - a2;
			*(float*)(This + 0x3F) = timeDown;
			if (timeDown <= 0.0)
			{
				if ((This[0x39] & 0x20) != 0)
					app::HUD::SRUtility::SetAnimation(This + 0x52, "Hide_Anim", 0, -1.0, 0, 0);
				else
					app::HUD::SRUtility::SetAnimation(This + 0x52, "Display_2_Anim", 0, -1.0, 0, 0);
				This[0x39] ^= 0x20;
			}
		}
	}
}

HOOK(int*, __fastcall, SpecialRingUpdate, ASLR(0x005027A0), int* This, void* edx, float a2, char a3)
{
	int* result = 0;
	if (IsYoshiIslandStage())
		SpecialFlowerUpdate(This, a2, a3);
	else
		result = originalSpecialRingUpdate(This, edx, a2, a3);
	return result;
}

void Initialize()
{
	//MessageBoxA(NULL, "AAAAAAAAAAAA", NULL, MB_OK);
	WRITE_MEMORY(ASLR(0x0093F790), 0xB2, 0x1, 0x90);

	/* Re-enable initialization of EggManager */
	WRITE_MEMORY(ASLR(0x00E05B80), 0x64, 0xE7, 0xFE);

	/* TODO: Please replace this with sane code. */
	WRITE_MEMORY(ASLR(0x00D41252),
		0x68,
		(const char)((uint32_t)construct_EggManager >> 0),
		(const char)((uint32_t)construct_EggManager >> 8),
		(const char)((uint32_t)construct_EggManager >> 16),
		(const char)((uint32_t)construct_EggManager >> 24)
	);

	WriteCall((void*)ASLR(0x009159F3), &IsYoshiIslandStage);
	WriteCall((void*)ASLR(0x00915A16), &IsYoshiIslandStage);
	WriteCall((void*)ASLR(0x00916EED), &IsYoshiIslandStage);

	auto Create_Big_ObjYoshiJumpBoard = &app::Create_Big_ObjYoshiJumpBoard;
	auto Create_Small_bjYoshiJumpBoard = &app::Create_Small_ObjYoshiJumpBoard;
	auto ObjYoshiJumpBoard_AddCallback = &app::ObjYoshiJumpBoard::AddCallback;
	auto ObjYoshiJumpBoard_ProcessMessage = &app::ObjYoshiJumpBoard::ProcessMessage;
	auto ObjYoshiJumpBoard_Update = &app::ObjYoshiJumpBoard::Update;
	auto ObjYoshiJumpBoardInfo_Initialize = &app::ObjYoshiJumpBoardInfo::Initialize;
	auto ObjYoshiJumpBoardInfo_GetInfoName = &app::ObjYoshiJumpBoardInfo::GetInfoName;

	auto create_ObjYoshiCoin = &app::create_ObjYoshiCoin;
	auto ObjYoshiCoin__ProcessMessage = &app::ObjYoshiCoin::ProcessMessage;
	auto ObjYoshiCoin__AddCallback = &app::ObjYoshiCoin::AddCallback;
	auto ObjYoshiCoinInfo__Initialize = &app::ObjYoshiCoinInfo::Initialize;
	auto ObjYoshiCoinInfo__GetInfoName = &app::ObjYoshiCoinInfo::GetInfoName;

	auto create_ObjRotateLift = &app::create_ObjRotateLift;
	auto ObjRotateLift__ProcessMessage = &app::ObjRotateLift::ProcessMessage;
	auto ObjRotateLift__AddCallback = &app::ObjRotateLift::AddCallback;
	auto ObjRotateLiftInfo__Initialize = &app::ObjRotateLiftInfo::Initialize;
	auto ObjRotateLiftInfo__GetInfoName = &app::ObjRotateLiftInfo::GetInfoName;

	auto create_ObjYoshiSpecialFlower = &app::create_ObjYoshiSpecialFlower;
	auto ObjYoshiSpecialFlower__ProcessMessage = &app::ObjYoshiSpecialFlower::ProcessMessage;
	auto ObjYoshiSpecialFlower__AddCallback = &app::ObjYoshiSpecialFlower::AddCallback;
	auto ObjYoshiSpecialFlowerInfo__Initialize = &app::ObjYoshiSpecialFlowerInfo::Initialize;
	auto ObjYoshiSpecialFlowerInfo__GetInfoName = &app::ObjYoshiSpecialFlowerInfo::GetInfoName;

	auto create_ObjCrayPipe = &app::create_ObjCrayPipe;
	auto ObjCrayPipe__ProcessMessage = &app::ObjCrayPipe::ProcessMessage;
	auto ObjCrayPipe__AddCallback = &app::ObjCrayPipe::AddCallback;

	auto create_ObjCrayPipeExit = &app::create_ObjCrayPipeExit;
	auto ObjCrayPipeExit__ProcessMessage = &app::ObjCrayPipeExit::ProcessMessage;
	auto ObjCrayPipeExit__Update = &app::ObjCrayPipeExit::Update;
	auto ObjCrayPipeExit__AddCallback = &app::ObjCrayPipeExit::AddCallback;

	auto EnemyShyGuyInfo__Initialize = &app::EnemyShyGuyInfo::Initialize;
	auto EnemyShyGuyInfo__GetInfoName = &app::EnemyShyGuyInfo::GetInfoName;
	auto create_EnemyShyGuy = &app::create_EnemyShyGuy;
	auto create_EnemyShyGuyInfo = &app::create_EnemyShyGuy_EnemyShyGuyInfo;
	auto EnemyShyGuy__AddCallback = &app::EnemyShyGuy::AddCallback;

	/*WRITE_FUNCTION(ASLR(0x00D2C2D4), *(void**)&Create_Big_ObjYoshiJumpBoard);
	WRITE_FUNCTION(ASLR(0x00D2C324), *(void**)&Create_Small_bjYoshiJumpBoard);
	WRITE_FUNCTION(ASLR(0x00DE1390), *(void**)&ObjYoshiJumpBoard_AddCallback);
	WRITE_FUNCTION(ASLR(0x00DE137C), *(void**)&ObjYoshiJumpBoard_ProcessMessage);
	WRITE_FUNCTION(ASLR(0x00DE1380), *(void**)&ObjYoshiJumpBoard_Update);
	WRITE_FUNCTION(ASLR(0x00D95040), *(void**)&ObjYoshiJumpBoardInfo_Initialize);
	WRITE_FUNCTION(ASLR(0x00D95048), *(void**)&ObjYoshiJumpBoardInfo_GetInfoName);*/

	WRITE_FUNCTION(ASLR(0x00D94EDC), *(void**)&ObjYoshiCoin__ProcessMessage);
	WRITE_FUNCTION(ASLR(0x00D94EF0), *(void**)&ObjYoshiCoin__AddCallback);
	WRITE_FUNCTION(ASLR(0x00D99654), *(void**)&ObjYoshiCoinInfo__Initialize);
	WRITE_FUNCTION(ASLR(0x00D9965C), *(void**)&ObjYoshiCoinInfo__GetInfoName);
	WRITE_FUNCTION(ASLR(0x00D2C1E4), *(void**)&create_ObjYoshiCoin);
	
	/*WRITE_FUNCTION(ASLR(0x00D94D9C), *(void**)&ObjRotateLift__ProcessMessage);
	WRITE_FUNCTION(ASLR(0x00D94DB0), *(void**)&ObjRotateLift__AddCallback);
	WRITE_FUNCTION(ASLR(0x00D94DE4), *(void**)&ObjRotateLiftInfo__Initialize);
	WRITE_FUNCTION(ASLR(0x00D94DEC), *(void**)&ObjRotateLiftInfo__GetInfoName);
	WRITE_FUNCTION(ASLR(0x00D2C0F4), *(void**)&create_ObjRotateLift);*/

	WRITE_FUNCTION(ASLR(0x00D94FC4), *(void**)&ObjYoshiSpecialFlower__ProcessMessage);
	WRITE_FUNCTION(ASLR(0x00D94FD8), *(void**)&ObjYoshiSpecialFlower__AddCallback);
	WRITE_FUNCTION(ASLR(0x00D9500C), *(void**)&ObjYoshiSpecialFlowerInfo__Initialize);
	WRITE_FUNCTION(ASLR(0x00D95014), *(void**)&ObjYoshiSpecialFlowerInfo__GetInfoName);
	WRITE_FUNCTION(ASLR(0x00D2C284), *(void**)&create_ObjYoshiSpecialFlower);

	WRITE_FUNCTION(ASLR(0x00D94730), *(void**)&ObjCrayPipe__ProcessMessage);
	WRITE_FUNCTION(ASLR(0x00D94744), *(void**)&ObjCrayPipe__AddCallback);
	WRITE_FUNCTION(ASLR(0x00D2BB51), *(void**)&create_ObjCrayPipe);

	WRITE_FUNCTION(ASLR(0x00D947B0), *(void**)&ObjCrayPipeExit__ProcessMessage);
	WRITE_FUNCTION(ASLR(0x00D947B4), *(void**)&ObjCrayPipeExit__Update);
	WRITE_FUNCTION(ASLR(0x00D947C4), *(void**)&ObjCrayPipeExit__AddCallback);
	WRITE_FUNCTION(ASLR(0x00D2BBA1), *(void**)&create_ObjCrayPipeExit);

	WRITE_FUNCTION(ASLR(0x00D93A4C), *(void**)&EnemyShyGuyInfo__Initialize);
	WRITE_FUNCTION(ASLR(0x00D93A54), *(void**)&EnemyShyGuyInfo__GetInfoName);
	WRITE_FUNCTION(ASLR(0x00D93A18), *(void**)&EnemyShyGuy__AddCallback);
	WRITE_FUNCTION(ASLR(0x00D2A64F), *(void**)&create_EnemyShyGuyInfo);
	WRITE_FUNCTION(ASLR(0x00D2A654), *(void**)&create_EnemyShyGuy);
	WRITE_FUNCTION(ASLR(0x00D93A48), ASLR(0x00671D90));
	WRITE_FUNCTION(ASLR(0x00D93A04), ASLR(0x006724B0));

	INSTALL_HOOK(IsDLCStagePurchase);
	INSTALL_HOOK(CHudGameMainDisplayCtor);
	INSTALL_HOOK(SpecialRingUpdate);
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

		IsJumpBoardShadowOn = reader->GetBoolean("Shadows", "isYoshiJumpBoardShadowOn", true);
		//IsEggBlockShadowOn = reader->GetBoolean("Shadows", "isEggBlockShadowOn", false);
		IsYoshiSpecialFlowerShadowOn = reader->GetBoolean("Shadows", "isYoshiSpecialFlowerShadowOn", false);
		IsYoshiCoinShadowOn = reader->GetBoolean("Shadows", "isYoshiCoinShadowOn", false);
		//IsPiranhaPlantShadowOn = reader->GetBoolean("Shadows", "isPiranhaPlantShadowOn", true);
		IsShyGuyShadowOn = reader->GetBoolean("Shadows", "isShyGuyShadowOn", false);

		//PlayPipeOutTransition = reader->GetBoolean("Misc", "playPipeOutTransition", true);
		//ScoreToUnlockLevel = reader->GetInteger("Misc", "scoreToUnlockLevel", 1000000);

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

