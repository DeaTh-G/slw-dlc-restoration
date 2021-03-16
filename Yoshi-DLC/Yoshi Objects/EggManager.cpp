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

void app::EggManager::DoCheckReleaseAllEgg(int playerNo)
{
    int* playerInfo = ObjUtil::GetPlayerInformation(Document, playerNo);
    if (!playerInfo)
        return;

    bool isDamaged = *((char*)playerInfo + 0x172);
    if (isDamaged)
    {
        if (IsSpaceShrink & 8)
            return;

        if (!playerNo)
        {
            for (ObjEgg* egg : EggsP1)
                egg->StartDrop();
            EggsP1.clear();
        }
        else
        {
            for (ObjEgg* egg : EggsP2)
                egg->StartDrop();
            EggsP2.clear();
        }

        IsSpaceShrink |= 8;
    }
    else
    {
        IsSpaceShrink &= ~8;
    }
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
