#include "pch.h"

static void* ReturnAddressScatterRingForDamageAlwaysHeartLife = (void*)ASLR(0x008D9FB7);
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

        jmp[ReturnAddressScatterRingForDamage]
    }
}

__declspec(naked) void ScatterRingForDamageAlwaysHeartLifeMidAsmHook()
{
    __asm
    {
        push esi
        call app::Player::StateUtil::ReduceHeartLife
        add esp, 4
        jmp [ReturnAddressScatterRingForDamageAlwaysHeartLife]
    }
}

void app::Player::StateUtil::ScatterRingForDamage()
{
    if (IsAlwaysHeartLife)
        WRITE_JUMP(ASLR(0x008D9F9B), &ScatterRingForDamageAlwaysHeartLifeMidAsmHook)
    else
        WRITE_JUMP(ASLR(0x008D9F9B), &ScatterRingForDamageMidAsmHook);
}
