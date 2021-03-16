#include "pch.h"

char app::EggManager::SubSpaceCount(int playerNo)
{
    if (!playerNo)
        for (app::ObjEgg* egg : EggsP1)
            egg->SubSpaceOffset();
    else
        for (app::ObjEgg* egg : EggsP2)
            egg->SubSpaceOffset();

    IsSpaceShrink |= 4;
    return IsSpaceShrink;
}

bool app::EggManager::AddEgg(ObjEgg* egg)
{
    if (!egg->PlayerNo)
    {
        if (EggsP1.size() >= 0xC)
            return false;

        EggsP1.push_back(egg);
        return true;
    }
    else
    {
        if (EggsP2.size() >= 0xC)
            return false;

        EggsP2.push_back(egg);
        return true;
    }
}

int app::EggManager::GetTargetLocusIndex(int index, char playerNo)
{
    int result{};

    int* playerInfo = ObjUtil::GetPlayerInformation(Document, playerNo);
    if (!playerInfo)
        return 0;

    if (!playerNo)
    {
        for (size_t i = 0; i <= index; i++)
            result += EggsP1.at(i)->SpaceCount;
    }
    else
    {
        for (size_t i = 0; i <= index; i++)
            result += EggsP2.at(i)->SpaceCount;
    }

    return result;
}

char app::EggManager::AddSpaceCount(int playerNo)
{
    if (!playerNo)
        for (app::ObjEgg* egg : EggsP1)
            egg->AddSpaceOffset();
    else
        for (app::ObjEgg* egg : EggsP2)
            egg->AddSpaceOffset();

    IsSpaceShrink |= 4;
    return IsSpaceShrink;
}
