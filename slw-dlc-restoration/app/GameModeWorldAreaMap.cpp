#include "pch.h"

static void* CtorReturn = (void*)ASLR(0x009473FD);

static void* StateModeSelectReturn0 = (void*)ASLR(0x009463DF);
static void* StateModeSelectReturn1 = (void*)ASLR(0x009463D6);
static void* StateModeSelectReturn2 = (void*)ASLR(0x009463CD);
static void* StateModeSelectReturn = (void*)ASLR(0x009463A1);
static void* StateModeSelectStateChangeReturn = (void*)ASLR(0x009463D4);
static void* CWorldMapStateModeSelcect_GetResult = (void*)ASLR(0x00950F30);

__declspec(naked) void StateModeSelectMidAsmHook()
{
	__asm
	{
		call [CWorldMapStateModeSelcect_GetResult]
		sub eax, edi
		mov ecx, esi
		cmp eax, 0
		je ResultZero
		dec eax
		cmp eax, 0
		je ResultOne
		dec eax
		cmp eax, 1
		je ResultTwo
		cmp eax, 0
		je ResultThree
		jmp [StateModeSelectReturn]

		ResultZero:
		jmp [StateModeSelectReturn0]

		ResultOne:
		jmp [StateModeSelectReturn1]

		ResultTwo:
		jmp [StateModeSelectReturn2]

		ResultThree:
		mov eax, app::GameModeWorldAreaMap::StateRadiconSelect
		mov dword ptr[ebp - 14h], eax
		sub esp, 10h
		mov eax, esp
		mov dword ptr[ebp - 10h], edi
		jmp [StateModeSelectStateChangeReturn]
	}
}

__declspec(naked) void GameModeWorldAreaMapCtorMidAsmHook()
{
    __asm
    {
        lea edx, [ebp - 0x30]
        push edx
        mov ecx, esi
        call app::GameModeWorldAreaMap::CreateCircusTents
        mov ecx, esi
        jmp [CtorReturn]
    }
}

void app::GameModeWorldAreaMap::Constructor()
{
    // Disable Screen Switch Popup
    WRITE_MEMORY(ASLR(0x00946A2D), 0x90, 0x90, 0x90, 0x90, 0x90);
    WRITE_MEMORY(ASLR(0x00946A32), 0x90, 0x90, 0x90, 0x90);
    WRITE_MEMORY(ASLR(0x00946A36), 0x90, 0x90);
    WRITE_MEMORY(ASLR(0x00946A38), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
    WRITE_MEMORY(ASLR(0x00946A3F), 0x90, 0x90);
    WRITE_MEMORY(ASLR(0x00946A41), 0x90, 0x90);

    // Disable -1 initialiazers
    WRITE_MEMORY(ASLR(0x009473D3), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
    WRITE_MEMORY(ASLR(0x009473D9), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
    WRITE_MEMORY(ASLR(0x009473DF), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
    WRITE_MEMORY(ASLR(0x009473E5), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
    WRITE_MEMORY(ASLR(0x009473EB), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
    WRITE_MEMORY(ASLR(0x009473F1), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
    WRITE_MEMORY(ASLR(0x009473F7), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);

    WRITE_JUMP(ASLR(0x009473D3), &GameModeWorldAreaMapCtorMidAsmHook);
}

void app::GameModeWorldAreaMap::StateModeSelect()
{
	WRITE_MEMORY_WITH_TYPE(ASLR(0x009462DC), uint32_t, 0xBC0);
	WRITE_MEMORY_WITH_TYPE(ASLR(0x009464C1), uint32_t, 0xBC0);
	WRITE_MEMORY_WITH_TYPE(ASLR(0x009462DE), uint32_t, 0xBC0);

	WRITE_JUMP(ASLR(0x00946390), StateModeSelectMidAsmHook);
}