#include "pch.h"

char app::EggManager::AddSpaceCount()
{
    for (app::ObjEgg* egg : Eggs)
        egg->AddSpaceOffset();

    IsSpaceShrink |= 4;
    return IsSpaceShrink;
}
