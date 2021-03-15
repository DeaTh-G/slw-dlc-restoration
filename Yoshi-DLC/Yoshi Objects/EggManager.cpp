#include "pch.h"

char app::EggManager::SubSpaceCount()
{
    for (app::ObjEgg* egg : Eggs)
        egg->SubSpaceOffset();

    IsSpaceShrink |= 4;
    return IsSpaceShrink;
}

char app::EggManager::AddSpaceCount()
{
    for (app::ObjEgg* egg : Eggs)
        egg->AddSpaceOffset();

    IsSpaceShrink |= 4;
    return IsSpaceShrink;
}
