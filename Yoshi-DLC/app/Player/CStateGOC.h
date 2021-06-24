#pragma once

namespace app
{
    namespace Player
    {
        class CStateGOC
        {
        public:
            inline static FUNCTION_PTR(int*, __thiscall, GetBlackBoard, ASLR(0x0085B510), int* This);
            inline static FUNCTION_PTR(int, __thiscall, GetToHudPlayerNumber, ASLR(0x0085BBE0), int* This);
            inline static FUNCTION_PTR(int, __thiscall, SendMessageImmToGame, ASLR(0x0085AC50), int* This, fnd::Message* a2);
            inline static FUNCTION_PTR(void*, __thiscall, GetStatePluginPtr, ASLR(0x0085C410), void* This, char* pluginName);
            inline static FUNCTION_PTR(void*, __thiscall, AddStatePlugin, ASLR(0x0085C3F0), void* This, int a2);
        };
    }
}