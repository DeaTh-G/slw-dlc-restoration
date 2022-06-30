#include "pch.h"

static void* ReturnAddressSetInfo = (void*)ASLR(0x00505E75);
static void* ReturnAddressIsZeldaInitLayer = (void*)ASLR(0x00503993);
static void* RingUpdateOffset = (void*)ASLR(0x005025D0);
static void* SetAnimationOffset = (void*)ASLR(0x00525070);

HOOK(void, __fastcall, SetInfoHook, ASLR(0x00505DF0), app::HUD::CHudGameMainDisplay* This, void* edx, int a2, float deltaTime)
{
    originalSetInfoHook(This, edx, a2, deltaTime);

    This->HeartLifeUpdate(a2, deltaTime, 0);
}

HOOK(int*, __fastcall, SpecialRingUpdateHook, ASLR(0x005027A0), int* This, void* edx, float a2, char a3)
{
    int* result = 0;
    const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*) * app::Document);
    if (strncmp(packFileName, "zdlc02", 6) == 0)
        app::HUD::SpecialFlowerUpdate(This, a2, a3);
    else
        result = originalSpecialRingUpdateHook(This, edx, a2, a3);
    return result;
}

HOOK(int*, __fastcall, CHudGameMainDisplayHook, ASLR(0x00503290), int* This, int* edx, char a2, int a3, int a4, char a5, int a6, int a7)
{
    originalCHudGameMainDisplayHook(This, edx, a2, a3, a4, a5, a6, a7);

    const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*) * app::Document);

    if (strncmp(packFileName, "zdlc02", 6) == 0 || strncmp(packFileName, "zdlc03", 6) == 0)
    {
        if (strncmp(packFileName, "zdlc02", 6) == 0)
        {
            *(This + 0x7A) |= 0x40;
            *(This + 0x7A) |= 0x20;
        }
        else if (strncmp(packFileName, "zdlc03", 6) == 0)
        {
            *(This + 0x7A) |= 0x80;
            *(This + 0x7A) |= 0x100;
            return This;
        }
    }

    if (IsAlwaysHeartLife)
    {
        if (strncmp(packFileName, "zdlc01", 6) != 0)
            *(This + 0x7A) |= 0x20;

        *(This + 0x7A) |= 0x80;
        *(This + 0x7A) |= 0x100;
    }

    return This;
}

char IsZeldaHud()
{
    const char* packFileName = app::ObjUtil::GetStagePackName((app::GameDocument*) * app::Document);
    if (strncmp(packFileName, "zdlc03", 6) == 0)
        return 1;
    return 0;
}

HOOK(bool, __fastcall, CHudGameMainDisplayProcessMessageHook, ASLR(0x005060D0), app::HUD::CHudGameMainDisplay* This, int* edx, app::fnd::Message& message)
{
    switch (message.Type)
    {
    case app::fnd::PROC_MSG_DLC_ZELDA_HEART_ALL_RECOVERY:
        This->ProcMsgDlcZeldaHeartAllRecovery((app::xgame::MsgDlcZeldaHeartAllRecovery&)message);
        return true;
    case app::fnd::PROC_MSG_DLC_ZELDA_TAKE_HEART_CONTAINER:
        This->ProcMsgDlcZeldaTakeHeartContainer((app::xgame::MsgDlcZeldaTakeHeartContainer&)message);
        return true;
    default:
        return originalCHudGameMainDisplayProcessMessageHook(This, edx, message);
    }
}

__declspec(naked) void CHudGameMainDisplayInitLayerMidAsmHook()
{
    __asm
    {
        call IsZeldaHud
        test al, al
        jne isZelda
        call[SetAnimationOffset]
        jmp[ReturnAddressIsZeldaInitLayer]

        isZelda:
        jmp[ReturnAddressIsZeldaInitLayer]
    }
}

HOOK(void, __fastcall, CHudGameMainDisplayInitLayerHook, ASLR(0x00503780), app::HUD::CHudGameMainDisplay* This, int* edx)
{
    originalCHudGameMainDisplayInitLayerHook(This, edx);

    if ((This->Flags & 0x100) || IsAlwaysHeartLife)
    {
        This->field_1A8 = 3;
        ++((int*)This->field_E0)[1];
        This->LayerController = This->GOCHud->CreateLayerController(This->field_E0, "info_ring_zdlc03", 0x12);
        This->LayerController->PlayAnimation("Intro_Anim", 0, 0);
        This->LayerController->SetVisible(true);

        app::game::HudLayerController::PlayInfo volumeInfo{};
        volumeInfo.AnimationName = "life_volume";
        volumeInfo.field_0C = This->field_1A8 * 20;
        This->LayerController->PlayAnimation(volumeInfo);

        app::game::HudLayerController::PlayInfo playUsual{};
        playUsual.AnimationName = "Usual_Anim";
        playUsual.IsLooping = true;
        This->LayerController->PlayAnimation(playUsual);
    }
}

void app::HUD::CHudGameMainDisplay::__ct()
{
    INSTALL_HOOK(CHudGameMainDisplayHook);
}

void app::HUD::CHudGameMainDisplay::SpecialRingUpdate()
{
    INSTALL_HOOK(SpecialRingUpdateHook);
}

void app::HUD::CHudGameMainDisplay::ProcessMessage()
{
    INSTALL_HOOK(CHudGameMainDisplayProcessMessageHook);
}

void app::HUD::CHudGameMainDisplay::InitLayer()
{
    WRITE_JUMP(ASLR(0x0050398E), &CHudGameMainDisplayInitLayerMidAsmHook);
    INSTALL_HOOK(CHudGameMainDisplayInitLayerHook);
}

void app::HUD::CHudGameMainDisplay::SetInfo()
{
    INSTALL_HOOK(SetInfoHook);
}
