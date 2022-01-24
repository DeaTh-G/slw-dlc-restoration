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

            void GetHeartData()
            {
                int* playerGOC = CGOCCollectionImpl::GetGOC((void*)((int*)this + 0xCB), GOCCState);
                if (!playerGOC)
                    return;

                PluginStateHeartLife* pluginState = (app::Player::PluginStateHeartLife*)CStateGOC::GetStatePluginPtr(playerGOC, PluginStateHeartLifeString);
                if (!pluginState)
                    return;

                NUM_HEARTS = app::Player::PluginStateHeartLife::GetNumHearts(pluginState);
                MAX_NUM_HEARTS = app::Player::PluginStateHeartLife::GetMaxNumHearts(pluginState);
            }
        };
    }
}