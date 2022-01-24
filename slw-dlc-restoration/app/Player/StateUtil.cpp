#include "pch.h"

static void* ReturnAddressScatterRingForDamage = (void*)ASLR(0x008D9FA6);
static void* StateUtilLostRingOffset = (void*)ASLR(0x008D9090);

__declspec(naked) void ScatterRingForDamageMidAsmHook()
{
    __asm
    {
        push esi
        call app::Player::StateUtil::ReduceHeartLife
        add esp, 4
        test al, al
        je scatterRings

        scatterRings:
        push 0x14
        push esi
        call [StateUtilLostRingOffset]
        add esp, 8
        
        jmp [ReturnAddressScatterRingForDamage]
    }
}

void app::Player::StateUtil::ScatterRingForDamage()
{
    WRITE_JUMP(ASLR(0x008D9F9B), &ScatterRingForDamageMidAsmHook);
}
