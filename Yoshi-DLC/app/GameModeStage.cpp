#include "pch.h"
#include "GameModeStage.h"

static void* FadeInAddress = (void*)ASLR(0x004AC5C0);
static void* OriginalAddress = (void*)ASLR(0x00918060);

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
        jmp [OriginalAddress]

        crayPipeWipe:
        call create_crayPipeWipe
        jmp [OriginalAddress]
    }
}

void app::GameModeStage::StateWarp()
{
    WRITE_JUMP(ASLR(0x0091805B), &GameModStageStateWarpMidAsmHook);
}
