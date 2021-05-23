#include "pch.h"

HOOK(bool, __fastcall, CStateSonicBaseProcessMessageHook, ASLR(0x00898960), int* This, void* edx, app::Player::CStateGOC* a1, app::fnd::Message& message)
{
    if (message.Type == app::fnd::PROC_MSG_DLC_ZELDA_TAKE_HEART)
        app::Player::StateUtil::RecoveryHeartLife(a1);

    return originalCStateSonicBaseProcessMessageHook(This, edx, a1, message);
}

void app::Player::CStateSonicBase::ProcessMessage()
{
    INSTALL_HOOK(CStateSonicBaseProcessMessageHook);
}