#pragma once

extern int NUM_HEARTS;
extern int MAX_NUM_HEARTS;
extern int AFTER_DEATH_MAX_NUM_HEARTS;

namespace app
{
    namespace Player
    {
        class CSonic
        {
        public:
            static void AddCallback();
            static void SendPlayerInfo();
        };
    }
}