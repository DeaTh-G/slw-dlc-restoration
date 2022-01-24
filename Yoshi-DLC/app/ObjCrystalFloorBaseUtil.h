#pragma once

namespace app
{
    class ObjCrystalFloorBaseUtil
    {
    public:
        inline static FUNCTION_PTR(int, __cdecl, CreateUpDebris, ASLR(0x0076BB70), int* a1, csl::math::Vector3* a2, debris::ResRandomSpaceDebris* a3, GameDocument* a4);
        inline static FUNCTION_PTR(int, __cdecl, CreateDownDebris, ASLR(0x0076BD60), int* a1, csl::math::Vector3* a2, debris::ResRandomSpaceDebris* a3, GameDocument* a4);
    };
}