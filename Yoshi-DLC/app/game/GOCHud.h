#pragma once

namespace app
{
    namespace game
    {
        class GOCHud
        {
        public:
            struct Description
            {
                const char* field_00;
                int field_04;
                int field_08;
                uint8_t field_09;
                uint8_t field_0A;
                uint8_t field_0B;
                uint8_t field_0D;
                int field_0C;
                int field_10;
                int field_14;
                int field_18;
            };

        private:
            inline static FUNCTION_PTR(void, __thiscall, f_Setup, ASLR(0x004BFF90), GOCHud* This, Description* hudDescriptor);
            inline static FUNCTION_PTR(int*, __thiscall, f_SetupProject, ASLR(0x004BFA20), GOCHud* This, int* a2, const char* fileName, int packFile);
            inline static FUNCTION_PTR(HudLayerController*, __thiscall, f_CreateLayerController, ASLR(0x004BFEB0), GOCHud* This, int a1, const char* sceneName, const char* layerName, int a4);
            inline static FUNCTION_PTR(HudLayerController*, __thiscall, f_CreateLayerController_1, ASLR(0x004BFD70), GOCHud* This, int a1, const char* layerName, int a3);

        public:
            void Setup(Description* hudDescriptor)
            {
                f_Setup(this, hudDescriptor);
            }

            int* SetupProject(int* a2, const char* fileName, int packFile)
            {
                return f_SetupProject(this, a2, fileName, packFile);
            }

            HudLayerController* CreateLayerController(int a1, const char* sceneName, const char* layerName, int a4)
            {
                return f_CreateLayerController(this, a1, sceneName, layerName, a4);
            }

            HudLayerController* CreateLayerController(int a1, const char* layerName, int a3)
            {
                return f_CreateLayerController_1(this, a1, layerName, a3);
            }
        };
    }
}