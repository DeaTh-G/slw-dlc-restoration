#include "pch.h"

static unsigned int SonicZeldaInfoOffset = ASLR(0x00E02C70);
static const char* GetObjectInfoOffset = (const char*)ASLR(0x0073D380);
static void* ReturnAddress = (void*)ASLR(0x008FF0B5);

__declspec(naked) void VisualVirtualSonicActivateSubMidAsmHook()
{
    __asm
    {
        push 0x00E02C70
        push eax
        call [GetObjectInfoOffset]
        mov edx, [eax + 0x14]
        push ecx
        mov ecx, esp
        add eax, 0x18
        mov [ecx], edx
        push eax
        add ecx, 4
        jmp [ReturnAddress]
    }
}

void app::Player::VisualVirtualSonic::ActivateSub()
{
    WRITE_JUMP(ASLR(0x008FF096), &VisualVirtualSonicActivateSubMidAsmHook);
}
