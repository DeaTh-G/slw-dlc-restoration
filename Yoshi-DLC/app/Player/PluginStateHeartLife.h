#pragma once

namespace app
{
    namespace Player
    {
        class PluginStateHeartLife
        {
        private:
            INSERT_PADDING(24);
            unsigned int MaxNumHearts{};
            unsigned int NumHearts{};
            float Time{};
            bool IsHeartAlert{};

        public:
            static int GetNumHearts(PluginStateHeartLife* This) { return This->NumHearts; }
            static int GetMaxNumHearts(PluginStateHeartLife* This) { return This->MaxNumHearts; }
            static int Damaged(PluginStateHeartLife* This)
            {
                This->NumHearts--;
                printf("NumHearts: %d\n", GetNumHearts(This));
                return ((This->NumHearts - 1) | (This->NumHearts - 2)) >> 31;
            }
        };
    }
}