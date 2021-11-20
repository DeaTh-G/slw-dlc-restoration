#include "pch.h"

static void* FadeInAddress = (void*)ASLR(0x004AC5C0);
static void* OA_STATEPLAY = (void*)ASLR(0x0091B290);
static void* OA_STATEWARP = (void*)ASLR(0x00918060);
static void* OA_REGISTEROBJINFOS = (void*)ASLR(0x00916113);
static const char* ZeldaUIPac = "ui/ui_zdlc03_gamemodestage.pac";

void create_crayPipeWipe()
{
    app::GameMode::FadeIn(0, 3);
    app::HudWipeCrayPipe* obj = new app::HudWipeCrayPipe();
    if (obj)
        app::GameDocument::AddGameObject(*app::Document, obj);
}

__declspec(naked) void GameModStageStatePlayMidAsmHook()
{
    __asm
    {
        push eax
        mov ecx, edi
        call app::GameModeStage::StatePlayZeldaNotice
        add esp, 4
        xor eax, eax
        mov byte ptr[esi + 10h], 1
        jmp [OA_STATEPLAY]
    }
}

__declspec(naked) void GameModStageStateWarpMidAsmHook()
{
    __asm
    {
        cmp dword ptr[esi + 1E4h], 5
        je crayPipeWipe
        call [FadeInAddress]
        jmp [OA_STATEWARP]

        crayPipeWipe:
        call create_crayPipeWipe
        jmp [OA_STATEWARP]
    }
}

bool IsZeldaStage()
{
    if (LinkSonicPlayType == PlayType::NEVER)
        return false;

    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0 || LinkSonicPlayType == PlayType::ALWAYS)
        return true;

    return false;
}

__declspec(naked) void GameModeStageRegisterObjInfosAsmHook()
{
    __asm
    {
        call IsZeldaStage
        jmp[OA_REGISTEROBJINFOS]
    }
}

HOOK(int, __fastcall, GameModeStageLoadLevelHook, ASLR(0x00917730), int* This, void* edx)
{
    int result = originalGameModeStageLoadLevelHook(This, edx);
    if (!IsAlwaysHeartLife)
        return result;

    app::fnd::FileLoaderParam loader{};
    loader.SetMultiLanguageAttr(true, 0);
    app::GameMode::LoadFile("ui/ui_zdlc03_gamemodestage.pac", &loader);
    return result;
}

HOOK(void, __fastcall, GameModeStageResetStageHook, ASLR(0x00916BC0), int* This, void* edx)
{
    originalGameModeStageResetStageHook(This, edx);
    app::CLevelInfo* levelInfo = (app::CLevelInfo*)app::CLevelInfo::GetService((app::GameDocument*)(This[6]));
    levelInfo->SetPlayingZeldaEvent(0);
}

void app::GameModeStage::StatePlay()
{
    WRITE_JUMP(ASLR(0x0091B28A), &GameModStageStatePlayMidAsmHook);
}

void app::GameModeStage::StateWarp()
{
    WRITE_JUMP(ASLR(0x0091805B), &GameModStageStateWarpMidAsmHook);
}

void app::GameModeStage::RegisterObjInfos()
{
    WRITE_JUMP(ASLR(0x0091610E), &GameModeStageRegisterObjInfosAsmHook);
}

void app::GameModeStage::LoadLevel()
{
    INSTALL_HOOK(GameModeStageLoadLevelHook);
}

void app::GameModeStage::ResetStage()
{
    INSTALL_HOOK(GameModeStageResetStageHook);
}