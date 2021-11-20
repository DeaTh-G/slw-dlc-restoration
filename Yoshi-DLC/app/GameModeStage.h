#pragma once

namespace app
{
    class GameModeStage
    {
    private:
        inline static FUNCTION_PTR(void, __thiscall, f_SendToGroup, ASLR(0x00937B10), GameModeStage* This, int a2, fnd::Message& message );

    public:
        void SendToGroup(int a1, fnd::Message& message) { return f_SendToGroup(this, a1, message); }

        static void StatePlay();
        static void StateWarp();
        static void RegisterObjInfos();
        static void LoadLevel();
        static void ResetStage();

        int StatePlayZeldaNotice(app::fnd::MessageType messageType)
        {
            if (messageType == app::fnd::MessageType::PROC_MSG_DLC_ZELDA_NOTICE_STOP_ENEMY)
            {
                xgame::MsgDlcZeldaNoticeStopEnemy message{};
                CLevelInfo* levelInfo = (CLevelInfo*)CLevelInfo::GetService((GameDocument*)(((int*)this)[6]));
                levelInfo->SetPlayingZeldaEvent(1);
                SendToGroup(6, message);
                SendToGroup(7, message);
            }
            else if (messageType == app::fnd::MessageType::PROC_MSG_DLC_ZELDA_NOTICE_ACTIVE_ENEMY)
            {
                xgame::MsgDlcZeldaNoticeActiveEnemy message{};
                app::CLevelInfo* levelInfo = (app::CLevelInfo*)app::CLevelInfo::GetService((GameDocument*)(((int*)this)[6]));
                levelInfo->SetPlayingZeldaEvent(0);
                SendToGroup(6, message);
                SendToGroup(7, message);
            }
            return (int)messageType;
        }
    };
}