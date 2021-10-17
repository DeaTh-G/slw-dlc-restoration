#include "pch.h"

static void* ReturnAddressProcMsgPlayerReachGoal = (void*)ASLR(0x008976BA);
static void* ReturnAddressCheckHitDamage = (void*)ASLR(0x0089469A);
static void* ReturnAddressReduceHeartLife = (void*)ASLR(0x008946BC);
static void* StateUtilIsBarrierOffset = (void*)ASLR(0x008D7470);

HOOK(int, __fastcall, ProcMsgTakeObjectHook, ASLR(0x008947B0), int* This, void* edx, int* cStateGOC, app::xgame::MsgTakeObject& message)
{
    int result{};
    if (message.TakeType == app::xgame::MsgTakeObject::EType::YOSHI_ONE_UP)
    {
        int messageVar[4] { 1, 2, 3, 5 };

        if (app::Player::StateUtil::IsDead(cStateGOC) || (app::Player::CStateGOC::GetBlackBoard(cStateGOC)[6] & 0x4000000))
            return 1;
    
        int hudPlayerNo = app::Player::CStateGOC::GetToHudPlayerNumber(cStateGOC);
        app::xgame::MsgExtendPlayer extendMessage{ hudPlayerNo, 1 };
        if (message.field_1C == -1)
        {
            app::Player::StateUtil::SendMission(cStateGOC, 2);
            extendMessage.field_1C = 1;
        }
        else
        {
            extendMessage.field_1C = messageVar[message.field_1C];
        }
        app::Player::CStateGOC::SendMessageImmToGame(cStateGOC, &extendMessage);
        message.field_20 = 1;

        return 1;
    }
    else if (message.TakeType == app::xgame::MsgTakeObject::EType::YOSHI_COIN)
    {
        if (app::Player::StateUtil::IsDead(cStateGOC) || (app::Player::CStateGOC::GetBlackBoard(cStateGOC)[6] & 0x4000000))
            return 1;

        if (!message.IsValidUserID() || message.ShapeID == 6)
        {
            app::Player::StateUtil::AddRingNum(cStateGOC, 1);
            message.field_20 = 1;
        }

        return 1;
    }
    else if (message.TakeType == app::xgame::MsgTakeObject::EType::ZELDA_ONE_UP)
    {
        if (app::Player::StateUtil::IsDead(cStateGOC) || (app::Player::CStateGOC::GetBlackBoard(cStateGOC)[6] & 0x4000000))
            return 1;

        int hudPlayerNo = app::Player::CStateGOC::GetToHudPlayerNumber(cStateGOC);
        app::xgame::MsgExtendPlayer extendMessage{ hudPlayerNo, 10 };
        for (size_t i = 0; i < 10; i++)
            app::Player::StateUtil::SendMission(cStateGOC, 2);

        app::Player::CStateGOC::SendMessageImmToGame(cStateGOC, &extendMessage);
        message.field_20 = 1;

        return 1;
    }
    else
    {
        return result = originalProcMsgTakeObjectHook(This, edx, cStateGOC, message);
    }
}

HOOK(bool, __stdcall, CStateBaseProcessMessageHook, ASLR(0x008981A0), app::Player::CStateGOC* a1, app::fnd::Message& message)
{
    if (message.Type == app::fnd::PROC_MSG_PLAYER_PASS_POINT_MARKER && DoesPointMarkerRestoreLife)
    {
        app::Player::StateUtil::AllRecoveryHeartLife(a1);
        app::HUD::DO_RECOVER_LIFE = true;
        AFTER_DEATH_MAX_NUM_HEARTS = MAX_NUM_HEARTS;
    }
        
    return originalCStateBaseProcessMessageHook(a1, message);
}

static int IsNotZeldaGoal()
{
    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0)
        return 0;

    return 1;
}

__declspec(naked) void StateBaseProcMsgPlayerReachGoalMidAsmHook()
{
    __asm
    {
        call IsNotZeldaGoal
        test eax, eax
        je zeldaGoal
        mov al, [ebp + 0x8]
        mov ecx, [ebp + 0xC]
        jmp [ReturnAddressProcMsgPlayerReachGoal]

        zeldaGoal:
        mov al, 0
        mov ecx, [ebp + 0xC]
        jmp[ReturnAddressProcMsgPlayerReachGoal]
    }
}

__declspec(naked) void StateBaseCheckHitDamageMidAsmHook()
{
    __asm
    {
        call [StateUtilIsBarrierOffset]
        add esp, 4
        push eax
        push esi
        call app::Player::StateUtil::IsHeartLife
        add esp, 4
        pop ecx
        or eax, ecx

        jmp [ReturnAddressCheckHitDamage]
    }
}

__declspec(naked) void StateBaseCheckHitDamageReduceHeartLife()
{
    __asm
    {
        push esi
        call app::Player::StateUtil::ReduceHeartLife
        add esp, 4
        mov ecx, [ebp + 0xC]
        mov byte ptr [ecx], 01

        jmp[ReturnAddressReduceHeartLife]
    }
}

void app::Player::CStateBase::ProcessMessage()
{
    INSTALL_HOOK(CStateBaseProcessMessageHook);
}

void app::Player::CStateBase::ProcMsgTakeObject()
{
    INSTALL_HOOK(ProcMsgTakeObjectHook);
}

void app::Player::CStateBase::CheckHitDamage()
{
    WRITE_JUMP(ASLR(0x00894692), &StateBaseCheckHitDamageMidAsmHook);
    WRITE_JUMP(ASLR(0x008946B6), &StateBaseCheckHitDamageReduceHeartLife);
}

void app::Player::CStateBase::ProcMsgPlayerReachGoal()
{
    WRITE_JUMP(ASLR(0x008976B4), &StateBaseProcMsgPlayerReachGoalMidAsmHook);
}