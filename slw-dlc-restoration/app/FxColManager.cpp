#include "pch.h"
static int p_GrassRes = (int)ASLR(0x00FB6D1C);
static char p_ShadowRes = (char)ASLR(0x00FB6D1D);
static int p_RetAddress = (int)ASLR(0x00904E94);

static D3DADAPTER_IDENTIFIER9 Identifier;

__declspec(naked) void FxColManagerUpdateMidAsmHook()
{
    __asm
    {
        cmp byte ptr[p_GrassRes], 0
        sete cl
        mov[edi + 49Ch], cl
        mov al, [p_ShadowRes]
        mov[edi + 4F4h], al

        mov eax, p_RetAddress
        jmp eax
    }
}

HOOK(void, __fastcall, FxColManagerUpdateHook, ASLR(0x00904E00), int* a1, int* edx, void* updateInfo)
{
    if (strncmp(Identifier.Description, "Intel", 5) == 0 && IsIntelGPUFix)
    {
        const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*)*app::Document);
        if (strncmp(packFileName, "zdlc02", 6) == 0 || strncmp(packFileName, "zdlc03", 6) == 0)
        {
            // Turn off Depth of Field if in Yoshi's Island Zone or The Legend of Zelda Zone
            WRITE_MEMORY(ASLR(0x00FEFC7C), 0);
            WRITE_MEMORY(ASLR(0x0040498C), 0x909090909090);
            WRITE_MEMORY(ASLR(0x00404C3E), 0x909090909090);
        }
        else
        {
            // Turn on Depth of Field if not in Yoshi's Island Zone or The Legend of Zelda Zone
            WRITE_MEMORY(ASLR(0x00FEFC7C), 1);
            WRITE_MEMORY(ASLR(0x0040498C), 0x880D7CFCFE00);
            WRITE_MEMORY(ASLR(0x00404C3E), 0x88157CFCFE00);
        }
    }

    originalFxColManagerUpdateHook(a1, edx, updateInfo);
}

void app::FxColManager::Update()
{
    IDirect3D9Ex* pD3D = NULL;
    Direct3DCreate9Ex(D3D_SDK_VERSION, &pD3D);

    pD3D->GetAdapterIdentifier(0, 0, &Identifier);

    if (strncmp(Identifier.Description, "Intel", 5) == 0 && IsIntelGPUFix)
    {
        INSTALL_HOOK(FxColManagerUpdateHook);
        return;
    }

    WRITE_JUMP(ASLR(0x00904E64), &FxColManagerUpdateMidAsmHook);
}
