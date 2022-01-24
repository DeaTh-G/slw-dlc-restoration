#pragma once

namespace app
{
    class GameModeStageBattle
    {
    public:
        static void LoadLoadingScreenData();
        static void RegisterObjInfos();
        static void InitFirst();
        static void StatePlay();

        void SendToGroup(unsigned int groupID, fnd::Message& message)
        {
            unsigned int groupActorID = ((GameDocument*)*((int**)this + 6))->GetGroupActorID(groupID);
            if (groupActorID)
                ((fnd::CActor*)(((int*)this)[3] + 12))->BroadcastMessageImm(groupActorID, message);
        }

        void StatePlayZeldaNotice(fnd::MessageType messageType)
        {
            if (messageType == fnd::MessageType::PROC_MSG_DLC_ZELDA_NOTICE_STOP_ENEMY)
            {
                xgame::MsgDlcZeldaNoticeStopEnemy message{};
                CLevelInfo* levelInfo = (CLevelInfo*)CLevelInfo::GetService((GameDocument*)(((int*)this)[6]));
                levelInfo->SetPlayingZeldaEvent(1);
                SendToGroup(6, message);
                SendToGroup(7, message);
            }
            else if (messageType == fnd::MessageType::PROC_MSG_DLC_ZELDA_NOTICE_ACTIVE_ENEMY)
            {
                xgame::MsgDlcZeldaNoticeActiveEnemy message{};
                app::CLevelInfo* levelInfo = (CLevelInfo*)CLevelInfo::GetService((GameDocument*)(((int*)this)[6]));
                levelInfo->SetPlayingZeldaEvent(0);
                SendToGroup(6, message);
                SendToGroup(7, message);
            }
        }
    };
}