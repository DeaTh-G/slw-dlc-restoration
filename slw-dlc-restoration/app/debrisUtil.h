#pragma once

namespace app
{
    namespace debrisUtil
    {
        inline static FUNCTION_PTR(int*, __cdecl, CreateRandomSpaceDebris, ASLR(0x00846F80), GameDocument* document, debris::SRandomSpaceDebrisInfo* debrisInfo);
    }
}