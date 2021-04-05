#include "pch.h"

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
    else
    {
        return result = originalProcMsgTakeObjectHook(This, edx, cStateGOC, message);
    }
}

void app::Player::CStateBase::ProcMsgTakeObject()
{
    INSTALL_HOOK(ProcMsgTakeObjectHook);
}
