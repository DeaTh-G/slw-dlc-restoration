#include "pch.h"

static void* RETURN_ADDRESS = (void*)ASLR(0x00913A31);

void ChangeAnimalRequirement(int* requirement, const char* name)
{
    if (IsCustomRequirement)
    {
        if (strcmp(name + 0xC, "w1a06") == 0)
            *requirement = WindyHillZone4;
        else if (strcmp(name + 0xC, "w2a06") == 0)
            *requirement = DesertRuins4;
        else if (strcmp(name + 0xC, "w3a06") == 0)
            *requirement = TropicalCoast4;
        else if (strcmp(name + 0xC, "w4a06") == 0)
            *requirement = FrozenFactory4;
        else if (strcmp(name + 0xC, "xmoa01") == 0)
            *requirement = SilentForest4;
        else if (strcmp(name + 0xC, "w2c01") == 0)
            *requirement = SkyRoad4;
        else if (strcmp(name + 0xC, "w7a06") == 0)
            *requirement = LavaMountain4;
        else if (strcmp(name + 0xC, "xbak01") == 0)
            *requirement = HiddenWorld4;
        else if (strcmp(name + 0xC, "w6a03") == 0)
            *requirement = TropicalCoastSecret;
        else if (strcmp(name + 0xC, "xnim02") == 0)
            *requirement = SkyRoadSecret;
    }
    else
    {
        if (!IsWiiURequirement)
            return;

        if (strcmp(name + 0xC, "w1a06") == 0)
            *requirement = 100;
        else if (strcmp(name + 0xC, "w2a06") == 0)
            *requirement = 300;
        else if (strcmp(name + 0xC, "w3a06") == 0)
            *requirement = 1000;
        else if (strcmp(name + 0xC, "w4a06") == 0)
            *requirement = 1500;
        else if (strcmp(name + 0xC, "xmoa01") == 0)
            *requirement = 3000;
        else if (strcmp(name + 0xC, "w2c01") == 0)
            *requirement = 4000;
        else if (strcmp(name + 0xC, "w7a06") == 0)
            *requirement = 5000;
        else if (strcmp(name + 0xC, "xbak01") == 0)
            *requirement = 7000;
        else if (strcmp(name + 0xC, "w6a03") == 0)
            *requirement = 1250;
        else if (strcmp(name + 0xC, "xnim02") == 0)
            *requirement = 4500;
    }
}

__declspec(naked) void CStageInfoReadMission_MissionMidAsmHook()
{
    __asm
    {
        test al, al
        jnz notZeroRet
        mov dword ptr [esi], 0

        notZeroRet:
        push edi
        push esi
        call ChangeAnimalRequirement
        add esp, 8
        jmp [RETURN_ADDRESS]
    }
}

void app::StageInfo::CStageInfo::ReadMission_Mission()
{
    WRITE_JUMP(ASLR(0x00913A27), &CStageInfoReadMission_MissionMidAsmHook);
}
