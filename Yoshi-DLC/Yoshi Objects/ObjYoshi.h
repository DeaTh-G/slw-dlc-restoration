#pragma once

namespace app
{
    class ObjYoshiInfo
    {
    public:
        INSERT_PADDING(12);
        int Models[4];
        int Skeleton;
        animation::AnimationResContainer AnimationContainer{};
        int field_60[12];
        int TexSrtAnimContainer[24];

        virtual ~ObjYoshiInfo() {};
        virtual void Load(int* loader) {};
        virtual void Initialize(GameDocument* gameDocument)
        {
            const char* modelNames[4] { "zdlc02_obj_yoshi_green", "zdlc02_obj_yoshi_blue", "zdlc02_obj_yoshi_red", "zdlc02_obj_yoshi_yellow" };
            const char* texSrtAnimNames[24]
            {
                "yos_result01_start_Eye_L",
                "yos_result01_start_Eye_R",
                "yos_result01_loop_Eye_L",
                "yos_result01_loop_Eye_R",
                "yos_result02_start_Eye_L",
                "yos_result02_start_Eye_R",
                "yos_result02_loop_Eye_L",
                "yos_result02_loop_Eye_R",
                "yos_result03_start_Eye_L",
                "yos_result03_start_Eye_R",
                "yos_result03_loop_Eye_L",
                "yos_result03_loop_Eye_R",
                "yos_result04_start_Eye_L",
                "yos_result04_start_Eye_R",
                "yos_result04_loop_Eye_L",
                "yos_result04_loop_Eye_R",
                "yos_result05_start_Eye_L",
                "yos_result05_start_Eye_R",
                "yos_result05_loop_Eye_L",
                "yos_result05_loop_Eye_R",
                "yos_result06_start_Eye_L",
                "yos_result06_start_Eye_R",
                "yos_result06_loop_Eye_L",
                "yos_result06_loop_Eye_R",
            };

            int packFile = 0;
            int animationScript = 0;
            ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(gameDocument));

            for (size_t i = 0; i < 4; i++)
                ObjUtil::GetModelResource(&Models[i], modelNames[i], &packFile);
            ObjUtil::GetSkeletonResource(&Skeleton, "zdlc02_obj_yoshi", packFile);

            ObjUtil::GetAnimationScriptResource(&animationScript, "yoshi", packFile);

            if (animationScript)
                animation::AnimationResContainer::LoadFromBuffer(&AnimationContainer, &animationScript, packFile);

            for (size_t i = 0; i < 12; i++)
                field_60[i] = i % 6;

            for (size_t i = 0; i < 24; i++)
                ObjUtil::GetTexSrtAnimationResource(&TexSrtAnimContainer[i], texSrtAnimNames[i], packFile);
        }
        virtual void Finalize() {};
        virtual const char* GetInfoName()
        {
            return "ObjYoshiInfo";
        }
        virtual void RegistCallback(int* container) {};
    };
}