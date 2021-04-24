#include "pch.h"

static void* FadeInAddress = (void*)ASLR(0x004AC5C0);
static void* OA_STATEWARP = (void*)ASLR(0x00918060);
static void* OA_REGISTEROBJINFOS = (void*)ASLR(0x00916113);

void create_crayPipeWipe()
{
    app::GameMode::FadeIn(0, 3);
    app::HudWipeCrayPipe* obj = new app::HudWipeCrayPipe();
    if (obj)
        app::GameDocument::AddGameObject(*app::Document, obj);
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

void app::GameModeStage::StateWarp()
{
    WRITE_JUMP(ASLR(0x0091805B), &GameModStageStateWarpMidAsmHook);
}

bool IsZeldaStage()
{
    if (LinkSonicPlayType == PlayType::NEVER)
        return false;

    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0)
        return true;

    if (LinkSonicPlayType == PlayType::ALWAYS)
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

void app::GameModeStage::RegisterObjInfos()
{
    WRITE_JUMP(ASLR(0x0091610E), &GameModeStageRegisterObjInfosAsmHook);
}
