#include "pch.h"

static void* RETURNADRESS_LOADSETDATA = (void*)ASLR(0x0084AFEC);

void IsProblematicSetData(char* setName)
{
    if (!strncmp((const char*)*(*(((int***)*app::Document) + 3) + 5), "GameModeStageBattle", 17))
        if (!strncmp("zdlc03_obj_00", setName, 13))
            strcpy(setName, "zdlc03_obj_00b");
}

__declspec(naked) void CSetObjectManagerLoadSetDataMidAsmHook()
{
    __asm
    {
        push ebx
        call IsProblematicSetData
        add esp, 4

        mov ecx, [esi+2E4h]
        jmp [RETURNADRESS_LOADSETDATA]
    }
}

void app::CSetObjectManager::LoadSetData()
{
    WRITE_JUMP(ASLR(0x0084AFE6), &CSetObjectManagerLoadSetDataMidAsmHook);
}
