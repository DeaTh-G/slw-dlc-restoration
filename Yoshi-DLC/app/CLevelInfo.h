#pragma once

namespace app
{
    class CLevelInfo
    {
    public:
        inline static FUNCTION_PTR(int*, __cdecl, staticClass, ASLR(0x009123D0));
        inline static FUNCTION_PTR(int, __thiscall, GetPlayerID, ASLR(0x00912550), void* This, int playerNo);

        static void* GetService(GameDocument* gameDocument)
        {
            void* levelInfo = staticClass();
            void* service = GameDocument::GetServiceByClass(gameDocument, levelInfo);
            return service;
        }

        bool IsPlayingZeldaEvent()
        {
            return ((*(int*)this + 36) & (1 << 20)) != 0;
        }
    };
}