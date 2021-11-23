#include "pch.h"

static void* RegisterResourceInfosOffset = (void*)ASLR(0x008F73F0);
static void* SetupGameStatusOffset = (void*)ASLR(0x00938520);
static void* RegisterObjInfosReturnAddress = (void*)ASLR(0x009387E4);
static void* InitFirstReturnAddress = (void*)ASLR(0x0093985C);
static void* OA_STATEPLAY = (void*)ASLR(0x0093AEC3);
static void* STATEPLAY_END = (void*)ASLR(0x0093AECB);

HOOK(int, __fastcall, LoadLoadingScreenDataHook, ASLR(0x0093A6D0), int* This, void* edx, int a2)
{
    app::fnd::FileLoaderParam loader{};
    loader.SetMultiLanguageAttr(true, 0);
    app::GameMode::LoadFile("ui/ui_zdlc02_load_all.pac", &loader);
    loader.SetMultiLanguageAttr(0, 0);
    loader.SetMultiLanguageAttr(true, 0);
    app::GameMode::LoadFile("ui/ui_zdlc03_load_all.pac", &loader);
    loader.SetMultiLanguageAttr(0, 0);


    loader.SetMultiLanguageAttr(true, 0);
    app::GameMode::LoadFile("ui/ui_zdlc03_gamemodestage.pac", &loader);
    loader.SetMultiLanguageAttr(0, 0);

    int result = originalLoadLoadingScreenDataHook(This, edx, a2);
    return result;
}

static bool IsZeldaBattleStage()
{
    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0 || IsVirtualLinkSonic)
        return true;

    return false;
}

__declspec(naked) void GameModStageBattleStatePlayMidAsmHook()
{
    __asm
    {
        cmp eax, 611Eh
        jnz activeEvent
        push eax
        call app::GameModeStageBattle::StatePlayZeldaNotice
        jmp [STATEPLAY_END]

        activeEvent:
        cmp eax, 611Fh
        jnz original
        push eax
        call app::GameModeStageBattle::StatePlayZeldaNotice
        jmp [STATEPLAY_END]

        original:
        cmp eax, 5003h
        jmp [OA_STATEPLAY]
    }
}

__declspec(naked) void GameModeStageBattleRegisterObjInfosAsmHook()
{
    __asm
    {
        mov ebx, eax
        call IsZeldaBattleStage
        test al, 1
        je setZeldaStage

        setZeldaStage:
        or [ebp - 4], 16

        mov eax, ebx
        call [RegisterResourceInfosOffset]
        jmp [RegisterObjInfosReturnAddress]
    }
}

__declspec(naked) void GameModeStageBattleInitFirstMidAsmHook()
{
    __asm
    {
        call IsZeldaBattleStage
        test al, 1
        je setZeldaStage

        setZeldaStage:
        or dword ptr[edi + 0x90], 0x80000
        
        mov ecx, esi
        call [SetupGameStatusOffset]
        jmp[InitFirstReturnAddress]
    }
}

void app::GameModeStageBattle::LoadLoadingScreenData()
{
    INSTALL_HOOK(LoadLoadingScreenDataHook);
}

void app::GameModeStageBattle::StatePlay()
{
    WRITE_JUMP(ASLR(0x0093AEBE), &GameModStageBattleStatePlayMidAsmHook);
}

void app::GameModeStageBattle::RegisterObjInfos()
{
    WRITE_JUMP(ASLR(0x009387DF), &GameModeStageBattleRegisterObjInfosAsmHook);
}

void app::GameModeStageBattle::InitFirst()
{
    WRITE_JUMP(ASLR(0x00939855), &GameModeStageBattleInitFirstMidAsmHook);
}
