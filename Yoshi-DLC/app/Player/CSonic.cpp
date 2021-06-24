#include "pch.h"

static void* OA_SENDPLAYERINFO = (void*)ASLR(0x00861AC3);
int NUM_HEARTS{};
int MAX_NUM_HEARTS{};
int AFTER_DEATH_MAX_NUM_HEARTS{};

void GetHeartData(int* This)
{
    int* playerGOC = app::CGOCCollectionImpl::GetGOC((void*)(This + 0xCB), CStateGOC);
    if (!playerGOC)
        return;

    app::Player::PluginStateHeartLife* pluginState = (app::Player::PluginStateHeartLife*)app::Player::CStateGOC::GetStatePluginPtr(playerGOC, PluginStateHeartLifeString);
    if (!pluginState)
        return;

    NUM_HEARTS = app::Player::PluginStateHeartLife::GetNumHearts(pluginState);

    if (MAX_NUM_HEARTS > AFTER_DEATH_MAX_NUM_HEARTS)
        app::Player::PluginStateHeartLife::SetMaxNumHearts(pluginState, AFTER_DEATH_MAX_NUM_HEARTS);
    MAX_NUM_HEARTS = app::Player::PluginStateHeartLife::GetMaxNumHearts(pluginState);
}

__declspec(naked) void CSonicSendPlayerInfoMidAsmHook()
{
    __asm
    {
        push esi
        call GetHeartData
        push MAX_NUM_HEARTS
        push NUM_HEARTS
        push ebx
        push ecx
        jmp[OA_SENDPLAYERINFO]
    }
}

HOOK(void, __fastcall, CSonicAddCallbackHook, ASLR(0x00861B00), int* This, void* edx, int* a2)
{
    const char* packFileName = app::ObjUtil::GetStagePackName(*app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0 || IsAlwaysHeartLife)
        *(short*)(This[215] + 56) |= 0x1000;

    originalCSonicAddCallbackHook(This, edx, a2);
}

void app::Player::CSonic::SendPlayerInfo()
{
    WRITE_JUMP(ASLR(0x00861ABD), &CSonicSendPlayerInfoMidAsmHook);
}

void app::Player::CSonic::AddCallback()
{
    INSTALL_HOOK(CSonicAddCallbackHook);
}