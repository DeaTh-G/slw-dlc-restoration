#pragma once

namespace app
{
    namespace Player
    {
        class StateUtil
        {
        public:
            inline static FUNCTION_PTR(unsigned int, __cdecl, IsLandOnMovableGround, ASLR(0x008D6510), int* stateGOC);
            inline static FUNCTION_PTR(unsigned int, __cdecl, IsDead, ASLR(0x008D8C10), int* stateGOC);
            inline static FUNCTION_PTR(unsigned int, __cdecl, SendMission, ASLR(0x008D70C0), int* stateGOC, int a2);
            inline static FUNCTION_PTR(unsigned int, __cdecl, AddRingNum, ASLR(0x008D7BA0), int* stateGOC, int a2);
        };
    }
}