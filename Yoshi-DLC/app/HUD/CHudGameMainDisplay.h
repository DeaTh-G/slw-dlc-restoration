#pragma once

namespace app
{
    namespace HUD 
    {
        inline static const char* HeartMode[] =
        {
            "mode_heart4",
            "mode_heart5",
            "mode_heart6"
        };

        inline static int* SetSpecialFlower(int* This)
        {
            char DstBuf[32]{};
            EggManager* eggManager = EggManager::GetService(*(GameDocument**)(This + 0xA));
            if (This[0x52])
            {
                int FlowerFlag = 0;
                if (eggManager)
                {
                    for (size_t i = 0; i < 5; ++i)
                    {
                        int flower = eggManager->IsYoshiSpecialFlowerTaked(i);
                        if (flower)
                            FlowerFlag |= 1 << i;
                    }

                    if (This[0x68] != FlowerFlag)
                    {
                        This[0x68] = FlowerFlag;
                        for (size_t i = 0; i < 5; i++)
                        {
                            snprintf(DstBuf, 32, "icon_s_ring_%d", i);
                            SurfRide::Layer::SetCastPatternIndex(This + 0x52, DstBuf, FlowerFlag & 1);
                            FlowerFlag >>= 1;
                        }
                        if (!*(This + 0x228))
                        {
                            HUD::SRUtility::SetAnimation(This + 0x52, "Display_2_Anim", 0, -1, 0, 0);
                            This[0x3F] = 0x40000000;
                            This[0x39] |= 0x20;
                        }
                    }
                }
            }

            return This;
        }

        inline static void SpecialFlowerUpdate(int* This, float a2, char a3)
        {
            int result = *(This + 0x52);
            if (This[0x52])
            {
                SetSpecialFlower(This);
                int unknown_1 = This[0x39] >> 5;
                if (!a3)
                    unknown_1 = ~unknown_1;

                int unknown_2 = unknown_1 & 1;

                if (unknown_2)
                {
                    This[0x3F] = 0x3F000000;
                    return;
                }

                float timer = *(float*)(This + 0x3F);
                if (timer > 0.0)
                {
                    float timeDown = timer - a2;
                    *(float*)(This + 0x3F) = timeDown;
                    if (timeDown <= 0.0)
                    {
                        if ((This[0x39] & 0x20) != 0)
                            HUD::SRUtility::SetAnimation(This + 0x52, "Hide_Anim", 0, -1.0, 0, 0);
                        else
                            HUD::SRUtility::SetAnimation(This + 0x52, "Display_2_Anim", 0, -1.0, 0, 0);
                        This[0x39] ^= 0x20;
                    }
                }
            }
        }

        class CHudGameMainDisplay : public GameObject
        {
        public:
            game::GOCHud* GOCHud;
            int field_D4;
            game::HudLayerController* LayerController;
            int field_DC;
            int field_E0;
            int field_E4;
            int field_E8;
            int field_EC;
            int field_F0;
            int field_F4;
            int field_F8;
            int field_FC;
            int field_100;
            int field_104;
            int field_108;
            int field_10C;
            int field_110;
            int field_114;
            int field_118;
            int field_11C;
            int field_120;
            int field_124;
            int field_128;
            int field_12C;
            int field_130;
            int field_134[17];
            int field_178[2];
            int field_180;
            int field_184;
            int field_188;
            int field_18C;
            int field_190;
            int field_194;
            int field_198;
            int field_19C;
            int field_1A0;
            int field_1A4;
            int field_1A8;
            int field_1AC;
            int field_1B0;
            int field_1B4;
            float Time;
            float field_1BC;
            float field_1C0;
            int field_1C4;
            int field_1C8;
            int field_1CC;
            int field_1D0;
            int field_1D4;
            int field_1D8;
            int field_1DC;
            int field_1E0;
            int field_1E4;
            int Flags;
            int field_1EC;
            int field_1F0;
            int field_1F4;
            int field_1F8;
            int field_1FC;
            int field_200;
            int field_204;
            int field_208;
            int field_20C;
            int field_210;
            int field_214;
            int field_218;
            int field_21C;
            int field_220;
            int field_224;
            float field_228;
            int field_22C;
            int field_230;
            int field_234;
            int field_238;
            int field_23C;
            int field_240;
            int field_244;
            int field_248;
            int field_24C;
            int field_250;
            int field_254;
            int field_258;
            int field_25C;
            int field_260;
            int field_264;
            float field_268;
            int field_26C;
            int field_270;
            float field_274;
            float field_278;
            int field_27C;
            int field_280;
            int field_284;
            int field_288;
            int field_28C;
            int field_290;
            int field_294;

        public:
            static void __ct();
            static void SpecialRingUpdate();
            static void ProcessMessage();
            static void InitLayer();
            static void RingUpdate();

            void HeartLifeUpdate(int a2, float deltaTime, int a4)
            {
                if (!(Flags & 0x100))
                    return;

                if (field_1EC & 1)
                {
                    Time -= deltaTime;
                    if (Time <= 0 && !LayerController->IsCurrentAnimation("Hide_Anim"))
                    {
                        app::game::HudLayerController::PlayInfo hideInfo{};
                        hideInfo.AnimationName = "Hide_Anim";
                        LayerController->PlayAnimation(hideInfo);
                    }
                }

                if (field_1A8 != NUM_HEARTS)
                {
                    field_1A8 = NUM_HEARTS;
                    
                    app::game::HudLayerController::PlayInfo volumeInfo{};
                    volumeInfo.AnimationName = "life_volume";
                    volumeInfo.field_0C = field_1A8 * 20;
                    LayerController->PlayAnimation(volumeInfo);

                    app::game::HudLayerController::PlayInfo playUsual{};
                    playUsual.AnimationName = "Usual_Anim";
                    playUsual.IsLooping = true;
                    LayerController->PlayAnimation(playUsual);
                }

                LayerController->SetVisible(true);
            }

            void ProcMsgDlcZeldaHeartAllRecovery(xgame::MsgDlcZeldaHeartAllRecovery& message)
            {
                field_278 = 0;
                field_27C = 1;
            }

            void ProcMsgDlcZeldaTakeHeartContainer(xgame::MsgDlcZeldaTakeHeartContainer& message)
            {
                field_278 = 0;
                field_27C = 1;
            }
        };
    }
}