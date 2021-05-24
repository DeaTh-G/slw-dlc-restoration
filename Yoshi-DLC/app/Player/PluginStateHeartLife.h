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
                return ((This->NumHearts - 1) | (This->NumHearts - 2)) >> 31;
            }

            static bool AllRecovery(PluginStateHeartLife* This)
            {
                if (This->NumHearts >= This->MaxNumHearts)
                    return false;
            
                This->NumHearts = This->MaxNumHearts;
                return true;
            }

            static bool Recovery(PluginStateHeartLife* This)
            {
                if (This->NumHearts >= This->MaxNumHearts)
                    return false;

                This->NumHearts++;
                return true;
            }

            static void IncrementMaxHearts(PluginStateHeartLife* This)
            {
                if (This->MaxNumHearts >= 6)
                    This->MaxNumHearts = 6;
                else
                    This->MaxNumHearts++;

                This->NumHearts = This->MaxNumHearts;
            }
        };
    }
}