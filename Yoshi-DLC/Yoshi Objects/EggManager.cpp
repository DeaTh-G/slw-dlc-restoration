#include "pch.h"

char app::EggManager::SubSpaceCount(int playerNo)
{
    for (app::ObjEgg* egg : Eggs)
        if (playerNo == egg->PlayerNo)
            egg->SubSpaceOffset();

    IsSpaceShrink |= 4;
    return IsSpaceShrink;
}

int app::EggManager::GetTargetLocusIndex(int index, char playerNo)
{
    int result{};

    int* playerInfo = ObjUtil::GetPlayerInformation(Document, playerNo);
    if (!playerInfo)
        return 0;

    for (size_t i = 0; i <= index; i++)
        result += Eggs.at(i)->SpaceCount;

    return result;
}

char app::EggManager::AddSpaceCount(int playerNo)
{
    for (app::ObjEgg* egg : Eggs)
        if (playerNo == egg->PlayerNo)
            egg->AddSpaceOffset();

    IsSpaceShrink |= 4;
    return IsSpaceShrink;
}
