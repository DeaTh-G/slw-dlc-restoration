#pragma once

namespace app
{
    namespace Player
    {
        class PluginStateHeartLife
        {
        public:
            static int GetNumHearts(int* a1) { return a1[7]; }
            static int GetMaxNumHearts(int* a1) { return a1[6]; }
        };
    }
}